#include <stdio.h>


void printBits(int *x, int count) {
    for (int i = 0; i < count; i++) {
        int b = (x >> count-i);
        printf("(%d): (%d)\n", i, b);
    }
    
}


//SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMM
int main() {
    printf("Begin");

    int x = 5;
    printBits(&x, sizeof(x) * sizeof(int));
}
