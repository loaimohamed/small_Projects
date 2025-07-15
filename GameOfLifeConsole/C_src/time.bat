@echo off

setlocal enabledelayedexpansion

echo Checking Running Time...

GOTO :Start_Time

cmd /c

GOTO :End_Time


:Start_Time
    for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
        rem set /a start=(((1%%a%%100)*60+1%%b%%100)*60+1%%c%%100)*100+1%%d%%100 - 111111100
        echo    Start Time = %%a:%%b:%%c:%%d
        set /a 
    )
:End_Time
   for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
        set /a end=1%a
        echo    End Time = %%a:%%b:%%c:%%d
    )
   exit /b end
:CalculateTime
    
