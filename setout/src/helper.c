#include "helper.h"

int helper_compute(int x) {
    // small deterministic calculation
    int res = 0;
    for (int i = 0; i < x; ++i) res += (i * 31) ^ (i >> 3);
    return res;
}
