#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/encryption.h"
#include "../include/file_manager.h"
#include "../include/cloud_operations.h"

// Upload and encrypt file
void upload_file() {
    char filename[100], key[50], file_id[100];
    char encrypted_path[150], key_hash_str[50];
    FILE *input, *output;
    
    printf("\n=== UPLOAD FILE ===\n");
    printf("Enter filename to upload: ");
    scanf("%s", filename);
    
    printf("Enter encryption key: ");
    scanf("%s", key);
    
    // Open input file
    input = fopen(filename, "rb");
    if(input == NULL) {
        printf("Error: File not found!\n");
        return;
    }
    
    // Generate unique file ID
    generate_file_id(file_id);
    sprintf(encrypted_path, "cloud_storage/%s_encrypted.bin", file_id);
    
    // Read file content
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    char *buffer = (char*)malloc(file_size);
    fread(buffer, 1, file_size, input);
    fclose(input);
    
    // Encrypt content
    xor_encrypt(buffer, file_size, key);
    
    // Save encrypted file
    output = fopen(encrypted_path, "wb");
    fwrite(buffer, 1, file_size, output);
    fclose(output);
    
    free(buffer);
    
    // Generate key hash and update metadata
    unsigned int key_hash = generate_key_hash(key);
    sprintf(key_hash_str, "%u", key_hash);
    add_metadata(file_id, filename, 1, key_hash_str);
    
    printf("\n✓ File uploaded successfully!\n");
    printf("File ID: %s\n", file_id);
    printf("⚠ IMPORTANT: Save this ID and remember your key!\n");
}

// Download and decrypt file with key verification
void download_file() {
    char file_id[100], key[50], encrypted_path[150];
    char output_name[100], stored_key_hash[50], input_key_hash_str[50];
    FILE *input, *output;
    
    printf("\n=== DOWNLOAD FILE ===\n");
    printf("Enter File ID: ");
    scanf("%s", file_id);
    
    // Search file in metadata and get stored key hash
    if(!search_metadata(file_id, encrypted_path, stored_key_hash)) {
        printf("❌ Error: File ID not found!\n");
        return;
    }
    
    printf("Enter decryption key: ");
    scanf("%s", key);
    
    // Verify key by comparing hashes
    unsigned int input_key_hash = generate_key_hash(key);
    sprintf(input_key_hash_str, "%u", input_key_hash);
    
    if(strcmp(stored_key_hash, input_key_hash_str) != 0) {
        printf("❌ Error: Incorrect decryption key!\n");
        printf("Access denied. Please use the correct key.\n");
        return;
    }
    
    // Open encrypted file
    input = fopen(encrypted_path, "rb");
    if(input == NULL) {
        printf("Error: Encrypted file missing!\n");
        return;
    }
    
    // Read encrypted content
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    char *buffer = (char*)malloc(file_size);
    fread(buffer, 1, file_size, input);
    fclose(input);
    
    // Decrypt content
    xor_decrypt(buffer, file_size, key);
    
    // Save decrypted file
    printf("Enter output filename: ");
    scanf("%s", output_name);
    
    output = fopen(output_name, "wb");
    fwrite(buffer, 1, file_size, output);
    fclose(output);
    
    free(buffer);
    
    printf("\n✅ File downloaded and decrypted successfully!\n");
}

// Delete file from cloud
void delete_file() {
    char file_id[100], encrypted_path[150], dummy_hash[50];
    
    printf("\n=== DELETE FILE ===\n");
    printf("Enter File ID to delete: ");
    scanf("%s", file_id);
    
    // Search file
    if(!search_metadata(file_id, encrypted_path, dummy_hash)) {
        printf("Error: File ID not found!\n");
        return;
    }
    
    // Delete encrypted file
    if(remove(encrypted_path) == 0) {
        delete_metadata(file_id);
        printf("\n✓ File deleted successfully!\n");
    } else {
        printf("Error: Cannot delete file!\n");
    }
}

// List all files in cloud storage
void list_files() {
    FILE *meta = fopen("cloud_storage/metadata.txt", "r");
    
    if(meta == NULL) {
        printf("\nNo files in cloud storage!\n");
        return;
    }
    
    printf("\n=== FILES IN CLOUD STORAGE ===\n");
    printf("%-20s %-30s %-10s\n", "File ID", "Original Name", "Version");
    printf("-----------------------------------------------------------\n");
    
    char line[256];
    char file_id[100], original_name[100], hash[50];
    int version;
    
    while(fgets(line, sizeof(line), meta)) {
        // Updated to read hash as well (but not display it)
        sscanf(line, "%[^|]|%[^|]|v%d|%s", file_id, original_name, &version, hash);
        printf("%-20s %-30s v%-9d\n", file_id, original_name, version);
    }
    
    fclose(meta);
}
