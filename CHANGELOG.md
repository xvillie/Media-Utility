# Changelog

All notable changes to **Media Utility** are documented here.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and this project adheres to [Semantic Versioning](https://semver.org/).

---

## [1.0.0] — 2026-09-26 · *First Frame*

Initial public release.

### Added
- **Menu-driven UI.** Two-level menu (site → content type), clean 256-color palette, single-line spinner for every long step.
- **Organized library.** On launch, the working directory is populated with a fixed tree:
  - `tiktok/{videos, slideshows, profiles, stories}`
  - `youtube/{videos, shorts, profiles}`
  - `instagram/{posts, reels, stories, profiles}`
  - Every download creates its own subfolder in the correct category.
- **Auto-setup of runtime dependencies.** On first launch, missing tooling is silently installed via `winget` (with `pip` fallback) behind a `preparing environment...` spinner.
- **TikTok support:** video, slideshow (multi-image with `<name>_NN.jpg`), profile (avatar + bio), story.
- **YouTube support:** standard video, short, channel (avatar + largest available banner + description).
- **Instagram support:** post, reel, story, profile (avatar).
- **Save-name sanitization** for Windows-reserved characters (`<>:"/\|?*`, control chars, trailing dots/spaces).
- **Post-download summary.** Files that landed in the target folder are listed after each run.
- **Interactive session loop.** After each item, prompt to download another; clean redraw between runs.
- **MIT License, README, and this changelog.**

### Technical
- Single Visual Studio 2022 solution: `media_utility.sln` + `media_utility.vcxproj`.
- MSVC v143 toolset, C++17, x64.
- Links `urlmon.lib` (for direct HTTP downloads of YouTube channel banners via `URLDownloadToFile`) and `shlwapi.lib`.
- Full UTF-8 console output (`SetConsoleOutputCP(CP_UTF8)`, VT sequences enabled).
- Subprocess helpers: `run_inherit`, `run_capture`, `run_silent` — every long-running child is silenced and paired with a spinner.
- Codebase split across `core/` (detector, dispatcher, layout, menu), `handlers/` (per content-type), `util/` (process, fs_utils, http, console, deps).

### Known limitations
- Instagram content commonly requires an authenticated session. Failures print a hint to supply cookies via `--cookies-from-browser chrome` or `--cookies cookies.txt`.
- TikTok "story" URLs are only sometimes available publicly; unsupported entries surface as a clean failure.
- Duplicate save-names in the same category will overwrite the previous folder's files.

[1.0.0]: https://github.com/xvillie/media-utility/releases/tag/v1.0.0
