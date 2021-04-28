@REM description: start docsify
@REM usage: st [-d dir] [-p port]
@REM dir (optional): site root dir,docs (default)
@REM port (optional): port,3000 (default)
@REM eg: st / st -d docs2 / st -p 4000
@REM eg: st -d doc2 -p 4000

@echo off
set d=docs
set p=3000

if "%~1" equ "" goto start;

if "%~1" equ "-d" (
    set d=%~2
    if "%~3" equ "-p" set p=%~4
) else if "%~1" equ "-p" (
    set p=%~2
    if "%~3" equ "-d" set p=%~4
)

:start
docsify s %d% --port %p%
exit /b