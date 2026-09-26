#include "deps.h"
#include "process.h"
#include "console.h"
#include <string>

namespace deps {

static bool tool_present(const char* probe) {
    std::string sink;
    return process::run_capture(probe, sink) == 0 && !sink.empty();
}

bool have_ytdlp()  { return tool_present("yt-dlp --version"); }
bool have_ffmpeg() { return tool_present("ffmpeg -version"); }

static bool winget_install(const std::string& id) {
    std::string cmd =
        "winget install --id " + id +
        " -e --accept-source-agreements --accept-package-agreements --silent";
    return process::run_silent(cmd) == 0;
}

static bool pip_install(const std::string& pkg) {
    std::string cmd = "python -m pip install --upgrade --quiet " + pkg;
    return process::run_silent(cmd) == 0;
}

bool ensure_all() {
    bool need_y = !have_ytdlp();
    bool need_f = !have_ffmpeg();
    if (!need_y && !need_f) return true;

    console::spinner sp;
    sp.start("preparing environment...");

    bool ok = true;
    if (need_y) {
        bool got = winget_install("yt-dlp.yt-dlp");
        if (!got) got = pip_install("yt-dlp");
        if (!got) ok = false;
    }
    if (need_f) {
        bool got = winget_install("Gyan.FFmpeg");
        if (!got) ok = false;
    }

    if (ok && have_ytdlp()) {
        sp.stop_ok("environment ready");
        return true;
    }

    sp.stop_err("could not prepare environment automatically");
    return have_ytdlp();
}

}
