@echo off
echo ========================================
echo   C Syntax Checker - GUI Server
echo ========================================
echo.
echo Checking Python installation...
python --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Python is not installed or not in PATH
    echo Please install Python 3 from https://www.python.org/
    pause
    exit /b 1
)

echo.
echo Checking if SyntaxChecker is compiled...
if not exist "SyntaxChecker.exe" (
    echo WARNING: SyntaxChecker.exe not found!
    echo Compiling now...
    gcc RumMain.c MainFunc.c Braces.c semicolon.c StackFunc.c QueueFunc.c variable.c function1.c -o SyntaxChecker.exe
    if errorlevel 1 (
        echo ERROR: Compilation failed!
        pause
        exit /b 1
    )
    echo Compilation successful!
)

echo.
echo Installing Python dependencies...
python -m pip install -q Flask Werkzeug flask-cors
if errorlevel 1 (
    echo ERROR: Failed to install dependencies
    echo Trying alternative method...
    pip install Flask Werkzeug flask-cors
    if errorlevel 1 (
        pause
        exit /b 1
    )
)

echo.
echo Starting GUI server...
echo.
echo ========================================
echo   Server will start on:
echo   http://localhost:5100
echo ========================================
echo.
echo Press Ctrl+C to stop the server
echo.

python app.py

pause

