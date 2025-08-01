import hashlib
import requests
import base64
import os

from dotenv import load_dotenv

load_dotenv()

USERNAME = os.getenv('USERNAME')
PASSWORD = os.getenv('PASSWORD')
POW_PREFIX = os.getenv('POW_PREFIX')
ACTIVATE_URL = os.getenv('ACTIVATION_URL')

with open("public.key", "rb") as f:
    public_key_data = f.read()

public_key_b64 = base64.b64encode(public_key_data).decode('utf-8')

def find_nonce(pow_prefix, difficulty=5):
    """Find nonce for Proof of Work"""
    target = '0' * difficulty
    nonce = 0
    
    while True:
        test_str = f"{pow_prefix}:{nonce}"
        hash_result = hashlib.sha256(test_str.encode()).hexdigest()
        
        if hash_result.startswith(target):
            print(f"Found nonce: {nonce}")
            print(f"Hash: {hash_result}")
            return nonce
        
        nonce += 1
        if nonce % 10000 == 0:
            print(f"Trying nonce: {nonce}")

def activate_account():
    # Find the nonce
    nonce = find_nonce(POW_PREFIX)
    
    activation_data = {
        "username": USERNAME,
        "password": PASSWORD,
        "nonce": nonce,
        "public_key": public_key_b64
    }
    
    response = requests.post(ACTIVATE_URL, json=activation_data)
    
    print(f"Status Code: {response.status_code}")
    print(f"Response: {response.text}")
    
    return response.status_code == 200

if __name__ == "__main__":
    activate_account()