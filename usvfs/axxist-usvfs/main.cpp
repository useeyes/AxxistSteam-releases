// axxist-usvfs - the Windows half of AxxistSteam's launch-intercepted VFS.
//
// This program is GPL-3.0 (see LICENSE in this directory) because it links Mod Organizer 2's
// USVFS. It is deliberately a *separate* executable: AxxistSteam itself is Elastic-2.0 and never
// links USVFS, it only spawns this launcher with a plan.
//
// Contract (built by `src-tauri/src/vfs_usvfs.rs::UsvfsPlan::command_line`):
//
//   axxist-usvfs.exe --instance <name> --root <game-dir>
//                    --map <source> <destination> [--map ...]
//                    --overwrite <dir> -- <game> [args...]
//
// The launcher creates a USVFS instance, maps the merged mods over the real game install, maps the
// profile's overwrite directory as the create target (so the game never writes into its install),
// maps per-profile file overrides, starts the game through the USVFS proxy and waits for it. The
// real install directory is never modified.

#include <windows.h>
#include <shellapi.h>

#include <cstdint>
#include <usvfs/logging.h>
#include <usvfs/usvfs.h>
#include <usvfs/usvfsparameters.h>

#include <cstdio>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Options {
  std::wstring instance;
  std::wstring root;
  std::wstring overwrite;
  std::vector<std::pair<std::wstring, std::wstring>> maps;
  std::vector<std::wstring> command;
};

std::wstring logPath;

std::wstring parentDir(const std::wstring& path) {
  const auto slash = path.find_last_of(L"\\/");
  return slash == std::wstring::npos ? std::wstring() : path.substr(0, slash);
}

std::string toUtf8(const std::wstring& value) {
  if (value.empty()) {
    return {};
  }
  const int size = WideCharToMultiByte(CP_UTF8, 0, value.c_str(), -1, nullptr, 0, nullptr, nullptr);
  if (size <= 1) {
    return {};
  }
  std::string result(static_cast<size_t>(size - 1), '\0');
  WideCharToMultiByte(CP_UTF8, 0, value.c_str(), -1, result.data(), size, nullptr, nullptr);
  return result;
}

void logLine(const std::wstring& line) {
  // Best-effort diagnostic log next to the profile's overlay runtime directory. Never fatal: a
  // missing log must not stop the game from launching.
  if (logPath.empty()) {
    return;
  }
  const std::wstring text = line + L"\r\n";
  HANDLE file = CreateFileW(logPath.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE,
                            nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) {
    return;
  }
  DWORD written = 0;
  WriteFile(file, text.data(), static_cast<DWORD>(text.size() * sizeof(wchar_t)), &written,
            nullptr);
  CloseHandle(file);
}

void fatal(const std::wstring& message) {
  logLine(L"ERROR: " + message);
  OutputDebugStringW((L"axxist-usvfs: " + message + L"\n").c_str());
  MessageBoxW(nullptr, message.c_str(), L"AxxistSteam VFS", MB_ICONERROR | MB_OK);
  ExitProcess(1);
}

// Standard Windows argv quoting (the algorithm used by CommandLineToArgvW / MSVCRT).
std::wstring quoteArgument(const std::wstring& argument) {
  if (argument.empty()) {
    return L"\"\"";
  }
  if (argument.find_first_of(L" \t\n\v\"") == std::wstring::npos) {
    return argument;
  }

  std::wstring quoted = L"\"";
  size_t backslashes = 0;
  for (const wchar_t ch : argument) {
    if (ch == L'\\') {
      ++backslashes;
    } else if (ch == L'"') {
      quoted.append(backslashes * 2 + 1, L'\\');
      quoted.push_back(L'"');
      backslashes = 0;
    } else {
      quoted.append(backslashes, L'\\');
      backslashes = 0;
      quoted.push_back(ch);
    }
  }
  quoted.append(backslashes * 2, L'\\');
  quoted.push_back(L'"');
  return quoted;
}

[[noreturn]] void usage(const std::wstring& problem) {
  fatal(problem +
        L"\n\nUsage:\naxxist-usvfs --instance <name> --root <game-dir> --map <source> "
        L"<destination> ... --overwrite <dir> -- <game> [args...]");
}

Options parseArguments(int argc, wchar_t** argv) {
  Options options;
  int index = 1;

  auto takeValue = [&](const wchar_t* flag) -> std::wstring {
    if (index + 1 >= argc) {
      usage(std::wstring(L"missing value for ") + flag);
    }
    return argv[++index];
  };

  for (; index < argc; ++index) {
    const std::wstring arg = argv[index];
    if (arg == L"--") {
      for (int rest = index + 1; rest < argc; ++rest) {
        options.command.emplace_back(argv[rest]);
      }
      break;
    }
    if (arg == L"--instance") {
      options.instance = takeValue(L"--instance");
    } else if (arg == L"--root") {
      options.root = takeValue(L"--root");
    } else if (arg == L"--overwrite") {
      options.overwrite = takeValue(L"--overwrite");
    } else if (arg == L"--map") {
      if (index + 2 >= argc) {
        usage(L"missing source/destination for --map");
      }
      const std::wstring source = argv[++index];
      const std::wstring destination = argv[++index];
      options.maps.emplace_back(source, destination);
    } else {
      usage(L"unknown argument: " + arg);
    }
  }

  if (options.instance.empty() || options.root.empty() || options.overwrite.empty()) {
    usage(L"--instance, --root and --overwrite are required");
  }
  if (options.maps.empty()) {
    usage(L"at least one --map is required");
  }
  if (options.command.empty()) {
    usage(L"no game command after --");
  }
  return options;
}

