#include <cstdio>
#include <android/log.h>

int main() {
    __android_log_print(ANDROID_LOG_INFO, "Auditor", "Hello from C++17 Android (arm64)");
    std::printf("Hello from C++17 Android (arm64)\n");
    return 0;
}
