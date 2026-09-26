#pragma once
#include <string>

namespace http {

bool download_to_file(const std::string& url, const std::string& out_path);

}
