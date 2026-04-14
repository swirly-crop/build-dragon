# Build-Dragon

[![Android Build](https://github.com/swirly-crop/build-dragon/actions/workflows/android.yml/badge.svg)](https://github.com/swirly-crop/build-dragon/actions/workflows/android.yml) [![Assembly Run](https://github.com/swirly-crop/build-dragon/actions/workflows/assembly.yml/badge.svg)](https://github.com/swirly-crop/build-dragon/actions/workflows/assembly.yml) [![Swift Build](https://github.com/swirly-crop/build-dragon/actions/workflows/swift.yml/badge.svg)](https://github.com/swirly-crop/build-dragon/actions/workflows/swift.yml) [![C++ Build](https://github.com/swirly-crop/build-dragon/actions/workflows/cpp.yml/badge.svg)](https://github.com/swirly-crop/build-dragon/actions/workflows/cpp.yml)

Collection of auditor, performance, and example native code used for device privacy audits and experimentation.

IMPORTANT: This project intentionally does NOT contain or enable stealth/malicious software. It provides non-destructive auditing tools, examples, and educational assembly/C/C++/Swift snippets for device owners and defenders.

Repository layout
- android-auditor/: Python, C++ and Swift CLIs to audit Android devices (uses `adb`).
- ios_auditor/: Swift auditor and C/C++/ObjC bridge helpers (uses libimobiledevice tools).
- perf/: C++17 aligned-buffer benchmark and ARM64 NEON example.
- setout/: assorted C/C++/assembly examples and tests.

Quick start

- Run the Python Android auditor (requires `adb`):
  ```bash
  python3 android-auditor/auditor.py --devices
  ```

- Build the C++ Android sample (requires Android NDK):
  ```bash
  export ANDROID_NDK=/path/to/android-ndk
  mkdir -p android-auditor/cpp_auditor/android_cpp/build && cd android-auditor/cpp_auditor/android_cpp/build
  cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-21 -DANDROID_NDK=$ANDROID_NDK -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake ..
  cmake --build . --config Release
  ```

- Build and run the perf benchmark (native host):
  ```bash
  mkdir -p android-auditor/perf/build && cd android-auditor/perf/build
  cmake ..
  cmake --build .
  ./perf_benchmark
  ```

- Build the iOS bridge (macOS/Linux with `libimobiledevice` and Swift):
  ```bash
  cd ios_auditor/bridge
  clang -c bridge.cpp -o bridge.o
  clang -c objc_wrapper.m -o objc_wrapper.o
  clang -c ../assembly/hello_x86_64.s -o hello.o
  ar rcs libbridge.a bridge.o objc_wrapper.o hello.o
  # then link into Swift if swiftc is installed
  ```

Contributing
- This repo is intended for defensive research and education. If you contribute, keep changes non-destructive and document them.

License
- none yet
