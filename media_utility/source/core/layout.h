#pragma once
#include <string>
#include "core/detector.h"

namespace layout {

void init(const std::string& root);
std::string target_folder(core::url_type type,
                          const std::string& root,
                          const std::string& name);

}
