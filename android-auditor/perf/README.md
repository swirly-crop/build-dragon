# Buffer performance utilities

This folder contains a C++17 aligned buffer allocator and a small benchmark program that compares `std::memcpy` to an ARM64 NEON-optimized `neon_memcpy` (when built on aarch64).

Build (native):

```bash
mkdir -p android-auditor/perf/build && cd android-auditor/perf/build
cmake ..
cmake --build .
./perf_benchmark
```

Notes:
- On non-aarch64 hosts the NEON routine is disabled and the benchmark falls back to `std::memcpy`.
- The assembly routine is educational and copies 16-byte NEON vectors in a loop.
