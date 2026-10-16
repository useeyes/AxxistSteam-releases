# AxxistSteam installation guide

This guide applies to the public **AxxistSteam v1.1.0** release. Future versions
are available from the same [latest release page](https://github.com/useeyes/AxxistSteam-releases/releases/latest).

## Download

Download only from the official public repositories:

- [AxxistSteam releases](https://github.com/useeyes/AxxistSteam-releases/releases)
- [Latest release](https://github.com/useeyes/AxxistSteam-releases/releases/latest)

The release page contains the compiled application files. The private
[AxxistSteam source repository](https://github.com/useeyes/AxxistSteam) is not
needed for installation.

### Which file should I choose?

| Platform | Recommended file | Alternative |
|---|---|---|
| Windows 10/11 64-bit | `AxxistSteam_<version>_x64-setup.exe` | `AxxistSteam_<version>_x64-portable.zip` |
| Ubuntu/Debian 64-bit | `AxxistSteam_<version>_amd64.deb` | `AxxistSteam_<version>_amd64.AppImage` |
| Other modern Linux 64-bit | `AxxistSteam_<version>_amd64.AppImage` | `.rpm` where provided |

The exact filenames on the release page are authoritative. Replace `<version>`
with the current release version, for example `1.1.0`.

## Windows

### Requirements

- Windows 10 or Windows 11, 64-bit
- An internet connection for Steam and online services
- Steam installed for Steam-dependent features
- Microsoft WebView2 Runtime

You do not need Rust, Node.js, Python, pnpm, the .NET SDK, or any programming
language/runtime used to build AxxistSteam.

The installer normally uses the WebView2 bootstrapper. If WebView2 is missing,
install it from Microsoft's official page:

- [Microsoft Edge WebView2 Runtime](https://developer.microsoft.com/en-us/microsoft-edge/webview2/)

### Installer installation

1. Open the [latest release](https://github.com/useeyes/AxxistSteam-releases/releases/latest).
2. Download `AxxistSteam_<version>_x64-setup.exe`.
3. Start the installer and follow the prompts.
4. Start AxxistSteam from the Start menu or its desktop shortcut.

The installer is configured for a per-machine installation. Windows may ask for
administrator approval during installation; this is normal. The application
itself should not be run as Administrator.

### Portable version

1. Download `AxxistSteam_<version>_x64-portable.zip`.
2. Extract the complete ZIP to a folder that you control.
3. Run `AxxistSteam.exe` from the extracted folder.

Keep the `libs` folder next to `AxxistSteam.exe`. Do not run the executable from
inside the ZIP and do not delete files from the extracted folder.

## Linux

### Requirements

- 64-bit Linux with a graphical desktop session
- A supported GTK/WebKitGTK desktop environment
- `libsecret` for secure credential storage
- An internet connection for Steam and online services
- Steam installed for Steam-dependent features

No Rust, Node.js, Python, pnpm, or .NET SDK installation is required for a
published binary. For the most predictable results, use a current
Ubuntu/Debian or Fedora-based distribution. Very old distributions may not
provide a compatible WebKitGTK runtime.

### AppImage (recommended for most Linux users)

1. Download `AxxistSteam_<version>_amd64.AppImage` from the
   [latest release](https://github.com/useeyes/AxxistSteam-releases/releases/latest).
2. Make it executable:

   ```bash
   chmod +x AxxistSteam_*_amd64.AppImage
   ```

3. Start it:

   ```bash
   ./AxxistSteam_*_amd64.AppImage
   ```

Some distributions require FUSE for AppImages. On Ubuntu/Debian, install the
compatibility package if the AppImage reports a FUSE error:

```bash
sudo apt update
sudo apt install libfuse2
```

On newer Ubuntu releases, the package may be named `libfuse2t64` instead:

```bash
sudo apt install libfuse2t64
```

If FUSE cannot be installed, try:

```bash
./AxxistSteam_*_amd64.AppImage --appimage-extract-and-run
```

### Debian/Ubuntu package

1. Download `AxxistSteam_<version>_amd64.deb`.
2. Install it with:

   ```bash
   sudo apt install ./AxxistSteam_*_amd64.deb
   ```

   Using `apt install ./file.deb` is preferred because it resolves package
   dependencies where the distribution provides them.

3. Start AxxistSteam from the application menu or with:

   ```bash
   AxxistSteam
   ```

If your system reports missing libraries, install common runtime packages:

```bash
sudo apt update
sudo apt install libsecret-1-0 libgtk-3-0 libwebkit2gtk-4.1-0
```

Package names vary by distribution. Do not download random `.so` files from
the internet; use your distribution's official package repository.

### RPM-based distributions

If an `.rpm` is included in the release, install it using your package manager,
for example:

```bash
sudo dnf install ./AxxistSteam_*_x86_64.rpm
```

Install missing GTK/WebKitGTK and `libsecret` packages only from official
Fedora/openSUSE repositories. Package names differ between distributions.

## First launch and Steam login

1. Start AxxistSteam.
2. Sign in using the normal Steam sign-in flow when available.
3. Complete Steam Guard verification if Steam requests it.
4. Keep Steam Guard enabled on every account.

Credentials and Steam session data are stored locally using the operating
system's credential store. Never send your Steam password, `steamLoginSecure`
cookie, screenshots of recovery codes, or exported session data to anyone.

Steam must be installed and running for features that interact with the local
Steam client. Some web features also require a public Steam profile or working
Steam Community access.

## Updates

AxxistSteam checks the official public release endpoint for updates. Updates are
cryptographically signed; an update that does not match the application's
trusted public key is rejected. You can also update manually from the
[latest release page](https://github.com/useeyes/AxxistSteam-releases/releases/latest).

Do not use unofficial mirrors or files sent through chat links.

## Troubleshooting

### Windows: the app does not open

- Install or repair the [WebView2 Runtime](https://developer.microsoft.com/en-us/microsoft-edge/webview2/).
- Restart Windows after a newly added WebView2 installation.
- Confirm that you downloaded the 64-bit installer.
- Do not launch the app as Administrator unless support specifically requires it.

### Linux: `Permission denied`

Make the AppImage executable with `chmod +x`, then run it again.

### Linux: FUSE/AppImage error

Install `libfuse2` or `libfuse2t64` as described above, or use
`--appimage-extract-and-run`.

### Linux: missing WebKitGTK or GTK library

Install matching packages from the official repositories for your distribution.
Avoid mixing packages from unrelated distributions.

### Steam login or account data is missing

Check that Steam is installed, that the correct account is selected, and that
Steam Guard is working. If you suspect a session was exposed, revoke other
Steam sessions, change the Steam password, and review the account security
settings immediately.

## Security notes

- Download releases only from `useeyes/AxxistSteam-releases`.
- Keep Windows Defender or Linux security tooling enabled.
- Never share private keys, Steam cookies, passwords, or recovery codes.
- AxxistSteam cannot protect an account if the host computer already contains
  malware, a keylogger, or an attacker with administrator/root access.
