@echo off

set SRC=src/main.c src/json.c
set INCLUDE=-Iheader

gcc %SRC% %INCLUDE% -o program.exe

program.exe
pause