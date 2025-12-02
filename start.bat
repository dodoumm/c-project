@echo off

chcp 65001>nul

set SRC=src/main.c src/json.c src/parser.c
set INCLUDE=-Iheader

gcc %SRC% %INCLUDE% -o program.exe

program.exe
pause