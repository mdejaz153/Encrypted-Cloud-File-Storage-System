#ifndef ENCRYPTION_H
#define ENCRYPTION_H

// Function to encrypt file content using XOR cipher
void xor_encrypt(char *data, int length, char *key);

// Function to decrypt file content using XOR cipher
void xor_decrypt(char *data, int length, char *key);

#endif
