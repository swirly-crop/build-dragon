# Building the assembly examples

This document explains how to build the assembly samples included in `android-auditor/assembly` for both x86_64 (host) and aarch64 (ARM64) targets.

Prerequisites
- On x86_64 host: `gcc`/`ld` (GNU toolchain) to assemble/link AT&T-syntax assembly.
- Cross-compile aarch64 toolchain: `aarch64-linux-gnu-gcc` (Debian/Ubuntu package `gcc-aarch64-linux-gnu`) or Android NDK.

Build (native x86_64)

```bash
cd android-auditor/assembly
make x86_64
./bin/hello
```

This uses `gcc -nostdlib -static` to produce a minimal ELF that starts at `_start`.

Cross-build (aarch64) using Linux cross-compiler

Install cross-compiler on Debian/Ubuntu:

```bash
sudo apt update
sudo apt install -y gcc-aarch64-linux-gnu
```

Then build:

```bash
cd android-auditor/assembly
make aarch64
```

The produced binary `bin/hello_aarch64` is an aarch64 ELF and can be run on aarch64 hosts or pushed to an Android device (if built for Android use the NDK method below).

Cross-build for Android using Android NDK (clang)

```bash
export ANDROID_NDK=/path/to/android-ndk
cd android-auditor/assembly
$ANDROID_NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang -target aarch64-none-linux-android -nostdlib -o bin/hello_aarch64 hello_aarch64.s
```

Push to device (adb)

```bash
adb push bin/hello_aarch64 /data/local/tmp/
adb shell chmod +x /data/local/tmp/hello_aarch64
adb shell /data/local/tmp/hello_aarch64
```

Notes
- The assembly sources use AT&T syntax compatible with `gcc` and `as` on Linux x86_64 and aarch64 respectively.
- We add `.note.GNU-stack` sections in the sources so the linker does not warn about executable stacks.