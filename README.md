# RC4
This project explores RC4's practical security with 40-bit keys. It includes algorithm implementation, code, and a demo of encrypting and recovering a 64+ character message by an attacker. Assessing vulnerabilities, it documents risks and evaluates break time, shedding light on RC4's security limitations.

# RC4 Parallelized Brute-Force Attack

## Overview

This repository demonstrates a parallelized brute-force attack on the RC4 encryption algorithm using Python multiprocessing. RC4 is a widely-used symmetric key stream cipher known for its simplicity and efficiency.

## Contents

### 1. RC4 Encryption Algorithm (`rc4`)

The `rc4` function embodies the RC4 encryption algorithm. It takes a key and plaintext as input, producing encrypted ciphertext.

### 2. Check Key Function (`check_key`)

The `check_key` function seeks the correct key by iterating through a specified range of key candidates. It converts each candidate to a 40-bit key and employs the RC4 algorithm to decrypt the ciphertext. If the known plaintext is found in the decrypted text, it returns the key and decrypted text.

### 3. Simulate Attack Function (`simulate_attack`)

The `simulate_attack` function orchestrates a brute-force attack on RC4 encryption using parallel processing. It divides the key space into equal ranges and concurrently checks each range with multiprocessing. Upon finding a successful result, it prints the recovered key, decrypted text, and the time taken for the attack.

### 4. Generate Random Key Function (`generate_random_key`)

The `generate_random_key` function crafts a random 40-bit key (5 bytes) using Python's `secrets` module.

### 5. Usage Example

The `if __name__ == '__main__':` block demonstrates how to use the code. It generates a random 40-bit key, encrypts a sample plaintext using RC4, and simulates an attack to recover the key.

## Instructions

1. **Dependencies:**
    - Python 3.x
    - Ensure the required libraries are installed by running: `pip install -r requirements.txt`

2. **Run the Example:**
    - Execute the script by running: `python script.py`
    - This will generate a random 40-bit key, encrypt a sample plaintext using RC4, and attempt to recover the key through a parallelized brute-force attack.

3. **Customization:**
    - Modify the `plaintext_64chars` variable in the example usage for different plaintext inputs.
    - Adjust the `cpu_count()` parameter in the `simulate_attack` function to match the available CPU cores.

## Notes

- The example demonstrates the use of multiprocessing for improved performance in the brute-force attack.
- The code includes `freeze_support()` to ensure compatibility with Windows multiprocessing.

## Disclaimer

This code is educational and for demonstration purposes only. Brute-force attacks are not recommended for real-world scenarios, and secure key management practices should be followed for encryption.
