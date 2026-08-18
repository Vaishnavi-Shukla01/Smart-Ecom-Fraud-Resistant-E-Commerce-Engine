@echo off
title SmartEcom - Fraud-Resistant E-Commerce Engine
color 0A

echo ============================================
echo   SmartEcom - Starting Application
echo ============================================
echo.

where gcc >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] GCC compiler not found!
    echo Please install MinGW or add GCC to your PATH.
    pause
    exit /b 1
)

where python >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Python not found!
    echo Please install Python and add it to your PATH.
    pause
    exit /b 1
)

cd /d "%~dp0"

if not exist "bin" mkdir bin

echo [1/3] Compiling C backend...
gcc -I include src\main.c src\auth.c src\product.c src\cart.c src\fraud.c src\order.c -o bin\smartecom.exe

if %ERRORLEVEL% neq 0 (
    echo [ERROR] C compilation failed!
    pause
    exit /b 1
)
echo [OK] C backend compiled successfully!
echo.

echo [2/3] Checking Flask installation...
python -c "import flask" 2>nul
if %ERRORLEVEL% neq 0 (
    echo [INFO] Flask not found. Installing Flask...
    pip install flask
)
echo [OK] Flask is ready!
echo.

echo [3/3] Starting Flask server...
echo ============================================
echo   Server running at http://127.0.0.1:5000
echo   Press Ctrl+C to stop the server
echo ============================================
echo.

python app.py

pause
