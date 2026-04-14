#include "aligned_buffer.h"
#include <chrono>
#include <cstring>
#include <iostream>

extern "C" void neon_memcpy(void *dst, const void *src, size_t n);

static inline void fallback_neon_memcpy(void *dst, const void *src, size_t n) {
    std::memcpy(dst, src, n);
}

int main() {
    const size_t size = 32 * 1024 * 1024; // 32 MB
    AlignedBuffer a(size), b(size);
    if (!a.data() || !b.data()) {
        std::cerr << "Allocation failed\n";
        return 1;
    }

    // init source
    std::memset(b.data(), 0xAA, size);

    // select copy function
#if defined(__aarch64__)
    auto copy_fn = neon_memcpy;
#else
    auto copy_fn = fallback_neon_memcpy;
#endif

    // warmup
    copy_fn(a.data(), b.data(), size);

    // timed runs
    const int runs = 5;
    for (int i = 0; i < runs; ++i) {
        auto t0 = std::chrono::high_resolution_clock::now();
        copy_fn(a.data(), b.data(), size);
        auto t1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> s = t1 - t0;
        double gb = double(size) / (1024.0 * 1024.0 * 1024.0);
        std::cout << "Run " << i << ": " << (gb / s.count()) << " GB/s\n";
    }

    return 0;
}
