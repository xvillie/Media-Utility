#pragma once
#include <string>

namespace handlers {

int download_video(const std::string& url,
                   const std::string& folder,
                   const std::string& name,
                   const std::string& desc_filename);

}
