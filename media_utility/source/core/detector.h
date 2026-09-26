#pragma once
#include <string>

namespace core {

enum class url_type {
    unknown,
    tiktok_video, tiktok_slideshow, tiktok_profile, tiktok_story,
    youtube_video, youtube_short, youtube_channel,
    instagram_post, instagram_reel, instagram_story, instagram_profile
};

const char* type_name(url_type t);
url_type detect(const std::string& url);

}
