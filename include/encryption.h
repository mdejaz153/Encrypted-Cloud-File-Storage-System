#ifndef ENCRYPTION_H
#define ENCRYPTION_H

void encrypt_file(const char *input_path, const char *output_path, int key);
void decrypt_file(const char *input_path, const char *output_path, int key);

#endif
