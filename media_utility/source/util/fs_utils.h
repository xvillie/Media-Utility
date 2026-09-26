#pragma once
#include <string>
#include <vector>

namespace fs_utils {

std::string sanitize_name(const std::string& raw);
bool create_folder(const std::string& path);
std::vector<std::string> list_folder(const std::string& path);
bool write_text(const std::string& path, const std::string& content);
bool rename_file(const std::string& from, const std::string& to);
bool file_exists(const std::string& path);
std::string join(const std::string& a, const std::string& b);

}
