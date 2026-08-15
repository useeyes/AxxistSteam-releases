# Privacy Policy

**Last updated: 2026-08-14**

AxxistSteam is a desktop application that automates parts of your own Steam account on your own
computer. This policy explains what is stored, what leaves your machine, when, and why. It describes
the actual behavior of the current release - not aspirations.

## The short version

- Your Steam credentials and session tokens **never leave your computer**, except to Steam's own
  servers as part of signing in or using Steam features.
- Your saved credentials (agent-session refresh tokens, Steam Community cookies, your own Steam Web
  API key if you set one) are stored in your **operating system's keychain** (Windows Credential
  Manager / libsecret via Secret Service), not in plain-text files.
- AxxistSteam has **no analytics, no tracking, no telemetry**.
- Outbound connections are limited to: Steam (Valve), GitHub (update checks, release metadata, the
  in-app notification feed), and a static asset CDN (cover art and product artwork). Details below.

## What is stored locally

All of the following lives only on your device, in the app's data directory (or next to the exe for
portable builds), unless marked otherwise:

| Data | Where | Why |
|------|-------|-----|
| Agent-mode refresh tokens | **OS keychain** | Keep Steam sign-ins alive between restarts |
| Steam Community cookies | **OS keychain** | Card farming / community features |
| Your own Steam Web API key (optional) | **OS keychain** | Override the built-in shared key for profile/library data |
| Settings & preferences | App data directory (`settings.json`) | App configuration (accounts roster stores usernames only, never tokens) |
| Owned-games, achievements, favorites & queue caches | App data directory, per SteamID64 | Avoids re-fetching on every launch |
| Booster presets & boost/analytics history | App data directory | Persist your presets and session statistics across restarts |
| Free-game claim history | App data directory | Marks already-claimed games |
| Custom background image | App data directory | Optional theme customization |
| Log files | App data directory | Local diagnostics; never uploaded |
| Per-account activity log, custom display names, UI preferences | Browser local storage | Dashboard activity view and app preferences |

"Remove account" in Settings deletes that account's keychain entries and roster record; other
accounts are untouched. Settings → Debug → "Uninstall AxxistSteam" deletes **all** of the above
before removing the app.

## What leaves your computer, and when

| Destination | What is sent | What is received | When |
|-------------|--------------|------------------|------|
| `steamcommunity.com`, `api.steampowered.com`, `store.steampowered.com` | Sign-in credentials (only while signing in), session cookies/tokens, Steam Web API calls (with the built-in shared key or your own) | Your profile, library, playtime, achievements, card-drop and store data | Sign-in; every feature that reads your Steam data (games list, card farming, achievement unlocker, free games, dashboard analytics) |
| Steam CDNs (`steamstatic.com`, `akamaihd.net`) | Anonymous GET requests | Game artwork (header/cover images) | Whenever game art is displayed |
| `github.com`, `raw.githubusercontent.com` | Anonymous GET requests | Update manifest, release history, in-app notification feed | Background update checks and notification polls |
| `pub-ca47df86597c4ccbb6ddf4366ca7f733.r2.dev` (Cloudflare R2 static asset bucket) | Anonymous GET requests | Cover art wall and product mockup for the sign-in screen, ad/promo creatives | Sign-in screen; dashboard ad slots |

- Update downloads come from GitHub Releases and every updater artifact is **minisign-signed**; the
  app verifies the signature against a baked-in public key before installing anything.
- No data is sent to any analytics, advertising, or tracking endpoint. The R2 bucket is static
  hosting operated by AxxistSteam; it serves files only and receives nothing but plain GET requests.
- Clicking a Discord/Steam link in the app opens your external browser - those visits are between
  you and those sites.

## What never happens

- No credentials, tokens, cookies, or API keys are transmitted anywhere except Steam's own
  endpoints.
- No gameplay data, chat content, or friend data is read or sent anywhere.
- No crash reports, usage statistics, or device identifiers are uploaded - there is no telemetry
  pipeline at all.
- The device fingerprint used by the (currently unused) subscription/licensing groundwork never
  leaves your machine in this release.

## Your responsibility

Automating a Steam account (idling, achievement unlocking, card farming) is at your own risk and
may conflict with Steam's terms of service - see the [Terms of Use](TERMS.md). AxxistSteam does not
bypass VAC or any anti-cheat and never attaches to game processes.

## Contact

Questions about privacy? Open an issue or ask on [Discord](https://discord.gg/k3fara4U64).
