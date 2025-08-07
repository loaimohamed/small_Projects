#include <stdint.h>

typedef struct {
        uint64_t x, y,z,w;
} XOR128;

int xor128_new(XOR128* xor128, uint64_t seed);
uint64_t xor128_next(XOR128* xor);
double xor128_xfloat(XOR128* xor);


