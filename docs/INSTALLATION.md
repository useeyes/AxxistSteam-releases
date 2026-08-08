# Installation Guide

This guide walks you through installing AxxistSteam on Windows and Linux.

---

## Windows

### Requirements
- Windows 10 or 11 (64-bit)
- ~200 MB free disk space

### Install
1. Download the latest `AxxistSteam_*_x64-setup.exe` from the
   [latest release](https://github.com/useeyes/AxxistSteam-releases/releases/latest).
2. Run the installer. Windows SmartScreen may ask for confirmation because the installer is not
   EV-code-signed - click **"More info" → "Run anyway"**.
3. The installer requires administrator rights (per-machine install) and places AxxistSteam in
   `C:\Program Files\AxxistSteam`.
4. Launch AxxistSteam from the Start menu or desktop shortcut.

### Uninstall
Use **Settings → Apps → AxxistSteam → Uninstall**, or the uninstaller in the installation folder.

---

## Linux

### Option A: `.deb` package (Debian, Ubuntu, Kali, Mint, …)

1. Download the latest `axxiststeam_*_amd64.deb` from the
   [latest release](https://github.com/useeyes/AxxistSteam-releases/releases/latest).
2. Install it:
   ```bash
   sudo dpkg -i axxiststeam_*_amd64.deb
   sudo apt-get install -f   # resolves the libsecret dependency if missing
   ```
3. Launch `AxxistSteam` from your application menu.

**Note:** `.deb` installations cannot update themselves in-app (system packages are owned by
your package manager). The app will still notify you about new versions and offer a download
link - see [UPDATES.md](UPDATES.md).

### Option B: AppImage (any distribution)

1. Download the latest `*_amd64.AppImage` from the
   [latest release](https://github.com/useeyes/AxxistSteam-releases/releases/latest).
2. Make it executable:
   ```bash
   chmod +x AxxistSteam_*.AppImage
   ```
3. Run it:
   ```bash
   ./AxxistSteam_*.AppImage
   ```

The AppImage is portable (stores its data next to the system app-data dir) and **can update
itself in-app** with one click.

### Required dependency
AxxistSteam stores your Steam Web API key in the system keychain, so `libsecret` is required:
```bash
# Debian/Ubuntu/Kali
sudo apt install libsecret-1-0
```

---

## First start

1. Sign in with your Steam account (credentials or QR code).
2. Add a Steam Web API key in **Settings → General** so profile data (level, avatar, VAC status)
   can be loaded. Get one free at <https://steamcommunity.com/dev/apikey>.
3. Done - the dashboard shows your account card; click it for quick tools, analytics and your
   activity log.

---

## Updating

See [UPDATES.md](UPDATES.md) - the short version: the app notifies you, you click once, done.
