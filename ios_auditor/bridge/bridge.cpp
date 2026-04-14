#include "bridge.h"
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>

static std::string run_cmd(const std::string &cmd) {
    std::string data;
    FILE *stream = popen(cmd.c_str(), "r");
    if (!stream) return std::string();
    const int bufsize = 256;
    char buffer[bufsize];
    while (fgets(buffer, bufsize, stream) != nullptr) {
        data += buffer;
    }
    pclose(stream);
    return data;
}

extern "C" void run_idevice_id(void) {
    auto out = run_cmd("idevice_id -l 2>/dev/null");
    if (out.empty()) std::puts("(no devices or idevice_id not installed)");
    else std::fputs(out.c_str(), stdout);
}

extern "C" void run_idevice_info(const char *udid) {
    if (!udid) { std::puts("udid is null"); return; }
    std::string cmd = std::string("ideviceinfo -u ") + udid + " 2>/dev/null";
    auto out = run_cmd(cmd);
    if (out.empty()) std::puts("(no info or ideviceinfo not installed)");
    else std::fputs(out.c_str(), stdout);
}
