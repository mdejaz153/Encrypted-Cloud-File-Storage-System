#include "../include/encryption.h"

// XOR Encryption Function
void xor_encrypt(char *data, int length, char *key) {
    int key_len = 0;
    
    // Calculate key length
    while(key[key_len] != '\0') {
        key_len++;
    }
    
    // Encrypt each byte with XOR
    for(int i = 0; i < length; i++) {
        data[i] = data[i] ^ key[i % key_len];
    }
}

// XOR Decryption Function (same as encryption for XOR)
void xor_decrypt(char *data, int length, char *key) {
    xor_encrypt(data, length, key);  // XOR is symmetric
}
