#!/bin/bash

echo "========================================"
echo "  C Syntax Checker - GUI Server"
echo "========================================"
echo ""

echo "Checking Python installation..."
if ! command -v python3 &> /dev/null; then
    echo "ERROR: Python 3 is not installed"
    echo "Please install Python 3"
    exit 1
fi

echo ""
echo "Checking if SyntaxChecker is compiled..."
if [ ! -f "./SyntaxChecker" ]; then
    echo "WARNING: SyntaxChecker not found!"
    echo "Compiling now..."
    gcc RumMain.c MainFunc.c Braces.c semicolon.c StackFunc.c QueueFunc.c variable.c function1.c -o SyntaxChecker
    if [ $? -ne 0 ]; then
        echo "ERROR: Compilation failed!"
        exit 1
    fi
    echo "Compilation successful!"
fi

echo ""
echo "Installing Python dependencies..."
pip3 install -q Flask Werkzeug 2>/dev/null || pip install -q Flask Werkzeug 2>/dev/null

echo ""
echo "Starting GUI server..."
echo ""
echo "========================================"
echo "  Server will start on:"
echo "  http://localhost:5000"
echo "========================================"
echo ""
echo "Press Ctrl+C to stop the server"
echo ""

python3 app.py || python app.py


