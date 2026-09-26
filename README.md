# Media Utility

A small, no-nonsense Windows console tool for pulling media from **TikTok**, **YouTube**, and **Instagram** into an organized local library.

Menu-driven. Silent progress. Auto-installs its own tooling on first run.

---

## Features

- **Organized library.** Everything lands in a fixed folder tree in your working directory:
  ```
  tiktok/
    videos/  slideshows/  profiles/  stories/
  youtube/
    videos/  shorts/  profiles/
  instagram/
    posts/  reels/  stories/  profiles/
  ```
- **Menu-driven.** Pick site → pick content type → paste URL → done.
- **Silent progress.** No noisy console spam — just a spinner and a tick per step.
- **Auto-setup.** Missing tools are installed in the background on first launch via `winget` (falls back to `pip`).
- **Per-download folder.** Each save creates its own subfolder inside the right category.

## Supported content

| Site       | What you can grab                                     |
|------------|-------------------------------------------------------|
| TikTok     | Video · Slideshow · Story · Profile (avatar + bio)    |
| YouTube    | Video · Short · Channel (avatar + banner + info)      |
| Instagram  | Post · Reel · Story · Profile (avatar)                |

## Requirements

- Windows 10 / 11 (x64)
- Visual Studio 2022 (v143 toolset), C++17
- Internet connection

Runtime deps (auto-installed on first run):

- [yt-dlp](https://github.com/yt-dlp/yt-dlp)
- [ffmpeg](https://ffmpeg.org/)

## Build

1. Open [`media_utility.sln`](media_utility.sln) in Visual Studio 2022.
2. Select **Release · x64**.
3. Build → the binary lands at `x64\Release\media_utility.exe`.

Or from a Developer Command Prompt:

```bat
msbuild media_utility.sln /p:Configuration=Release /p:Platform=x64
```

## Usage

Run `media_utility.exe` from any folder — that folder becomes the library root.

```
media utility  ·  tiktok  ·  youtube  ·  instagram
────────────────────────────────────────────

  library  ·  D:\downloads

  what to download
  ----------------

     0 · exit
     1 · tiktok
     2 · youtube
     3 · instagram

  › select:
```

Files land in the right subfolder automatically. Save name is sanitized for Windows.

## Instagram authentication

Instagram content routinely requires a signed-in session. If a fetch fails, use one of:

```
--cookies-from-browser chrome
--cookies cookies.txt
```

(Configure yt-dlp with these flags via your own wrapper if you need auth — the tool prints the hint when a fetch fails.)

## Project layout

```
media_utility/
  media_utility.vcxproj
  source/
    main.cpp
    core/     detector · dispatcher · layout · menu
    handlers/ video · yt_channel · ig_profile · tiktok_profile · tiktok_slideshow
    util/     process · fs_utils · http · console · deps
```

## License

[MIT](LICENSE) © 2026 xvillie
