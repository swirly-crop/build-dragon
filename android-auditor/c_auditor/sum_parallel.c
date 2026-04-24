#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    long long *arr;
    int start, end;
    long long sum;
} thread_data_t;

void *sum_partial(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    data->sum = 0;
    for (int i = data->start; i < data->end; i++) {
        data->sum += data->arr[i];
    }
    return NULL;
}

long long sum_array_parallel(long long *arr, int n, int num_threads) {
    pthread_t threads[num_threads];
    thread_data_t data[num_threads];
    int chunk = n / num_threads;
    for (int i = 0; i < num_threads; i++) {
        data[i].arr = arr;
        data[i].start = i * chunk;
        data[i].end = (i == num_threads - 1) ? n : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, sum_partial, &data[i]);
    }
    long long total = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total += data[i].sum;
    }
    return total;
}

int main() {
    const int N = 1000000;
    long long *arr = malloc(N * sizeof(long long));
    for (int i = 0; i < N; i++) {
        arr[i] = i % 100;
    }

    clock_t start = clock();
    long long sum1 = sum_array_parallel(arr, N, 1);
    clock_t end = clock();
    double time1 = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    long long sum2 = sum_array_parallel(arr, N, 4);
    end = clock();
    double time2 = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Single thread sum: %lld, time: %f\n", sum1, time1);
    printf("4 threads sum: %lld, time: %f\n", sum2, time2);
    printf("Boost: %fx\n", time1 / time2);

    free(arr);
    return 0;
}