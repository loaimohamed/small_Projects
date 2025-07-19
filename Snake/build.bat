@echo off

gcc main.c -o main.exe

if "%~1" == "INTER" (
    .\main.exe
) else (
    start "title" call .\main.exe
)
