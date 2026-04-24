#include <iostream>
#include <vector>
#include <chrono>
#include <csignal>
#include <setjmp.h>
#include <functional>
#include <thread>
#include <mutex>

// External functions
extern "C" long long sum_array_scalar(const long long* arr, int n);
extern "C" long long sum_array_simd(const long long* arr, int n);

// Safe versions
long long safe_sum_array_scalar(const long long* arr, int n) {
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum;
}

long long safe_sum_array_simd(const long long* arr, int n) {
    // Check CPU support (simplified)
    // In real code, use CPUID or __builtin_cpu_supports
    try {
        return sum_array_simd(arr, n);
    } catch (...) {
        throw std::runtime_error("SIMD failed");
    }
}

long long safe_sum_array_parallel(const long long* arr, int n) {
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    std::vector<long long> partial_sums(num_threads, 0);
    std::vector<std::thread> threads;

    auto worker = [&](int id) {
        int chunk = n / num_threads;
        int start = id * chunk;
        int end = (id == num_threads - 1) ? n : (id + 1) * chunk;
        for (int i = start; i < end; ++i) {
            partial_sums[id] += arr[i];
        }
    };

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    long long total = 0;
    for (auto s : partial_sums) {
        total += s;
    }
    return total;
}

long long safe_sum_array_gpu(const long long* arr, int n) {
    // Mock GPU: in real code, use Vulkan
    throw std::runtime_error("GPU not implemented");
}

// Signal handling for crashes
jmp_buf env;
volatile sig_atomic_t crashed = 0;

void signal_handler(int sig) {
    crashed = 1;
    longjmp(env, 1);
}

long long run_with_fallback(std::function<long long()> func, const char* name) {
    if (setjmp(env) == 0) {
        crashed = 0;
        signal(SIGSEGV, signal_handler);
        signal(SIGBUS, signal_handler);
        signal(SIGILL, signal_handler);
        auto result = func();
        signal(SIGSEGV, SIG_DFL);
        signal(SIGBUS, SIG_DFL);
        signal(SIGILL, SIG_DFL);
        return result;
    } else {
        std::cerr << "Crash detected in " << name << ", falling back..." << std::endl;
        throw std::runtime_error(std::string("Crash in ") + name);
    }
}

long long realtime_sum(const long long* arr, int n) {
    std::vector<std::pair<std::function<long long()>, const char*>> methods = {
        {[&]() { return safe_sum_array_gpu(arr, n); }, "GPU"},
        {[&]() { return safe_sum_array_simd(arr, n); }, "SIMD"},
        {[&]() { return safe_sum_array_parallel(arr, n); }, "Parallel"},
        {[&]() { return safe_sum_array_scalar(arr, n); }, "Scalar"}
    };

    for (auto& method : methods) {
        try {
            auto start = std::chrono::high_resolution_clock::now();
            long long result = run_with_fallback(method.first, method.second);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "Using " << method.second << ": " << result << " in " << elapsed.count() << "s" << std::endl;
            return result;
        } catch (const std::exception& e) {
            std::cerr << "Method " << method.second << " failed: " << e.what() << std::endl;
        }
    }
    throw std::runtime_error("All methods failed");
}

int main() {
    const int N = 1000000;
    std::vector<long long> arr(N);
    for (int i = 0; i < N; ++i) {
        arr[i] = i % 100;
    }

    try {
        long long result = realtime_sum(arr.data(), N);
        std::cout << "Final result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "All sum methods failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}