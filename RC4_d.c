#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define RC4_STATE_SIZE 256

void byteSwap(uint8_t *a, uint8_t *b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

void initializeRC4State(uint8_t state[RC4_STATE_SIZE], uint8_t key[], int keyLength) {
    int i, j = 0;
    for (i = 0; i < RC4_STATE_SIZE; i++) {
        state[i] = i;
    }
    for (i = 0; i < RC4_STATE_SIZE; i++) {
        j = (j + state[i] + key[i % keyLength]) % RC4_STATE_SIZE;
        byteSwap(&state[i], &state[j]);
    }
}

void decryptRC4(uint8_t state[RC4_STATE_SIZE], uint8_t ciphertext[], uint8_t plaintext[], int length) {
    int i = 0, j = 0, k;
    for (int n = 0; n < length; n++) {
        i = (i + 1) % RC4_STATE_SIZE;
        j = (j + state[i]) % RC4_STATE_SIZE;
        byteSwap(&state[i], &state[j]);
        k = state[(state[i] + state[j]) % RC4_STATE_SIZE];
        plaintext[n] = ciphertext[n] ^ k;
    }
}

int main() {
    clock_t startTime, endTime;

    FILE *file = fopen("secret.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read ciphertext from file
    uint8_t ciphertext[1000]; // Assuming ciphertext length won't exceed 1000 bytes
    size_t len = 0;
    int value;
    while (fscanf(file, "%x", &value) == 1) {
        ciphertext[len++] = (uint8_t)value;
    }
    fclose(file);

    // Expected plaintext
    uint8_t expectedPlaintext[] = "This is a 64-character plaintext used for testing RC4 encryption.";
    
    startTime = clock();  // Start measuring time

    // Brute-force attack on 32-bit keys
    for (uint32_t candidateKey = 0; candidateKey <= UINT32_MAX; candidateKey++) {
        // Convert key to uint8_t array
        uint8_t keyBytes[4];
        keyBytes[0] = (candidateKey >> 24) & 0xFF;
        keyBytes[1] = (candidateKey >> 16) & 0xFF;
        keyBytes[2] = (candidateKey >> 8) & 0xFF;
        keyBytes[3] = candidateKey & 0xFF;

        // Initialize RC4 state array
        uint8_t rc4State[RC4_STATE_SIZE];
        initializeRC4State(rc4State, keyBytes, sizeof(keyBytes));

        // Decrypt the ciphertext
        uint8_t decryptedText[len];
        decryptRC4(rc4State, ciphertext, decryptedText, len);

        // Check if decrypted plaintext matches expected plaintext
        if (memcmp(decryptedText, expectedPlaintext, len) == 0) {
            endTime = clock();  // Stop measuring time

            // Print success if match found
            printf("Key found: %02X%02X%02X%02X\n", keyBytes[0], keyBytes[1], keyBytes[2], keyBytes[3]);

            double elapsedTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
            printf("Time taken: %f seconds\n", elapsedTime);
            return 0;
        }
    }

    endTime = clock();  // Stop measuring time
    double elapsedTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    // Print failure if no match found
    printf("Decryption failed.\n");
    printf("Time taken: %f seconds\n", elapsedTime);

    return 0;
}
