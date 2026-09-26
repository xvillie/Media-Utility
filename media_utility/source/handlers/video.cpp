#include "video.h"
#include "util/process.h"
#include "util/fs_utils.h"
#include "util/console.h"

namespace handlers {

static bool is_instagram(const std::string& url) {
    return url.find("instagram.com") != std::string::npos;
}

int download_video(const std::string& url,
                   const std::string& folder,
                   const std::string& name,
                   const std::string& desc_filename) {
    std::string dl_cmd =
        "yt-dlp --no-playlist --restrict-filenames --no-warnings -q "
        "-f \"bv*+ba/b\" --merge-output-format mp4 "
        "-o \"" + folder + "\\" + name + ".%(ext)s\" \"" + url + "\"";

    {
        console::spinner sp;
        sp.start("downloading video...");
        int rc = process::run_silent(dl_cmd);
        if (rc != 0) {
            sp.stop_err("download failed");
            if (is_instagram(url)) {
                console::warn("instagram content usually requires auth");
                console::warn("try: --cookies-from-browser chrome  |  --cookies cookies.txt");
            }
            return rc;
        }
        sp.stop_ok("video saved");
    }

    {
        console::spinner sp;
        sp.start("fetching description...");
        std::string desc_cmd =
            "yt-dlp --skip-download --no-warnings -q --print description \"" + url + "\"";
        std::string desc;
        int drc = process::run_capture(desc_cmd, desc);
        if (drc == 0 && fs_utils::write_text(fs_utils::join(folder, desc_filename), desc)) {
            sp.stop_ok(std::string("description saved (") + desc_filename + ")");
        } else {
            sp.stop_err("description skipped");
        }
    }
    return 0;
}

}
