#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/file_manager.h"

// Generate unique file ID using timestamp
void generate_file_id(char *file_id) {
    time_t t = time(NULL);
    sprintf(file_id, "FILE_%ld", t);
}

// Simple hash function for key verification
unsigned int generate_key_hash(char *key) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash;
}

// Add file entry to metadata with key hash
void add_metadata(char *file_id, char *original_name, int version, char *key_hash) {
    FILE *meta = fopen("cloud_storage/metadata.txt", "a");
    if(meta == NULL) {
        printf("Error: Cannot create metadata file!\n");
        return;
    }
    
    fprintf(meta, "%s|%s|v%d|%s\n", file_id, original_name, version, key_hash);
    fclose(meta);
    printf("Metadata updated successfully!\n");
}

// Search file ID in metadata and return key hash
int search_metadata(char *file_id, char *encrypted_filename, char *stored_key_hash) {
    FILE *meta = fopen("cloud_storage/metadata.txt", "r");
    if(meta == NULL) {
        return 0;  // File not found
    }
    
    char line[256];
    char stored_id[100], original_name[100], hash[50];
    int version;
    
    while(fgets(line, sizeof(line), meta)) {
        sscanf(line, "%[^|]|%[^|]|v%d|%s", stored_id, original_name, &version, hash);
        
        if(strcmp(stored_id, file_id) == 0) {
            sprintf(encrypted_filename, "cloud_storage/%s_encrypted.bin", file_id);
            strcpy(stored_key_hash, hash);
            fclose(meta);
            return 1;  // Found
        }
    }
    
    fclose(meta);
    return 0;  // Not found
}

// Delete file entry from metadata
void delete_metadata(char *file_id) {
    FILE *meta = fopen("cloud_storage/metadata.txt", "r");
    FILE *temp = fopen("cloud_storage/temp.txt", "w");
    
    if(meta == NULL || temp == NULL) {
        printf("Error accessing metadata!\n");
        return;
    }
    
    char line[256];
    char stored_id[100];
    
    while(fgets(line, sizeof(line), meta)) {
        sscanf(line, "%[^|]", stored_id);
        
        if(strcmp(stored_id, file_id) != 0) {
            fprintf(temp, "%s", line);
        }
    }
    
    fclose(meta);
    fclose(temp);
    
    remove("cloud_storage/metadata.txt");
    rename("cloud_storage/temp.txt", "cloud_storage/metadata.txt");
}
