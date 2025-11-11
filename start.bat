@echo off

set SRC=src/main.c src/window.c
set INCLUDE=-Iheader

gcc %SRC% %INCLUDE% -o program.exe

program.exe
pause