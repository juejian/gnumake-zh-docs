@REM description: stop docsify
@REM usage: sp [port]
@REM port (optional): docsify start port, 3000 default
@echo off
set port=3000
if "%~1" neq "" set port=%~1

@REM use bofore to avoid kill same process many times.

setlocal enabledelayedexpansion
for /f "tokens=1-5" %%a in ('netstat -ano^|findstr %port%') do (
    @REM %%e (5th item): process id
    if "!before!" neq "%%e" (
        @REM look process msg pid=%%e
        @REM tasklist /fi "pid eq %%e" /v /fo list

        @REM kill pid=%%e process and it's all sub process
        taskkill /pid %%e /f /t 1>nul 2>nul
        set before=%%e
    )
)
endlocal
echo exit finish.
REM pause>nul
exit /b