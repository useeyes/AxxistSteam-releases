# Security Policy

## Supported Versions

Only the latest published release receives security fixes. Always run the newest version - the
built-in updater will notify you when one is available.

| Version | Supported |
|---------|-----------|
| latest  | ✅ |
| older   | ❌ |

## Reporting a Vulnerability

**Please do not report security vulnerabilities through public GitHub issues.**

Instead, report them privately:

- **Discord:** direct message a maintainer on the [Axxist Discord](https://discord.gg/k3fara4U64)
- **GitHub:** use [private vulnerability reporting](../../security/advisories/new) if enabled

Please include:

- A description of the vulnerability and its impact
- Steps to reproduce / proof of concept
- Affected version(s)
- Your environment (OS, install type)

You can expect an initial response within **72 hours**. If the report is confirmed, we will:

1. Develop and verify a fix
2. Publish a new signed release
3. Credit you in the release notes (unless you prefer to stay anonymous)

## Scope

In scope:

- The AxxistSteam desktop application (all released installers)
- The update mechanism (manifest, signatures, download path)
- The bundled SteamUtility helper

Out of scope:

- Vulnerabilities in Steam/Valve services themselves
- Issues requiring physical access to an unlocked machine
- Social engineering

## Update artifact verification

Every updater artifact is signed with [minisign](https://jedisct1.github.io/minisign/). The app
verifies the signature against a baked-in public key before installing an update - a tampered or
unsigned artifact is rejected automatically. Note that this verifies the update channel, not the
initial download itself: get installers only from this repository's
[Releases](../../releases) page.

## Credential handling

- Steam session tokens, Steam Community cookies, and your own Steam Web API key (if you set one)
  are stored in the **OS keychain** (Windows Credential Manager / a running Secret Service such as
  libsecret on Linux), never in plain-text files.
- AxxistSteam never transmits your credentials anywhere except Steam's own sign-in endpoints.
- Passwords entered at sign-in are held in memory only for the duration of the login handshake and
  are never written to disk; what persists is Steam's own refresh token (see above).

## Source access / code checkup

AxxistSteam's source code is private (see [LICENSE](LICENSE)). No automated source-scanning or
audit infrastructure exists, and we won't pretend otherwise. If you are a developer and want to
look before you trust: **we offer a manual code checkup** - ask on the official
[AxxistSteam Discord](https://discord.gg/k3fara4U64) and, when arranged with a maintainer, you get
temporary access to inspect the current version's source and ask questions about it.

## A note on absolute claims

No software is "100% secure", and this app is no exception. What we can point at concretely is the
behavior documented above and in the [Privacy Policy](docs/PRIVACY.md): credentials in the OS
keychain, no telemetry, signed updates, and a network surface limited to Steam, GitHub, and a
static asset CDN. If you find a gap between this document and the real behavior, that is a
vulnerability report - please tell us (see above).
