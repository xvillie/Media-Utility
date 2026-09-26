#include "tiktok_slideshow.h"
#include "util/process.h"
#include "util/fs_utils.h"
#include "util/console.h"

namespace handlers {

int download_tiktok_slideshow(const std::string& url,
                              const std::string& folder,
                              const std::string& name) {
    {
        console::spinner sp;
        sp.start("downloading slideshow...");
        std::string cmd =
            "yt-dlp --no-playlist --restrict-filenames -q --no-warnings "
            "-o \"" + folder + "\\" + name + "_%(autonumber)02d.%(ext)s\" \"" + url + "\"";
        int rc = process::run_silent(cmd);
        if (rc != 0) {
            sp.stop_err("slideshow failed");
            return rc;
        }
        sp.stop_ok("slides saved");
    }

    {
        console::spinner sp;
        sp.start("fetching captions...");
        std::string desc;
        int rc = process::run_capture(
            "yt-dlp --skip-download -q --no-warnings --print description \"" + url + "\"",
            desc);
        if (rc == 0 && fs_utils::write_text(fs_utils::join(folder, "captions.txt"), desc)) {
            sp.stop_ok("captions saved");
        } else {
            sp.stop_err("captions skipped");
        }
    }
    return 0;
}

}
