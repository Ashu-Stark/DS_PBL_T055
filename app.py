from flask import Flask, render_template, request, jsonify
from flask_cors import CORS
import subprocess
import os
import tempfile
import re

app = Flask(__name__)
CORS(app)

SYNTAX_CHECKER = "SyntaxChecker.exe" if os.name == 'nt' else "./SyntaxChecker"

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/about')
def about():
    return render_template('about.html')

@app.route('/contact')
def contact():
    return render_template('contact.html')

@app.route('/check', methods=['POST'])
def check_syntax():
    try:
        data = request.get_json()
        code = data.get('code', '')
        
        if not code:
            return jsonify({'error': 'No code provided'}), 400
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as temp_file:
            temp_file.write(code)
            temp_filename = temp_file.name
        
        try:
            result = subprocess.run(
                [SYNTAX_CHECKER, temp_filename],
                capture_output=True,
                text=True,
                timeout=10
            )
            
            output = result.stdout + result.stderr
            errors = parse_errors(output)
            
            return jsonify({
                'hasErrors': len(errors) > 0,
                'errors': errors,
                'output': output
            })
            
        finally:
            if os.path.exists(temp_filename):
                os.unlink(temp_filename)
    
    except subprocess.TimeoutExpired:
        return jsonify({'error': 'Syntax check timed out'}), 500
    except Exception as e:
        return jsonify({'error': str(e)}), 500

def parse_errors(output):
    errors = []
    lines = output.strip().split('\n')
    
    skip_headers = True
    for line in lines:
        line = line.strip()
        if not line:
            continue
        if "The list of Error" in line or "given below" in line:
            skip_headers = False
            continue
        if line.lower() == "no error":
            continue
        if line and not line.startswith('='):
            if line and line != '':
                errors.append(line)
    
    return errors

if __name__ == '__main__':
    if not os.path.exists(SYNTAX_CHECKER):
        print(f"ERROR: {SYNTAX_CHECKER} not found!")
        print("Please compile it first using:")
        print("  gcc RumMain.c MainFunc.c Braces.c semicolon.c StackFunc.c QueueFunc.c variable.c function1.c -o SyntaxChecker.exe")
        exit(1)
    
    print("=" * 60)
    print("  C Syntax Checker Server")
    print("=" * 60)
    print(f"  Server running on: http://localhost:5100")
    print(f"  Syntax checker: {SYNTAX_CHECKER}")
    print("=" * 60)
    print("\nPress Ctrl+C to stop the server\n")
    
    app.run(debug=True, host='0.0.0.0', port=5100)
