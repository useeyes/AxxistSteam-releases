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
verifies the signature against a baked-in public key before installing an update - a tampered
artifact is rejected automatically.

## Credential handling

- Steam session tokens and the Steam Web API key are stored in the **OS keychain**
  (Windows Credential Manager / libsecret on Linux), never in plain text.
- AxxistSteam never transmits your credentials anywhere except Steam's own sign-in endpoints.
