@echo off
REM goto :end
cls & chcp 65001

set f=f-outline.txt
set o=next_prev.txt

gnp.exe -i %f% >%o%

echo result has already been written into "%o%" file.
pause>nul
exit /b
