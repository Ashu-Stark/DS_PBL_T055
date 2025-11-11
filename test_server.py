#!/usr/bin/env python3
"""Quick test script to verify the server works"""

import requests
import time
import subprocess
import sys
import os

def test_server():
    print("Testing C Syntax Checker Server...")
    print("=" * 50)
    
    # Test code
    test_code = """#include <stdio.h>

int main() {
    printf("Hello");
    return 0;
}"""
    
    try:
        # Start server in background (this is a simple test)
        print("\n1. Testing if server can start...")
        print("   (You should see server output above)")
        
        # Test the /check endpoint
        print("\n2. Testing /check endpoint...")
        url = "http://127.0.0.1:5000/check"
        
        response = requests.post(
            url,
            json={'code': test_code},
            timeout=5
        )
        
        if response.status_code == 200:
            data = response.json()
            print(f"   ✓ Server responded successfully!")
            print(f"   ✓ Has errors: {data.get('hasErrors', False)}")
            print(f"   ✓ Error count: {len(data.get('errors', []))}")
            if data.get('errors'):
                print("   Errors found:")
                for err in data['errors']:
                    print(f"     - {err}")
            return True
        else:
            print(f"   ✗ Server returned status {response.status_code}")
            print(f"   Response: {response.text}")
            return False
            
    except requests.exceptions.ConnectionError:
        print("\n   ✗ Cannot connect to server!")
        print("   Make sure the server is running on http://127.0.0.1:5000")
        print("\n   To start the server, run: python app.py")
        return False
    except Exception as e:
        print(f"\n   ✗ Error: {e}")
        return False

if __name__ == "__main__":
    print("\nNOTE: Make sure the server is running in another terminal!")
    print("Run: python app.py\n")
    time.sleep(2)
    test_server()


