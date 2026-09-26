#include "util/console.h"
#include "util/fs_utils.h"
#include "util/deps.h"
#include "core/detector.h"
#include "core/dispatcher.h"
#include "core/layout.h"
#include "core/menu.h"

#include <filesystem>
#include <iostream>

int main() {
    console::enable_vt();
    console::clear_screen();
    console::print_banner();

    if (!deps::ensure_all()) {
        console::err("required tools are missing");
        console::info("install manually:");
        console::info("  winget install yt-dlp.yt-dlp");
        console::info("  winget install Gyan.FFmpeg");
        console::pause_exit();
        return 1;
    }

    std::string root = std::filesystem::current_path().string();
    layout::init(root);
    console::kv("library", root);

    while (true) {
        core::url_type t = menu::pick();
        if (t == core::url_type::unknown) break;

        console::section(core::type_name(t));

        std::string url = console::prompt("paste url");
        if (url.empty()) {
            console::warn("no url — skipped");
            continue;
        }

        std::string raw  = console::prompt("save name");
        std::string name = fs_utils::sanitize_name(raw);

        std::string folder = layout::target_folder(t, root, name);
        if (!fs_utils::create_folder(folder)) {
            console::err("could not create folder: " + folder);
            continue;
        }
        console::kv("saving to", folder);
        std::cout << "\n";

        int rc = core::dispatch(t, url, folder, name);

        std::cout << "\n";
        auto files = fs_utils::list_folder(folder);
        if (!files.empty()) {
            console::info("files:");
            for (auto& f : files) std::cout << "      · " << f << "\n";
        }
        if (rc != 0) console::warn("finished with warnings");
        std::cout << "\n";

        if (!console::yes_no("download another")) break;
        console::clear_screen();
        console::print_banner();
        console::kv("library", root);
    }

    console::pause_exit();
    return 0;
}
