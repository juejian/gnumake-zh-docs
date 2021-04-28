@echo off
@REM goto :end

chcp 65001 & cls
set in_dir=G:/itstudy-doc/gnumake-zh-docs/docs
set inf=md_flist.txt
set of=md_toc.md

if exist "%of%" del "%of%"

md_toc.exe -b %in_dir% -i %inf% -o %of%

:end
echo.& echo finish
pause>nul
@REM exit /b
