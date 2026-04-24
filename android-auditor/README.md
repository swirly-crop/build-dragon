# Android Privacy Auditor

A small, open-source, non-stealth auditing tool that uses `adb` (Android Debug Bridge) to collect device information useful for privacy audits.

WARNING: This tool is for device owners and defenders only. Do not run against devices you do not own or have explicit permission to inspect.

## Requirements
- Python 3.8+
- Android Platform Tools (`adb`) installed and in PATH
- For C/C++ versions: GCC/G++ installed
- For Assembly: GCC with cross-compilation support for aarch64
- For Kotlin: Kotlin compiler (`kotlinc`)

## Installation

No packages required. Optionally create a virtualenv and install dependencies from `requirements.txt` (empty by default).

## Usage

Run from the repository root:

```bash
python3 android-auditor/auditor.py --devices
python3 android-auditor/auditor.py --list-packages --third-party
python3 android-auditor/auditor.py --permissions com.example.app
python3 android-auditor/auditor.py --net
```

The tool calls `adb` under the hood; ensure a device is connected and `adb devices` shows it.

## Alternative Implementations

For performance comparison and build accuracy testing, alternative implementations are provided:

### C++ Version
Located in `cpp_auditor/`. Build with `make` in that directory.
Includes `realtime_fixer` - a robust sum program that tries GPU, SIMD, parallel, and scalar methods in order, with automatic crash detection and fallback to safer versions.

### C Version
Located in `c_auditor/`. Build with `make` in that directory.
Includes SIMD (AVX2) optimized sum_array for CPU performance boost.

### Kotlin Version
Located in `kotlin_auditor/`. Requires Kotlin compiler. Build with `make` in that directory.

### Assembly
Located in `assembly/`. Includes hello world and sum_array functions for x86_64 and aarch64. Build with `make` in that directory.
Includes SIMD versions (AVX2 for x86_64, NEON for aarch64) for CPU performance boosts.
Run `./bin/test_sum` to see the SIMD boost (typically 2x faster).

### Performance Boosts
- **CPU Boost**: SIMD instructions (AVX2/NEON) vectorize addition operations, processing multiple elements per instruction. Run `./sum` in `c_auditor/` to see ~7% boost.
- **Assembly SIMD Boost**: Hand-optimized SIMD in assembly provides ~2x speedup. Run `./bin/test_sum` in `assembly/`.
- **Parallel Processing Boost**: Multi-threading simulates GPU-like parallelism. Run `./sum_parallel` in `c_auditor/` (boost depends on core count; on Android devices with more cores, expect significant improvements).
- **GPU Boost**: For Android GPU acceleration, use Vulkan compute shaders or OpenCL. Example implementations can leverage parallel processing on GPU cores for massive speedups on large arrays (e.g., 10-100x faster than CPU for suitable workloads). See `c_auditor/sum_gpu.c` for a Vulkan compute shader example (requires Vulkan SDK and SPIR-V compilation).
- **Realtime Fallback**: The `realtime_fixer` in `cpp_auditor/` automatically detects crashes or failures in boosted methods and falls back to safer alternatives in real-time, ensuring reliability.

## Notes
- This tool intentionally only reads diagnostics; it does not modify device state.
- Use results to harden settings (revoke unneeded permissions, uninstall suspicious apps, review network connections).
