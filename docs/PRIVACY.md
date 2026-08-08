# Privacy Policy

**Last updated: 2026-08-08**

AxxistSteam is a desktop application that automates parts of your own Steam account on your own
computer. This policy explains what data is processed, where it goes, and what never leaves your
machine.

## The short version

- Your Steam credentials and session tokens **never leave your computer**, except to Steam's own
  servers as part of signing in.
- Your Steam Web API key is stored in your **operating system's keychain** (Windows Credential
  Manager / libsecret), not in plain text files.
- AxxistSteam has **no analytics, no tracking, no telemetry**.
- The only third-party connections are: Steam (Valve), SteamDB/Steam store pages (public data),
  and GitHub (update checks & release downloads).

## What is stored locally

| Data | Where | Why |
|------|-------|-----|
| Steam session tokens | App data directory / OS keychain | Keep you signed in between restarts |
| Steam Web API key | OS keychain | Fetch your profile data (level, avatar, playtime) |
| Settings & preferences | App data directory | App configuration |
| Owned-games cache | App data directory | Avoids re-fetching your library on every launch |
| Per-account activity log | Local storage | The dashboard's activity view |
| Custom account names | Local storage | In-app display names you set |

## What leaves your computer

| Destination | What | Why |
|-------------|------|-----|
| `steampowered.com` / `steamcommunity.com` | Login credentials, session cookies, API calls | Required for every feature (idling, cards, achievements, free games) |
| `store.steampowered.com` | Anonymous page requests | Discovering free games/packages |
| `github.com` / `raw.githubusercontent.com` | Anonymous GET requests | Update checks, release history, notification feed |

No data is sent to any server operated by AxxistSteam - there is none.

## Your responsibility

Automating a Steam account (idling, achievement unlocking, card farming) is at your own risk and
may conflict with Steam's terms of service. AxxistSteam does not bypass VAC or any anti-cheat and
never touches game processes.

## Contact

Questions about privacy? Open an issue or ask on [Discord](https://discord.gg/k3fara4U64).
