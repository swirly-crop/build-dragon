#include <stdio.h>
#include "helper.h"

int main(void) {
    printf("C app: starting\n");
    int v = helper_compute(42);
    printf("helper_compute(42) = %d\n", v);
    return 0;
}
