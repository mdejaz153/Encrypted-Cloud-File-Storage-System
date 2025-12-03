#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/encryption.h"
#include "../include/file_manager.h"
#include "../include/cloud_operations.h"

// Upload and encrypt file
void upload_file() {
    char filename[100], key[50], file_id[100];
    char encrypted_path[150];
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
    
    // Update metadata
    add_metadata(file_id, filename, 1);
    
    printf("\n✓ File uploaded successfully!\n");
    printf("File ID: %s\n", file_id);
    printf("Save this ID to download later!\n");
}

// Download and decrypt file
void download_file() {
    char file_id[100], key[50], encrypted_path[150];
    char output_name[100];
    FILE *input, *output;
    
    printf("\n=== DOWNLOAD FILE ===\n");
    printf("Enter File ID: ");
    scanf("%s", file_id);
    
    // Search file in metadata
    if(!search_metadata(file_id, encrypted_path)) {
        printf("Error: File ID not found!\n");
        return;
    }
    
    printf("Enter decryption key: ");
    scanf("%s", key);
    
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
    
    printf("\n✓ File downloaded and decrypted successfully!\n");
}

// Delete file from cloud
void delete_file() {
    char file_id[100], encrypted_path[150];
    
    printf("\n=== DELETE FILE ===\n");
    printf("Enter File ID to delete: ");
    scanf("%s", file_id);
    
    // Search file
    if(!search_metadata(file_id, encrypted_path)) {
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
    char file_id[100], original_name[100];
    int version;
    
    while(fgets(line, sizeof(line), meta)) {
        sscanf(line, "%[^|]|%[^|]|v%d", file_id, original_name, &version);
        printf("%-20s %-30s v%-9d\n", file_id, original_name, version);
    }
    
    fclose(meta);
}
