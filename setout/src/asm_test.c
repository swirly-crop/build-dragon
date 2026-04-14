#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <string.h>

#if defined(__x86_64__) || defined(_M_X64)
extern void fast_memcpy_x86_64(void *dst, const void *src, unsigned long n);
#elif defined(__aarch64__)
extern void fast_memcpy_aarch64(void *dst, const void *src, unsigned long n);
#endif

int main() {
    char src[64];
    char dst[64];
    for (int i = 0; i < 64; ++i) src[i] = (char)i;
    memset(dst, 0, sizeof(dst));

#if defined(__x86_64__) || defined(_M_X64)
    fast_memcpy_x86_64(dst, src, 64);
    printf("[x86_64] dst[0]=%d dst[63]=%d\n", (int)dst[0], (int)dst[63]);
#elif defined(__aarch64__)
    fast_memcpy_aarch64(dst, src, 64);
    printf("[aarch64] dst[0]=%d dst[63]=%d\n", (int)dst[0], (int)dst[63]);
#else
    // fallback
    memcpy(dst, src, 64);
    printf("[fallback memcpy] dst[0]=%d dst[63]=%d\n", (int)dst[0], (int)dst[63]);
#endif

    return 0;
}
