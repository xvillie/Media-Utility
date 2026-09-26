#include "tiktok_profile.h"
#include "util/process.h"
#include "util/fs_utils.h"
#include "util/console.h"
#include <filesystem>
#include <algorithm>

namespace handlers {

int download_tiktok_profile(const std::string& url, const std::string& folder) {
    {
        console::spinner sp;
        sp.start("fetching avatar...");
        std::string cmd =
            "yt-dlp --write-thumbnail --skip-download --playlist-items 1 "
            "--convert-thumbnails jpg -q --no-warnings "
            "-o \"" + folder + "\\avatar.%(ext)s\" \"" + url + "\"";
        int rc = process::run_silent(cmd);
        if (rc != 0) {
            sp.stop_err("avatar failed");
        } else {
            std::error_code ec;
            for (auto& e : std::filesystem::directory_iterator(folder, ec)) {
                auto fn = e.path().filename().string();
                std::string lo = fn;
                std::transform(lo.begin(), lo.end(), lo.begin(),
                               [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
                if (lo.rfind("avatar.", 0) == 0 && lo != "avatar.jpg") {
                    fs_utils::rename_file(e.path().string(),
                                          fs_utils::join(folder, "avatar.jpg"));
                    break;
                }
            }
            sp.stop_ok("avatar saved");
        }
    }

    {
        console::spinner sp;
        sp.start("fetching bio...");
        std::string desc;
        int rc = process::run_capture(
            "yt-dlp --skip-download --playlist-items 1 -q --no-warnings "
            "--print description \"" + url + "\"", desc);
        if (rc == 0 && fs_utils::write_text(fs_utils::join(folder, "info.txt"), desc)) {
            sp.stop_ok("bio saved");
        } else {
            sp.stop_err("bio skipped");
        }
    }
    return 0;
}

}
