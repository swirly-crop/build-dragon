#include <iostream>

long long add_sum(long long a, long long b) {
    return a + b;
}

int main() {
    long long a = 10, b = 20;
    std::cout << "Sum: " << add_sum(a, b) << std::endl;
    return 0;
}