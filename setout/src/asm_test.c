#include <stdio.h>
#include <string.h>

extern void fast_memcpy_x86_64(void *dst, const void *src, unsigned long n);
extern void fast_memcpy_aarch64(void *dst, const void *src, unsigned long n);

int main() {
    char src[64];
    char dst[64];
    for (int i = 0; i < 64; ++i) src[i] = (char)i;
    memset(dst, 0, sizeof(dst));

    // call available asm implementations
    fast_memcpy_x86_64(dst, src, 64);
    printf("dst[0]=%d dst[63]=%d\n", (int)dst[0], (int)dst[63]);

    memset(dst, 0, sizeof(dst));
    fast_memcpy_aarch64(dst, src, 64);
    printf("dst[0]=%d dst[63]=%d\n", (int)dst[0], (int)dst[63]);

    return 0;
}
