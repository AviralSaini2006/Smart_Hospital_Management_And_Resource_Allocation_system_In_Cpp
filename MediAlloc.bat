
@echo off
title MediAlloc Hospital Management System

cd /d "%~dp0"

echo ========================================
echo       MediAlloc Hospital System
echo ========================================
echo.

g++ -std=c++17 -Wall -Wextra *.cpp -o MediAlloc.exe

if errorlevel 1 (
    echo.
    echo Compilation failed. Check the errors above.
    pause
    exit /b 1
)

echo Compilation successful!
echo Starting MediAlloc...
echo.

MediAlloc.exe

echo.
echo Program ended.
pause
