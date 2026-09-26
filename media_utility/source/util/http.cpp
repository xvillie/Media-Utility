#include "http.h"
#include <windows.h>
#include <urlmon.h>

namespace http {

bool download_to_file(const std::string& url, const std::string& out_path) {
    HRESULT hr = URLDownloadToFileA(nullptr, url.c_str(), out_path.c_str(), 0, nullptr);
    return SUCCEEDED(hr);
}

}
