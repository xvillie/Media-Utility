#include "yt_channel.h"
#include "util/process.h"
#include "util/fs_utils.h"
#include "util/http.h"
#include "util/console.h"
#include <filesystem>
#include <sstream>
#include <algorithm>

namespace handlers {

static void rename_prefix(const std::string& folder,
                          const std::string& prefix,
                          const std::string& target) {
    std::error_code ec;
    for (auto& e : std::filesystem::directory_iterator(folder, ec)) {
        auto fn = e.path().filename().string();
        std::string lo = fn;
        std::transform(lo.begin(), lo.end(), lo.begin(),
                       [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
        if (lo.rfind(prefix, 0) == 0 && lo != target) {
            fs_utils::rename_file(e.path().string(),
                                  fs_utils::join(folder, target));
            return;
        }
    }
}

static std::string find_largest_banner(const std::string& listing) {
    std::istringstream ss(listing);
    std::string line, best_url;
    long long best_area = -1;

    while (std::getline(ss, line)) {
        std::string lo = line;
        std::transform(lo.begin(), lo.end(), lo.begin(),
                       [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
        if (lo.find("banner") == std::string::npos) continue;

        auto p = line.find("http");
        if (p == std::string::npos) continue;
        std::string u = line.substr(p);
        auto end = u.find_first_of(" \t\r\n");
        if (end != std::string::npos) u.resize(end);

        long long w = 0, h = 0;
        std::istringstream ls(line);
        std::string tok;
        while (ls >> tok) {
            auto x = tok.find('x');
            if (x == std::string::npos) continue;
            try {
                w = std::stoll(tok.substr(0, x));
                h = std::stoll(tok.substr(x + 1));
                break;
            } catch (...) {}
        }
        long long area = (w > 0 && h > 0) ? w * h : 1;
        if (area > best_area) { best_area = area; best_url = u; }
    }
    return best_url;
}

int download_yt_channel(const std::string& url, const std::string& folder) {
    {
        console::spinner sp;
        sp.start("fetching avatar...");
        std::string cmd =
            "yt-dlp --write-thumbnail --skip-download --convert-thumbnails jpg "
            "-q --no-warnings -o \"" + folder + "\\avatar.%(ext)s\" \"" + url + "\"";
        int rc = process::run_silent(cmd);
        if (rc == 0) {
            rename_prefix(folder, "avatar.", "avatar.jpg");
            sp.stop_ok("avatar saved");
        } else {
            sp.stop_err("avatar failed");
        }
    }

    {
        console::spinner sp;
        sp.start("fetching banner...");
        std::string listing;
        int rc = process::run_capture(
            "yt-dlp --list-thumbnails --skip-download --no-warnings \"" + url + "\"",
            listing);
        if (rc != 0) {
            sp.stop_err("banner listing failed");
        } else {
            std::string burl = find_largest_banner(listing);
            if (burl.empty()) {
                sp.stop_err("no banner available");
            } else if (!http::download_to_file(burl, fs_utils::join(folder, "banner.jpg"))) {
                sp.stop_err("banner download failed");
            } else {
                sp.stop_ok("banner saved");
            }
        }
    }

    {
        console::spinner sp;
        sp.start("fetching channel info...");
        std::string desc;
        int rc = process::run_capture(
            "yt-dlp --skip-download --no-warnings -q --print description \"" + url + "\"",
            desc);
        if (rc == 0 && fs_utils::write_text(fs_utils::join(folder, "info.txt"), desc)) {
            sp.stop_ok("info saved");
        } else {
            sp.stop_err("info skipped");
        }
    }
    return 0;
}

}
