#include "fs_utils.h"
#include <windows.h>
#include <fstream>
#include <filesystem>

namespace fs_utils {

std::string sanitize_name(const std::string& raw) {
    std::string out;
    out.reserve(raw.size());
    for (unsigned char c : raw) {
        if (c < 0x20) continue;
        switch (c) {
            case '<': case '>': case ':': case '"':
            case '/': case '\\': case '|': case '?': case '*':
                continue;
            default:
                out.push_back(static_cast<char>(c));
        }
    }
    while (!out.empty() && (out.back() == '.' || out.back() == ' ')) out.pop_back();
    while (!out.empty() && out.front() == ' ') out.erase(out.begin());
    if (out.empty()) out = "download";
    return out;
}

bool create_folder(const std::string& path) {
    std::error_code ec;
    std::filesystem::create_directories(path, ec);
    return !ec;
}

std::vector<std::string> list_folder(const std::string& path) {
    std::vector<std::string> files;
    std::error_code ec;
    for (auto& e : std::filesystem::directory_iterator(path, ec)) {
        if (e.is_regular_file(ec)) files.push_back(e.path().filename().string());
    }
    return files;
}

bool write_text(const std::string& path, const std::string& content) {
    std::ofstream f(path, std::ios::binary);
    if (!f) return false;
    f.write(content.data(), static_cast<std::streamsize>(content.size()));
    return f.good();
}

bool rename_file(const std::string& from, const std::string& to) {
    std::error_code ec;
    std::filesystem::rename(from, to, ec);
    return !ec;
}

bool file_exists(const std::string& path) {
    std::error_code ec;
    return std::filesystem::exists(path, ec);
}

std::string join(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    char last = a.back();
    if (last == '\\' || last == '/') return a + b;
    return a + "\\" + b;
}

}
