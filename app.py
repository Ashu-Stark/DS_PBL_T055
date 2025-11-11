from flask import Flask, render_template, request, jsonify
from flask_cors import CORS
import subprocess
import os
import tempfile
import re

app = Flask(__name__)
CORS(app)

# Path to the compiled syntax checker
SYNTAX_CHECKER = "SyntaxChecker.exe" if os.name == 'nt' else "./SyntaxChecker"

@app.route('/')
def index():
    """Serve the main HTML page"""
    return render_template('index.html')

@app.route('/check', methods=['POST'])
def check_syntax():
    """Check C code syntax"""
    try:
        data = request.get_json()
        code = data.get('code', '')
        
        if not code:
            return jsonify({'error': 'No code provided'}), 400
        
        # Create a temporary file with the C code
        with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as temp_file:
            temp_file.write(code)
            temp_filename = temp_file.name
        
        try:
            # Run the syntax checker
            result = subprocess.run(
                [SYNTAX_CHECKER, temp_filename],
                capture_output=True,
                text=True,
                timeout=10
            )
            
            # Parse the output
            output = result.stdout + result.stderr
            errors = parse_errors(output)
            
            return jsonify({
                'hasErrors': len(errors) > 0,
                'errors': errors,
                'output': output
            })
            
        finally:
            # Clean up temporary file
            if os.path.exists(temp_filename):
                os.unlink(temp_filename)
    
    except subprocess.TimeoutExpired:
        return jsonify({'error': 'Syntax check timed out'}), 500
    except Exception as e:
        return jsonify({'error': str(e)}), 500

def parse_errors(output):
    """Parse error messages from syntax checker output"""
    errors = []
    lines = output.strip().split('\n')
    
    for line in lines:
        line = line.strip()
        if line and not line.startswith('=') and line:
            # Skip empty lines and separator lines
            if line and line != '':
                errors.append(line)
    
    return errors

if __name__ == '__main__':
    # Check if syntax checker exists
    if not os.path.exists(SYNTAX_CHECKER):
        print(f"ERROR: {SYNTAX_CHECKER} not found!")
        print("Please compile it first using:")
        print("  gcc RumMain.c MainFunc.c Braces.c semicolon.c StackFunc.c QueueFunc.c variable.c function1.c -o SyntaxChecker.exe")
        exit(1)
    
    print("=" * 60)
    print("  C Syntax Checker Server")
    print("=" * 60)
    print(f"  Server running on: http://localhost:5000")
    print(f"  Syntax checker: {SYNTAX_CHECKER}")
    print("=" * 60)
    print("\nPress Ctrl+C to stop the server\n")
    
    app.run(debug=True, host='0.0.0.0', port=5000)
