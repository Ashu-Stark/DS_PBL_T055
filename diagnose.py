#!/usr/bin/env python3
"""Diagnostic script to check if everything is set up correctly"""

import os
import sys

def check_file(filename, description):
    if os.path.exists(filename):
        print(f"✓ {description}: {filename} - FOUND")
        return True
    else:
        print(f"✗ {description}: {filename} - NOT FOUND")
        return False

def check_python_module(module_name):
    try:
        __import__(module_name)
        print(f"✓ Python module '{module_name}' - INSTALLED")
        return True
    except ImportError:
        print(f"✗ Python module '{module_name}' - NOT INSTALLED")
        print(f"  Install with: python -m pip install {module_name}")
        return False

print("=" * 60)
print("C Syntax Checker - Diagnostic Check")
print("=" * 60)
print()

# Check Python version
python_version = sys.version_info
print(f"Python Version: {python_version.major}.{python_version.minor}.{python_version.micro}")
if python_version.major < 3 or (python_version.major == 3 and python_version.minor < 6):
    print("  ⚠ WARNING: Python 3.6+ recommended")
else:
    print("  ✓ Python version is OK")
print()

# Check required files
print("Checking required files...")
files_ok = True
files_ok &= check_file("app.py", "Flask application")
files_ok &= check_file("templates/index.html", "HTML template")
files_ok &= check_file("static/style.css", "CSS stylesheet")
files_ok &= check_file("static/script.js", "JavaScript file")
files_ok &= check_file("SyntaxChecker.exe", "Compiled syntax checker")
print()

# Check Python modules
print("Checking Python modules...")
modules_ok = True
modules_ok &= check_python_module("flask")
modules_ok &= check_python_module("flask_cors")
print()

# Check directories
print("Checking directory structure...")
dirs_ok = True
dirs_ok &= check_file("templates", "Templates directory")
dirs_ok &= check_file("static", "Static directory")
print()

# Summary
print("=" * 60)
if files_ok and modules_ok and dirs_ok:
    print("✓ All checks passed! You should be able to run the server.")
    print()
    print("To start the server, run:")
    print("  python app.py")
    print()
    print("Then open your browser to: http://localhost:5000")
else:
    print("✗ Some checks failed. Please fix the issues above.")
    print()
    if not modules_ok:
        print("To install missing Python modules:")
        print("  python -m pip install Flask flask-cors Werkzeug")
    if not check_file("SyntaxChecker.exe", ""):
        print("To compile the syntax checker:")
        print("  gcc RumMain.c MainFunc.c Braces.c semicolon.c StackFunc.c QueueFunc.c variable.c function1.c -o SyntaxChecker.exe")
print("=" * 60)


