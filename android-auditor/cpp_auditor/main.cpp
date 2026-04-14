#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

std::string run_cmd(const std::string &cmd) {
    std::string data;
    FILE *stream = popen(cmd.c_str(), "r");
    if (!stream) return "";
    const int bufsize = 256;
    char buffer[bufsize];
    while (fgets(buffer, bufsize, stream) != nullptr) {
        data += buffer;
    }
    pclose(stream);
    return data;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Usage: cpp_auditor [--devices] [--list-packages] [--third-party] [--permissions PKG] [--net]\n";
        return 1;
    }

    bool list_packages = false, third_party = false, devices = false, net = false;
    std::string pkg;

    std::unordered_map<std::string, std::function<void()>> handlers = {
        {"--list-packages", [&]{ list_packages = true; }},
        {"--third-party",   [&]{ third_party = true; }},
        {"--devices",       [&]{ devices = true; }},
        {"--net",           [&]{ net = true; }}
    };

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto it = handlers.find(a);
        if (it != handlers.end()) {
            it->second();
            continue;
        }
        if (a == "--permissions" && i + 1 < argc) {
            pkg = argv[++i];
            continue;
        }
        std::cerr << "Unknown arg: " << a << "\n";
    }

    if (devices) {
        std::cout << run_cmd("adb devices -l");
    }
    if (list_packages) {
        if (third_party) std::cout << run_cmd("adb shell pm list packages -3");
        else std::cout << run_cmd("adb shell pm list packages");
    }
    if (!pkg.empty()) {
        std::cout << run_cmd(std::string("adb shell dumpsys package ") + pkg);
    }
    if (net) {
        auto out = run_cmd("adb shell ss -tunp 2>/dev/null");
        if (out.empty()) out = run_cmd("adb shell netstat -anp 2>/dev/null");
        if (out.empty()) std::cerr << "Could not obtain network connections (ss/netstat missing).\n";
        else std::cout << out;
    }

    return 0;
}