bool isDirectory(const std::wstring& path) {
  const DWORD attributes = GetFileAttributesW(path.c_str());
  return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool isFile(const std::wstring& path) {
  const DWORD attributes = GetFileAttributesW(path.c_str());
  return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool ensureDirectory(const std::wstring& path) {
  if (path.empty() || isDirectory(path)) {
    return !path.empty();
  }
  const std::wstring parent = parentDir(path);
  if (!parent.empty() && !ensureDirectory(parent)) {
    return false;
  }
  return CreateDirectoryW(path.c_str(), nullptr) != FALSE || GetLastError() == ERROR_ALREADY_EXISTS;
}

}  // namespace

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
  int argc = 0;
  wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
  if (argv == nullptr) {
    fatal(L"could not parse the command line");
  }

  const Options options = parseArguments(argc, argv);
  LocalFree(argv);

  const std::wstring runtime = parentDir(options.overwrite);
  if (!runtime.empty()) {
    logPath = runtime + L"\\usvfs-launcher.log";
  }

  logLine(L"starting instance=" + options.instance + L" root=" + options.root +
          L" overwrite=" + options.overwrite);

  if (!isDirectory(options.root)) {
    fatal(L"game folder not found: " + options.root);
  }
  if (!ensureDirectory(options.overwrite)) {
    fatal(L"could not create the overwrite folder: " + options.overwrite);
  }

  usvfsParameters* parameters = usvfsCreateParameters();
  if (parameters == nullptr) {
    fatal(L"could not allocate USVFS parameters");
  }
  const std::string instanceName = toUtf8(options.instance);
  usvfsSetInstanceName(parameters, instanceName.c_str());
  usvfsSetLogLevel(parameters, LogLevel::Warning);
  usvfsSetDebugMode(parameters, FALSE);

  if (!usvfsCreateVFS(parameters)) {
    usvfsFreeParameters(parameters);
    fatal(L"could not create the USVFS instance (another launch may still be shutting down)");
  }
  usvfsFreeParameters(parameters);

  // 1. Merged mods over the real install. No create target here: writes must not land in the
  //    generated overlay (it is rebuilt from the profile on every launch).
  for (const auto& [source, destination] : options.maps) {
    if (isFile(source)) {
      if (!isFile(destination)) {
        logLine(L"skipping file override, target missing: " + destination);
        continue;
      }
      if (!usvfsVirtualLinkFile(source.c_str(), destination.c_str(), 0)) {
        logLine(L"failed to map file " + source + L" -> " + destination);
      }
      continue;
    }
    if (!isDirectory(source)) {
      logLine(L"skipping missing mapping source: " + source);
      continue;
    }
    if (!usvfsVirtualLinkDirectoryStatic(
            source.c_str(), destination.c_str(),
            LINKFLAG_RECURSIVE | LINKFLAG_MONITORCHANGES)) {
      logLine(L"failed to map directory " + source + L" -> " + destination);
    }
  }

  // 2. The overwrite layer is both visible and the create target for the whole virtual root, so
  //    every file the game creates/modifies stays out of the install directory. Added last so it
  //    wins over mod files with the same path, matching the profile's priority.
  if (!usvfsVirtualLinkDirectoryStatic(options.overwrite.c_str(), options.root.c_str(),
                                       LINKFLAG_RECURSIVE | LINKFLAG_CREATETARGET |
                                           LINKFLAG_MONITORCHANGES)) {
    logLine(L"failed to map the overwrite folder; game writes may reach the install directory");
  }

  // 3. Start the game through the USVFS proxy. The command line is rebuilt with Windows quoting
  //    because CreateProcess takes one string, not an argv array.
  std::wstring commandLine;
  for (size_t i = 0; i < options.command.size(); ++i) {
    if (i != 0) {
      commandLine.push_back(L' ');
    }
    commandLine += quoteArgument(options.command[i]);
  }

  STARTUPINFOW startup{};
  startup.cb = sizeof(startup);
  PROCESS_INFORMATION process{};

  const std::wstring& application = options.command.front();

  logLine(L"launching: " + commandLine);

  // Inherit the caller's working directory: Steam already starts the launch option in the game
  // folder, and for "run through VFS" tools the app sets an explicit working directory. Using
  // --root here would be wrong for Bethesda-style games whose mod root is `<install>\Data`.
  if (!usvfsCreateProcessHooked(application.c_str(), commandLine.data(), nullptr, nullptr, TRUE, 0,
                                nullptr, nullptr, &startup, &process)) {
    const DWORD error = GetLastError();
    usvfsDisconnectVFS();
    fatal(L"could not start the game through USVFS (error " + std::to_wstring(error) + L")");
  }

  WaitForSingleObject(process.hProcess, INFINITE);
  DWORD exitCode = 1;
  GetExitCodeProcess(process.hProcess, &exitCode);
  CloseHandle(process.hThread);
  CloseHandle(process.hProcess);

  logLine(L"game exited with code " + std::to_wstring(exitCode));
  usvfsDisconnectVFS();
  return static_cast<int>(exitCode);
}
