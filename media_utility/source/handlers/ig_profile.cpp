#include "ig_profile.h"
#include "util/process.h"
#include "util/fs_utils.h"
#include "util/console.h"
#include <filesystem>
#include <algorithm>

namespace handlers {

int download_ig_profile(const std::string& url, const std::string& folder) {
    console::spinner sp;
    sp.start("fetching profile picture...");
    std::string cmd =
        "yt-dlp --write-thumbnail --skip-download --convert-thumbnails jpg "
        "-q --no-warnings -o \"" + folder + "\\pfp.%(ext)s\" \"" + url + "\"";
    int rc = process::run_silent(cmd);
    if (rc != 0) {
        sp.stop_err("profile fetch failed");
        console::warn("instagram profiles usually need auth");
        console::warn("try: --cookies-from-browser chrome  |  --cookies cookies.txt");
        console::warn("or fallback: gallery-dl --range 1 " + url);
        return rc;
    }

    std::error_code ec;
    for (auto& e : std::filesystem::directory_iterator(folder, ec)) {
        auto fn = e.path().filename().string();
        std::string lo = fn;
        std::transform(lo.begin(), lo.end(), lo.begin(),
                       [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
        if (lo.rfind("pfp.", 0) == 0 && lo != "pfp.jpg") {
            fs_utils::rename_file(e.path().string(),
                                  fs_utils::join(folder, "pfp.jpg"));
            break;
        }
    }
    sp.stop_ok("profile picture saved");
    return 0;
}

}
