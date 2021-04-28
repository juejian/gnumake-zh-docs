@echo off
goto :end

chcp 65001 & cls
set f=filelist.txt
set d=docs\zh-cn

if "%~1" == "1" goto mf
if "%~1" == "2" goto df
if "%~1" == "3" goto visit
echo lost params.
goto end

:mf
for /f "usebackq" %%a in ("%f%") do (
    @REM echo %%a
    if "%%a" neq "" type nul>"%d%\%%a.md"
)
goto end

:df
for /f "usebackq" %%a in ("%f%") do (
    @REM echo %%a
    if "%%a" neq "" del "%d%\%%a.md"
)
goto end

:visit
for /f "usebackq" %%a in (`dir %d%\??-* /b`) do (echo %%a)

:end
echo.& echo finish
pause>nul
exit /b
