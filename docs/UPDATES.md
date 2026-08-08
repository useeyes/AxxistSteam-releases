# Update Guide

AxxistSteam has a built-in update system. This page explains how it works and what to expect.

---

## How updates work

1. **Background check** - the app checks for a new version every few minutes (and on every start)
   against the update manifest in this repository.
2. **Notification** - when a newer version exists, a badge appears on the notification bell in the
   title bar. Nothing is ever installed silently.
3. **You're in control** - open the notification (quick-update button included) or
   **Settings → Updates** to see what's new before you decide.
4. **Install** - one click downloads and installs the update. The app closes and restarts itself.
5. **What's new** - on the first launch after an update, a dialog shows you exactly what changed.

## Settings → Updates

The update center in the settings shows:

- the **currently installed version**
- a **"Check now"** button for a manual check
- the **pending update** (if any) with its changelog and the install/download button
- the **complete update history** of all published versions

## Platform differences

| Build | In-app update | Why |
|-------|---------------|-----|
| Windows installer (`.exe`) | ✅ one click | The installer can replace the app files |
| Linux AppImage | ✅ one click | The AppImage file can be replaced in place |
| Linux `.deb` / `.rpm` | ➡️ download link | System packages are managed by `dpkg`/`rpm` - the app can't replace its own files. You'll get a notification and a direct download link instead. |

## Verifying updates

All updater artifacts are **cryptographically signed** (minisign). The app verifies the signature
before installing anything - a tampered or corrupted download is rejected automatically.

The public key is baked into the app; the manifest it polls is
[`latest.json`](../latest.json) in this repository.

## Troubleshooting

**"Could not fetch a valid release JSON"** - the app couldn't reach the update server. Check your
internet connection; the check retries automatically every few minutes.

**Update button does nothing** - make sure no SteamUtility helper process is stuck; the updater
terminates them automatically before installing. If it still fails, download the latest installer
manually from the [releases page](https://github.com/useeyes/AxxistSteam-releases/releases) and
install over the existing version - your settings and accounts are preserved.

**Where are updates downloaded from?** - exclusively from this repository's
[GitHub Releases](https://github.com/useeyes/AxxistSteam-releases/releases). Nowhere else.
