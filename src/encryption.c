#include <stdio.h>
#include "encryption.h"

void encrypt_file(const char *input_path, const char *output_path, int key) {

    FILE *fin = fopen(input_path, "rb");
    if (!fin) {
        printf("Cannot open input file.\n");
        return;
    }

    FILE *fout = fopen(output_path, "wb");
    if (!fout) {
        printf("Cannot create encrypted file.\n");
        fclose(fin);
        return;
    }

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        fputc(ch ^ key, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("Encrypted & saved as: %s\n", output_path);
}

void decrypt_file(const char *input_path, const char *output_path, int key) {

    FILE *fin = fopen(input_path, "rb");
    if (!fin) {
        printf("Encrypted file not found.\n");
        return;
    }

    FILE *fout = fopen(output_path, "wb");
    if (!fout) {
        printf("Cannot create output file.\n");
        fclose(fin);
        return;
    }

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        fputc(ch ^ key, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("Decrypted & saved as: %s\n", output_path);
}
