#include <iostream>
#include "util.h"

int main() {
    std::cout << "C++ app: starting" << std::endl;
    auto v = util_compute(100);
    std::cout << "util_compute(100) = " << v << std::endl;
    return 0;
}
