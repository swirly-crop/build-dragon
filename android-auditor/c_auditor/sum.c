#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long sum_array(long long *arr, int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

#ifdef __x86_64__
#include <immintrin.h>

long long sum_array_simd(long long *arr, int n) {
    __m256i sum_vec = _mm256_setzero_si256();
    int i;
    for (i = 0; i + 4 <= n; i += 4) {
        __m256i vec = _mm256_loadu_si256((__m256i*)&arr[i]);
        sum_vec = _mm256_add_epi64(sum_vec, vec);
    }
    long long sum = 0;
    sum += _mm256_extract_epi64(sum_vec, 0);
    sum += _mm256_extract_epi64(sum_vec, 1);
    sum += _mm256_extract_epi64(sum_vec, 2);
    sum += _mm256_extract_epi64(sum_vec, 3);
    for (; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}
#endif

int main() {
    const int N = 1000000;
    long long *arr = malloc(N * sizeof(long long));
    for (int i = 0; i < N; i++) {
        arr[i] = i % 100;
    }

    clock_t start = clock();
    long long sum1 = sum_array(arr, N);
    clock_t end = clock();
    double time1 = (double)(end - start) / CLOCKS_PER_SEC;

#ifdef __x86_64__
    start = clock();
    long long sum2 = sum_array_simd(arr, N);
    end = clock();
    double time2 = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Scalar sum: %lld, time: %f\n", sum1, time1);
    printf("SIMD sum: %lld, time: %f\n", sum2, time2);
    printf("Boost: %fx\n", time1 / time2);
#else
    printf("Sum: %lld, time: %f\n", sum1, time1);
#endif

    free(arr);
    return 0;
}