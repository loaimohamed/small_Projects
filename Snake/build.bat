@echo off

gcc main.c -Wall -Wextra -o main.exe

if "%~1" == "INTER" (
    .\main.exe
) else (
    start "title" call .\main.exe
)
