@echo off

set SOURCE_FILE=src\mult.c
set OUTPUT_NAME=bin\mult.exe

ECHO Compile started...

gcc -std=gnu99 -O2 -g -Wall -o %OUTPUT_NAME% %SOURCE_FILE%

if errorlevel 1 (
  ECHO Compile gagal.
) else (
  ECHO Compile berhasil! Program '%OUTPUT_NAME%' telah dibuat.
)

PAUSE