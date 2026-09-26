#pragma once
#include <string>
#include <vector>
#include <thread>
#include <atomic>

namespace console {

void enable_vt();
void clear_screen();
void print_banner();
void section(const std::string& title);

int  menu(const std::string& title, const std::vector<std::string>& options);
std::string prompt(const std::string& label);
bool yes_no(const std::string& label);

void ok(const std::string& msg);
void err(const std::string& msg);
void warn(const std::string& msg);
void info(const std::string& msg);
void step(const std::string& msg);
void kv(const std::string& key, const std::string& value);

void pause_exit();

class spinner {
public:
    spinner() = default;
    ~spinner();
    void start(const std::string& label);
    void stop_ok(const std::string& done_label);
    void stop_err(const std::string& done_label);
private:
    std::atomic<bool> running_{false};
    std::thread thr_;
    std::string label_;
    void run_loop_();
};

}
