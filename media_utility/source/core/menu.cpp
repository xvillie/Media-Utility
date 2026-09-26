#include "menu.h"
#include "util/console.h"

namespace menu {

using core::url_type;

static url_type tiktok_menu() {
    int c = console::menu("tiktok", {
        "← back",
        "video",
        "slideshow",
        "profile (avatar + bio)",
        "story",
    });
    switch (c) {
        case 1: return url_type::tiktok_video;
        case 2: return url_type::tiktok_slideshow;
        case 3: return url_type::tiktok_profile;
        case 4: return url_type::tiktok_story;
        default: return url_type::unknown;
    }
}

static url_type youtube_menu() {
    int c = console::menu("youtube", {
        "← back",
        "video",
        "short",
        "channel (avatar + banner + info)",
    });
    switch (c) {
        case 1: return url_type::youtube_video;
        case 2: return url_type::youtube_short;
        case 3: return url_type::youtube_channel;
        default: return url_type::unknown;
    }
}

static url_type instagram_menu() {
    int c = console::menu("instagram", {
        "← back",
        "post",
        "reel",
        "story",
        "profile (avatar)",
    });
    switch (c) {
        case 1: return url_type::instagram_post;
        case 2: return url_type::instagram_reel;
        case 3: return url_type::instagram_story;
        case 4: return url_type::instagram_profile;
        default: return url_type::unknown;
    }
}

url_type pick() {
    while (true) {
        int c = console::menu("what to download", {
            "exit",
            "tiktok",
            "youtube",
            "instagram",
        });
        url_type t = url_type::unknown;
        switch (c) {
            case 1: t = tiktok_menu();    break;
            case 2: t = youtube_menu();   break;
            case 3: t = instagram_menu(); break;
            default: return url_type::unknown;
        }
        if (t != url_type::unknown) return t;
    }
}

}
