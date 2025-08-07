```Compiling
    zig build-lib .\xor128_2.zig -static
    gcc main.c -o main.exe -L. -lxor128_2
