@rem 0 offline (default),1 online
@echo off
set s=index-offline.html
if "%~1" EQU "1" (set s=index-online.html & echo online) else (echo offline)
copy "docs\%s%" "docs\index.html" /y
pause
exit /b
