#include "dispatcher.h"
#include "handlers/video.h"
#include "handlers/yt_channel.h"
#include "handlers/ig_profile.h"
#include "handlers/tiktok_profile.h"
#include "handlers/tiktok_slideshow.h"
#include "util/console.h"

namespace core {

int dispatch(url_type type, const std::string& url,
             const std::string& folder, const std::string& name) {
    switch (type) {
        case url_type::tiktok_video:
        case url_type::youtube_video:
        case url_type::youtube_short:
        case url_type::instagram_post:
        case url_type::instagram_reel:
            return handlers::download_video(url, folder, name, "captions.txt");
        case url_type::instagram_story:
        case url_type::tiktok_story:
            return handlers::download_video(url, folder, name, "info.txt");
        case url_type::tiktok_slideshow:
            return handlers::download_tiktok_slideshow(url, folder, name);
        case url_type::youtube_channel:
            return handlers::download_yt_channel(url, folder);
        case url_type::tiktok_profile:
            return handlers::download_tiktok_profile(url, folder);
        case url_type::instagram_profile:
            return handlers::download_ig_profile(url, folder);
        default:
            console::err("unknown URL type");
            return 1;
    }
}

}
