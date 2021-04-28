@echo off
set p=3000
if "%~1" neq "" set p=%~1
start http://localhost:%p%
exit /b
