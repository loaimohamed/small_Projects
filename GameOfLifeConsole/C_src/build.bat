@echo off

gcc main.c -o main.exe

if "%~1" == "INTER" (
    main.exe
) else (
    call start ./main.exe
    @REM start ./main.exe
)
