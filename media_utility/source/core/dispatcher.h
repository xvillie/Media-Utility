#pragma once
#include <string>
#include "detector.h"

namespace core {

int dispatch(url_type type, const std::string& url,
             const std::string& folder, const std::string& name);

}
