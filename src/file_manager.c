#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/file_manager.h"

// Generate unique file ID
void generate_file_id(char *file_id) {
    time_t t = time(NULL);
    sprintf(file_id, "FILE_%ld", t);
}

// Hash function for key
unsigned int generate_key_hash(char *key) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

// Add metadata entry (user-specific)
void add_metadata(char *file_id, char *original_name, int version, char *key_hash, char *username) {
    char meta_path[150];
    sprintf(meta_path, "cloud_storage/%s/metadata.txt", username);
    
    FILE *meta = fopen(meta_path, "a");
    if(meta == NULL) {
        printf("Error: Cannot create metadata!\n");
        return;
    }
    
    fprintf(meta, "%s|%s|v%d|%s\n", file_id, original_name, version, key_hash);
    fclose(meta);
}

// Search metadata (user-specific)
int search_metadata(char *file_id, char *encrypted_filename, char *stored_key_hash, char *username) {
    char meta_path[150];
    sprintf(meta_path, "cloud_storage/%s/metadata.txt", username);
    
    FILE *meta = fopen(meta_path, "r");
    if(meta == NULL) {
        return 0;
    }
    
    char line[256], stored_id[100], original_name[100], hash[50];
    int version;
    
    while(fgets(line, sizeof(line), meta)) {
        sscanf(line, "%[^|]|%[^|]|v%d|%s", stored_id, original_name, &version, hash);
        
        if(strcmp(stored_id, file_id) == 0) {
            sprintf(encrypted_filename, "cloud_storage/%s/%s_encrypted.bin", username, file_id);
            strcpy(stored_key_hash, hash);
            fclose(meta);
            return 1;
        }
    }
    
    fclose(meta);
    return 0;
}

// Delete metadata entry (user-specific)
void delete_metadata(char *file_id, char *username) {
    char meta_path[150], temp_path[150];
    sprintf(meta_path, "cloud_storage/%s/metadata.txt", username);
    sprintf(temp_path, "cloud_storage/%s/temp.txt", username);
    
    FILE *meta = fopen(meta_path, "r");
    FILE *temp = fopen(temp_path, "w");
    
    if(meta == NULL || temp == NULL) {
        printf("Error accessing metadata!\n");
        return;
    }
    
    char line[256], stored_id[100];
    
    while(fgets(line, sizeof(line), meta)) {
        sscanf(line, "%[^|]", stored_id);
        
        if(strcmp(stored_id, file_id) != 0) {
            fprintf(temp, "%s", line);
        }
    }
    
    fclose(meta);
    fclose(temp);
    
    remove(meta_path);
    rename(temp_path, meta_path);
}
