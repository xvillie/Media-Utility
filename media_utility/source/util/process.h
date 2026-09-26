#pragma once
#include <string>

namespace process {

int run_inherit(const std::string& command_line);
int run_capture(const std::string& command_line, std::string& out);
int run_silent(const std::string& command_line);

}
