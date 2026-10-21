# AxxistSteam USVFS launcher — source

This directory contains the complete GPL-3.0 source of `axxist-usvfs.exe` and
`axxist-usvfs-x86.exe`, the small launchers that AxxistSteam uses to start a game through
[Mod Organizer 2's USVFS](https://github.com/ModOrganizer2/usvfs).

AxxistSteam itself is Elastic-2.0 and never links USVFS. The launchers are separate executables,
so the GPL-3.0 engine stays at arm's length.

## Files

| File | Purpose |
|---|---|
| `main.cpp` | Launcher: creates the USVFS instance, maps mods/overwrite, starts the game, waits |
| `CMakeLists.txt` | Builds both architectures against the upstream USVFS headers/import libraries |
| `LICENSE` | GNU GPL-3.0 text (the launcher is GPL-3.0) |
| `README.md` | Build and behaviour notes |

## Upstream USVFS source

The binaries are built against the official USVFS release **v0.5.7.2**. Its complete source is
available from the upstream repository at that exact tag:

```sh
git clone --branch v0.5.7.2 https://github.com/ModOrganizer2/usvfs.git usvfs
```

The corresponding upstream source archive is also attached to the AxxistSteam release as
`usvfs-v0.5.7.2-source.tar.gz`.

## Rebuilding the launchers

1. Obtain the USVFS **v0.5.7.2** source and build/install it (or use the official prebuilt
   `usvfs_v0.5.7.2.7z`, which provides `include/`, `lib/usvfs_x64.lib`, `lib/usvfs_x86.lib`,
   `bin/usvfs_x64.dll` and `bin/usvfs_x86.dll`).
2. Configure and build this launcher with Visual Studio + CMake, once per architecture:

```powershell
cmake -S . -B build-x64 -A x64 -DUSVFS_DIR=<path-to-usvfs>
cmake --build build-x64 --config Release --target axxist-usvfs

cmake -S . -B build-x86 -A Win32 -DUSVFS_DIR=<path-to-usvfs>
cmake --build build-x86 --config Release --target axxist-usvfs
```

The Windows launchers link the MSVC runtime statically. The upstream `usvfs_*.dll` files use the
dynamic runtime and need the Microsoft Visual C++ 2015-2022 Redistributable (present on normal
Steam systems).
