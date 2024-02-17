import time
from multiprocessing import Pool, cpu_count , freeze_support
import secrets

# RC4 encryption algorithm
def rc4(key, plaintext):
    S = list(range(256)) # Initialize S-box
    j = 0 # Key-scheduling algorithm
    for i in range(256): # Permutation of S-box
        j = (j + S[i] + key[i % len(key)]) % 256 # Modulo 256
        S[i], S[j] = S[j], S[i] # Swap S[i] and S[j]

    i = 0 # Pseudo-random generation algorithm
    j = 0 # Stream generation
    ciphertext = [] # Encryption
    for char in plaintext: # XOR with plaintext
        i = (i + 1) % 256 # Modulo 256
        j = (j + S[i]) % 256 # Modulo 256
        S[i], S[j] = S[j], S[i] # Swap S[i] and S[j]
        k = S[(S[i] + S[j]) % 256] # Modulo 256
        ciphertext.append(chr(ord(char) ^ k)) # XOR with k
    return ''.join(ciphertext) # Return ciphertext

# checking if the key is correct
def check_key(candidate_range, encrypted_text):
    known_plaintext = "This is a 64-character plaintext used for testing RC4 encryption."
    known_first_word = known_plaintext.split()[0]
    
    for key_candidate in candidate_range:
        key = [(key_candidate >> shift) & 0xFF for shift in (32, 24, 16, 8, 0)] # Convert 40-bit key to 5 bytes 
        print(f"Checking key candidate: {key}")
        decrypted_text = rc4(key, encrypted_text)
        
        # Get the first word from the decrypted text
        decrypted_first_word = decrypted_text.split()[0]
        
        # Compare the known first word with the decrypted first word
        if known_plaintext in decrypted_text:
            return key, decrypted_text
        
        # # Compare only a segment of the decrypted text with the known plaintext
        # if known_plaintext[:20] in decrypted_text:
        #     return key, decrypted_text
    return None, None
        
# Simulating an attack on RC4 using brute force with parallel processing
def simulate_attack(encrypted_text):
    start_time = time.time()
    attack_successful = False
    # Split the key space into equal ranges for parallel processing
    candidate_ranges = [(2**40 * i // cpu_count(), 2**40 * (i + 1) // cpu_count()) for i in range(cpu_count())]

    while not attack_successful:
        with Pool(cpu_count()) as pool:
            results = pool.starmap(check_key, [(range(*candidate_range), encrypted_text) for candidate_range in candidate_ranges])

        for result in results:
            if result:
                elapsed_time = time.time() - start_time
                print("\nAttack Successful:")
                print("Recovered Key:", result[0])
                print("Recovered Text:", result[1])
                print("Time taken for the attack:", elapsed_time, "seconds")
                attack_successful = True
                return result[0], result[1], elapsed_time
            
    # If the loop completes without finding the key, return None
    return None, None, None


def generate_random_key():
    # Generate a random 40-bit key (5 bytes)
    random_key = [secrets.randbits(8) for _ in range(5)] # 40-bit key (5 bytes) 
    return random_key

if __name__ == '__main__':
    freeze_support()  # Add freeze_support() for Windows compatibility

    # Example usage
    random_key_40bit = generate_random_key()
    print("Random 40-bit Key:", random_key_40bit)
    plaintext_64chars = "This is a 64-character plaintext used for testing RC4 encryption."

    encrypted_text = rc4(random_key_40bit, plaintext_64chars)
    print("Encrypted: ", encrypted_text)

    # Simulate an attack
    simulate_attack(encrypted_text)    
        
    # # Example usage with a specific key for testing
    # key_for_testing = [0, 0, 0, 0, 0]
    # print("Testing with Key:", key_for_testing)
    # plaintext_64chars = "This is a 64-character plaintext used for testing RC4 encryption."

    # encrypted_text = rc4(key_for_testing, plaintext_64chars)
    # print("Encrypted:", encrypted_text)

    # # Simulate an attack with the known key
    # simulate_attack(encrypted_text)