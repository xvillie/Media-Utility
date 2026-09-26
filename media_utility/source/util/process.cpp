#include "process.h"
#include <windows.h>
#include <vector>

namespace process {

int run_inherit(const std::string& command_line) {
    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
    PROCESS_INFORMATION pi{};

    std::vector<char> cmd(command_line.begin(), command_line.end());
    cmd.push_back(0);

    BOOL ok = CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, TRUE, 0,
                             nullptr, nullptr, &si, &pi);
    if (!ok) return -1;

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return static_cast<int>(code);
}

int run_capture(const std::string& command_line, std::string& out) {
    out.clear();

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    HANDLE read_h = nullptr, write_h = nullptr;
    if (!CreatePipe(&read_h, &write_h, &sa, 0)) return -1;
    SetHandleInformation(read_h, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = write_h;
    si.hStdError  = write_h;
    PROCESS_INFORMATION pi{};

    std::vector<char> cmd(command_line.begin(), command_line.end());
    cmd.push_back(0);

    BOOL ok = CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, TRUE,
                             CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
    CloseHandle(write_h);
    if (!ok) {
        CloseHandle(read_h);
        return -1;
    }

    char buf[4096];
    DWORD n = 0;
    while (ReadFile(read_h, buf, sizeof(buf), &n, nullptr) && n > 0) {
        out.append(buf, buf + n);
    }
    CloseHandle(read_h);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return static_cast<int>(code);
}

int run_silent(const std::string& command_line) {
    std::string sink;
    return run_capture(command_line, sink);
}

}
