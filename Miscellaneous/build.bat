@echo off

@cls

if "%~1"=="" (
    echo Error: No argument Provided
    exit /b 1
)

set "file=%~1"
set "lcom=gcc gfortran gnat swiftc go rustc"
set "lext=.c .f .adc .swift .go .rs"

set "extension=%~x1"
set "filename=%~n1"

set "compiler=echo No Compiler Was Found"


set "index=0"

if not exist "%file%" (
    echo Error: File '%file%' not found!
    exit /b 1
)

for %%i in (%lext%) do (
    call :get_compiler !index!
    echo NINE: %%i uses !current_compiler!
    set /a index+=1
)
goto :eof

:get_compiler
set "compilers=%lcom%"
set "count=0"
for %%c in (%compilers%) do (
    if !count! equ %1 set "current_compiler=%%c"
    set /a count+=1
)
goto :eof

@REM @REM echo "%extension%"

