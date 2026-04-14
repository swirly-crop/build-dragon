# Android C++17 Auditor (arm64-v8a)

Small C++17 native executable example intended to be built with the Android NDK for `arm64-v8a` (Samsung tablets with 64-bit ARM CPUs).

Prerequisites
- Android NDK (r21+ recommended)
- CMake (3.10+)

Build (example):

```bash
export ANDROID_NDK=/path/to/android-ndk
mkdir -p android-auditor/cpp_auditor/android_cpp/build && cd android-auditor/cpp_auditor/android_cpp/build
cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-21 -DANDROID_NDK=$ANDROID_NDK -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake ..
cmake --build . --config Release
```

After build, the `auditor` binary can be pushed to a device and run (device must allow running user binaries):

```bash
adb push auditor /data/local/tmp/
adb shell chmod 755 /data/local/tmp/auditor
adb shell /data/local/tmp/auditor
```

This example is non-destructive and prints basic diagnostic text and Android log output.
