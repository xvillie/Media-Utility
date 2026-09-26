#include "console.h"
#include <windows.h>
#include <iostream>
#include <chrono>

namespace console {

static constexpr const char* c_rst  = "\x1b[0m";
static constexpr const char* c_dim  = "\x1b[38;5;240m";
static constexpr const char* c_soft = "\x1b[38;5;248m";
static constexpr const char* c_cy   = "\x1b[38;5;81m";
static constexpr const char* c_mg   = "\x1b[38;5;177m";
static constexpr const char* c_gr   = "\x1b[38;5;114m";
static constexpr const char* c_rd   = "\x1b[38;5;203m";
static constexpr const char* c_yl   = "\x1b[38;5;221m";
static constexpr const char* c_wt   = "\x1b[97m";
static constexpr const char* c_bold = "\x1b[1m";

void enable_vt() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD m = 0;
    if (h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &m))
        SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void clear_screen() {
    std::cout << "\x1b[2J\x1b[H";
}

void print_banner() {
    std::cout
        << "\n"
        << "  " << c_bold << c_cy << "media utility" << c_rst
        << c_soft << "  ·  tiktok  ·  youtube  ·  instagram" << c_rst << "\n"
        << "  " << c_dim  << "────────────────────────────────────────────" << c_rst << "\n\n";
}

void section(const std::string& title) {
    std::cout << "\n  " << c_bold << c_mg << title << c_rst << "\n"
              << "  " << c_dim << std::string(title.size(), '-') << c_rst << "\n\n";
}

int menu(const std::string& title, const std::vector<std::string>& options) {
    section(title);
    for (size_t i = 0; i < options.size(); i++) {
        std::cout << "     " << c_mg << i << c_rst << c_soft << " · " << c_rst
                  << options[i] << "\n";
    }
    std::cout << "\n";
    while (true) {
        std::cout << "  " << c_cy << "›" << c_rst << " select: ";
        std::string line;
        std::getline(std::cin, line);
        try {
            int c = std::stoi(line);
            if (c >= 0 && c < static_cast<int>(options.size())) return c;
        } catch (...) {}
    }
}

std::string prompt(const std::string& label) {
    std::cout << "  " << c_cy << "›" << c_rst << " " << label << c_soft << ": " << c_rst;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

bool yes_no(const std::string& label) {
    std::cout << "  " << c_cy << "›" << c_rst << " " << label
              << c_soft << " [y/N]: " << c_rst;
    std::string line;
    std::getline(std::cin, line);
    return !line.empty() && (line[0] == 'y' || line[0] == 'Y');
}

void ok(const std::string& m)   { std::cout << "  " << c_gr  << "✓ " << c_rst << m << "\n"; }
void err(const std::string& m)  { std::cout << "  " << c_rd  << "✗ " << c_rst << m << "\n"; }
void warn(const std::string& m) { std::cout << "  " << c_yl  << "! " << c_rst << m << "\n"; }
void info(const std::string& m) { std::cout << "  " << c_cy  << "· " << c_rst << m << "\n"; }
void step(const std::string& m) { std::cout << "  " << c_dim << "»" << c_rst << " " << m << "\n"; }

void kv(const std::string& k, const std::string& v) {
    std::cout << "  " << c_soft << k << c_dim << "  ·  " << c_rst << v << "\n";
}

void pause_exit() {
    std::cout << "\n  " << c_dim << "press enter to exit..." << c_rst;
    std::string _;
    std::getline(std::cin, _);
}

spinner::~spinner() {
    if (running_.load()) {
        running_.store(false);
        if (thr_.joinable()) thr_.join();
    }
}

void spinner::run_loop_() {
    static const char* frames[] = {
        "⠋","⠙","⠹","⠸","⠼","⠴","⠦","⠧","⠇","⠏"
    };
    int i = 0;
    while (running_.load()) {
        std::cout << "\r  " << c_cy << frames[i % 10] << c_rst << " " << label_ << "   " << std::flush;
        i++;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

void spinner::start(const std::string& label) {
    label_ = label;
    running_.store(true);
    thr_ = std::thread(&spinner::run_loop_, this);
}

void spinner::stop_ok(const std::string& done) {
    running_.store(false);
    if (thr_.joinable()) thr_.join();
    std::cout << "\r\x1b[2K  " << c_gr << "✓ " << c_rst << done << "\n" << std::flush;
}

void spinner::stop_err(const std::string& done) {
    running_.store(false);
    if (thr_.joinable()) thr_.join();
    std::cout << "\r\x1b[2K  " << c_rd << "✗ " << c_rst << done << "\n" << std::flush;
}

}
