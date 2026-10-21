# axxist-usvfs

Windows launcher for AxxistSteam's real VFS. It links [Mod Organizer 2's
USVFS](https://github.com/ModOrganizer2/usvfs) and is therefore **GPL-3.0** (see `LICENSE`), while
the rest of AxxistSteam stays Elastic-2.0. Keeping the launcher in its own process keeps the
licences separate: AxxistSteam only spawns `axxist-usvfs.exe` and hands it a plan.

## What it does

1. Creates a named USVFS instance (`--instance`).
2. Maps the merged profile mods over the real game folder (`--map`), without a create target.
3. Maps the profile's overwrite folder over the same root **with** `LINKFLAG_CREATETARGET`, so the
   game never writes into its install directory.
4. Starts the game through `usvfsCreateProcessHooked`, waits for it and returns its exit code.

The plan is produced by `src-tauri/src/vfs_usvfs.rs` and passed by the `axxist-vfs` helper.

## Build

Windows only (MSVC + CMake + 7-Zip). From the repository root:

```powershell
node scripts/vendor-usvfs.mjs
```

That script downloads the official, SHA-256-pinned USVFS release (headers, import libraries and
both proxy DLLs), then builds this launcher for **both** x64 and x86 (32-bit games are common in
older modding communities). It copies four files into `src-tauri/binaries/`, where the Tauri bundle
picks them up:

- `axxist-usvfs.exe` + `usvfs_x64.dll`
- `axxist-usvfs-x86.exe` + `usvfs_x86.dll`

The Rust helper reads the game's PE header and starts the launcher matching its architecture. CI
runs the same script on `windows-latest` and caches the result.

The launcher itself links the MSVC runtime statically, but the upstream proxy DLLs use the dynamic
runtime, so the machine needs the Microsoft Visual C++ 2015-2022 Redistributable (x64/x86). Steam
installs it for most games and AxxistSteam itself depends on it; every normal gaming PC has it.

## Source availability

This launcher is GPL-3.0. Its complete source (`main.cpp`, `CMakeLists.txt`, `README.md` and
`LICENSE`) is published at <https://github.com/useeyes/AxxistSteam-releases/tree/main/usvfs/axxist-usvfs>
and is attached to the corresponding release as a source archive.

USVFS itself is GPL-3.0; its complete source is available from
<https://github.com/ModOrganizer2/usvfs> at the pinned tag `v0.5.7.2` (the tag used by
`scripts/vendor-usvfs.mjs`). The release also ships the matching upstream source archive.
