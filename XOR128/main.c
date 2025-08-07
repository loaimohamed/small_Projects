#include <stdio.h>
#include <stdint.h>
#include "main.h"

int main() {
    printf("Welcome\n");
    XOR128 xor;
    int xorState = xor128_new(&xor, 12345678);
    printf("XOR128_State: %d\n", xorState);
    printf("Random Int: %llu\n", xor128_next(&xor));
    printf("Random Float: %f\n", xor128_xfloat(&xor));
    return 0;
}
