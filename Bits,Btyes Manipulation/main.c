#include <stdio.h>
#include <stdint.h>
#include <math.h>

double intBitsToFloat(uint32_t bits) {
    // Extract fields
    int sign     = (bits >> 31) & 1;
    int exponent = (bits >> 23) & 0xFF;
    uint32_t mantissa = bits & 0x7FFFFF;

    printf("Raw bits: 0x%08X\n", bits);
    printf("Sign = %d, Exponent = %d, Mantissa = %u\n", sign, exponent, mantissa);

    double value;
    if (exponent == 255) {
        if (mantissa == 0) {
            value = sign ? -INFINITY : INFINITY;
        } else {
            value = NAN; // Not a Number
        }
    } else if (exponent == 0) {
        if (mantissa == 0) {
            value = sign ? -0.0 : 0.0; // signed zero
        } else {
            // Subnormal number
            value = pow(-1.0, sign) *
                    pow(2.0, 1 - 127) *
                    ((double)mantissa / (1 << 23));
        }
    } else {
        // Normal number
        value = pow(-1.0, sign) *
                pow(2.0, exponent - 127) *
                (1.0 + (double)mantissa / (1 << 23));
    }

    return value;
}

int main() {
    uint32_t bits = 0x40490FDB; // pi in float bits
    double f = intBitsToFloat(bits);
    printf("Decoded value = %.10f\n", f);
    return 0;
}
