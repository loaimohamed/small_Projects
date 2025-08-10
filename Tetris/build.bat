@echo off

@cls

gcc main.c -Wall -Wextra -Wno-unused-variable -o main.exe

if "%~1" == "INTER" (
    .\main.exe
) else (
    start "title" call .\main.exe
)
