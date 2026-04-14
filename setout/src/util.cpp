#include "util.h"

int util_compute(int n) {
    int s = 1;
    for (int i = 1; i <= n; ++i) s = (s * 33) ^ (i & 0xFF);
    return s;
}
