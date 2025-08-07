@echo off

gcc main.c -Wall -Wextra -o main.exe

@cls

if "%~1" == "INTER" (
    .\main.exe
) else (
    start "title" call .\main.exe
)
