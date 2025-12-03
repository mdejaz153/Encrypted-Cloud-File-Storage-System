#include <stdio.h>
#include <stdlib.h>
#include "encryption.h"

/*
 * Simple XOR-based encryption and decryption.
 * Same function logic is used for both encrypt and decrypt
 * (encrypting twice with same key gives original data).
 */

void encrypt_file(const char *input_path, const char *output_path, int key) {
    FILE *fin = fopen(input_path, "rb");
    if (fin == NULL) {
        printf("Error: Cannot open input file: %s\n", input_path);
        return;
    }

    FILE *fout = fopen(output_path, "wb");
    if (fout == NULL) {
        printf("Error: Cannot create output file: %s\n", output_path);
        fclose(fin);
        return;
    }

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        unsigned char original = (unsigned char)ch;
        unsigned char enc = (unsigned char)(original ^ key);
        fputc(enc, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("Encryption completed. Saved to: %s\n", output_path);
}

void decrypt_file(const char *input_path, const char *output_path, int key) {
    FILE *fin = fopen(input_path, "rb");
    if (fin == NULL) {
        printf("Error: Cannot open encrypted file: %s\n", input_path);
        return;
    }

    FILE *fout = fopen(output_path, "wb");
    if (fout == NULL) {
        printf("Error: Cannot create output file: %s\n", output_path);
        fclose(fin);
        return;
    }

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        unsigned char enc = (unsigned char)ch;
        unsigned char dec = (unsigned char)(enc ^ key);
        fputc(dec, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("Decryption completed. Saved to: %s\n", output_path);
}
