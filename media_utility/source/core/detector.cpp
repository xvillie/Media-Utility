#include "detector.h"
#include <algorithm>

namespace core {

static bool has(const std::string& hay, const char* n) {
    return hay.find(n) != std::string::npos;
}

const char* type_name(url_type t) {
    switch (t) {
        case url_type::tiktok_video:      return "tiktok · video";
        case url_type::tiktok_slideshow:  return "tiktok · slideshow";
        case url_type::tiktok_profile:    return "tiktok · profile";
        case url_type::tiktok_story:      return "tiktok · story";
        case url_type::youtube_video:     return "youtube · video";
        case url_type::youtube_short:     return "youtube · short";
        case url_type::youtube_channel:   return "youtube · channel";
        case url_type::instagram_post:    return "instagram · post";
        case url_type::instagram_reel:    return "instagram · reel";
        case url_type::instagram_story:   return "instagram · story";
        case url_type::instagram_profile: return "instagram · profile";
        default:                          return "unknown";
    }
}

url_type detect(const std::string& url) {
    std::string u = url;
    std::transform(u.begin(), u.end(), u.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });

    if (has(u, "tiktok.com")) {
        if (has(u, "/photo/")) return url_type::tiktok_slideshow;
        if (has(u, "/story/")) return url_type::tiktok_story;
        if (has(u, "/video/")) return url_type::tiktok_video;
        return url_type::tiktok_profile;
    }

    if (has(u, "youtube.com/shorts/")) return url_type::youtube_short;
    if (has(u, "youtube.com/watch?v=") || has(u, "youtu.be/"))
        return url_type::youtube_video;
    if (has(u, "youtube.com/@") || has(u, "youtube.com/channel/uc")
        || has(u, "youtube.com/c/") || has(u, "youtube.com/user/"))
        return url_type::youtube_channel;

    if (has(u, "instagram.com/stories/")) return url_type::instagram_story;
    if (has(u, "instagram.com/reel/")) return url_type::instagram_reel;
    if (has(u, "instagram.com/p/") || has(u, "instagram.com/tv/"))
        return url_type::instagram_post;
    if (has(u, "instagram.com/")) return url_type::instagram_profile;

    return url_type::unknown;
}

}
