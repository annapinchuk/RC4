#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define STATE_SIZE 256

void byteSwap(uint8_t *a, uint8_t *b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

void initializeRC4State(uint8_t state[STATE_SIZE], uint8_t key[], int keyLength) {
    int i, j = 0;
    for (i = 0; i < STATE_SIZE; i++) {
        state[i] = i;
    }
    for (i = 0; i < STATE_SIZE; i++) {
        j = (j + state[i] + key[i % keyLength]) % STATE_SIZE;
        byteSwap(&state[i], &state[j]);
    }
}

void encryptRC4(uint8_t state[STATE_SIZE], uint8_t plaintext[], uint8_t ciphertext[], int length) {
    int i = 0, j = 0, k;
    for (int n = 0; n < length; n++) {
        i = (i + 1) % STATE_SIZE;
        j = (j + state[i]) % STATE_SIZE;
        byteSwap(&state[i], &state[j]);
        k = state[(state[i] + state[j]) % STATE_SIZE];
        ciphertext[n] = plaintext[n] ^ k;
    }
}

void writeToFile(const char *filename, uint8_t data[], int length) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < length; i++) {
        fprintf(file, "%02X ", data[i]);
    }

    fclose(file);
}

int main() {
    clock_t startTime, endTime;

    // Generate a random key using the current time as the seed
    srand((unsigned int)time(NULL));
    uint8_t secretKey[4];
    for (int i = 0; i < 4; i++) {
        secretKey[i] = rand() % 256;
    }
    int keySize = ARRAY_SIZE(secretKey);

    printf("Encryption key: %02X%02X%02X%02X\n",
           secretKey[0], secretKey[1], secretKey[2], secretKey[3]);

    // Message with 16 characters
    uint8_t message[] = "This is a 64-character plaintext used for testing RC4 encryption.";
    int messageLength = ARRAY_SIZE(message) - 1; // excluding null terminator

    startTime = clock();  // Start measuring time

    // Initialize RC4 state
    uint8_t rc4State[STATE_SIZE];
    initializeRC4State(rc4State, secretKey, keySize);

    // Encrypt the message
    uint8_t encryptedMessage[messageLength];
    encryptRC4(rc4State, message, encryptedMessage, messageLength);

    endTime = clock();  // Stop measuring time

    // Print the encrypted message
    printf("Encrypted Message: ");
    for (int i = 0; i < messageLength; i++) {
        printf("%02X ", encryptedMessage[i]);
    }
    printf("\n");

    // Write the encrypted message to "secret.txt"
    writeToFile("secret.txt", encryptedMessage, messageLength);

    double elapsedTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    printf("Encryption Time: %f seconds\n", elapsedTime);

    return 0;
}
