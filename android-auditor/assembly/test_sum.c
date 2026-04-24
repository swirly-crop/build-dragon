#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern long long sum_array(long long *arr, int n);
extern long long sum_array_simd(long long *arr, int n);

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

    start = clock();
    long long sum2 = sum_array_simd(arr, N);
    end = clock();
    double time2 = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Assembly scalar sum: %lld, time: %f\n", sum1, time1);
    printf("Assembly SIMD sum: %lld, time: %f\n", sum2, time2);
    printf("Boost: %fx\n", time1 / time2);

    free(arr);
    return 0;
}