@echo off
echo.call windres
windres main.rc -o tmp.o
echo.call g++
g++ -o main.exe main.cpp tmp.o -s -O2
echo.del tmp file
del /q tmp.o
pause