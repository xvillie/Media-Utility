#include "layout.h"
#include "util/fs_utils.h"

namespace layout {

void init(const std::string& root) {
    const char* dirs[] = {
        "tiktok",
        "tiktok\\videos",
        "tiktok\\slideshows",
        "tiktok\\profiles",
        "tiktok\\stories",
        "youtube",
        "youtube\\videos",
        "youtube\\shorts",
        "youtube\\profiles",
        "instagram",
        "instagram\\posts",
        "instagram\\reels",
        "instagram\\stories",
        "instagram\\profiles",
    };
    for (auto d : dirs) fs_utils::create_folder(fs_utils::join(root, d));
}

std::string target_folder(core::url_type t,
                          const std::string& root,
                          const std::string& name) {
    std::string sub;
    switch (t) {
        case core::url_type::tiktok_video:      sub = "tiktok\\videos";      break;
        case core::url_type::tiktok_slideshow:  sub = "tiktok\\slideshows";  break;
        case core::url_type::tiktok_profile:    sub = "tiktok\\profiles";    break;
        case core::url_type::tiktok_story:      sub = "tiktok\\stories";     break;
        case core::url_type::youtube_video:     sub = "youtube\\videos";     break;
        case core::url_type::youtube_short:     sub = "youtube\\shorts";     break;
        case core::url_type::youtube_channel:   sub = "youtube\\profiles";   break;
        case core::url_type::instagram_post:    sub = "instagram\\posts";    break;
        case core::url_type::instagram_reel:    sub = "instagram\\reels";    break;
        case core::url_type::instagram_story:   sub = "instagram\\stories";  break;
        case core::url_type::instagram_profile: sub = "instagram\\profiles"; break;
        default: sub = ".";
    }
    return fs_utils::join(fs_utils::join(root, sub), name);
}

}
