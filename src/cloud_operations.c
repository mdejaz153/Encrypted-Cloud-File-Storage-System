#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/encryption.h"
#include "../include/file_manager.h"
#include "../include/cloud_operations.h"

// ================== GLOBAL USER STATE ==================
static char current_username[50] = "";
static unsigned int current_password_hash = 0;

// ===== Helper functions =====
static void create_account();
static int login_existing_user();
static int verify_user_password_for_delete();
static void get_user_folder_path(char *path);

// ================== GET USER FOLDER PATH ==================
static void get_user_folder_path(char *path) {
    sprintf(path, "cloud_storage/%s", current_username);
}

// ================== LOGIN / SIGNUP PAGE ==================
void login_page() {
    int choice;
    system("mkdir -p users");
    system("mkdir -p cloud_storage");

    printf("\n============================================\n");
    printf("     ENCRYPTED CLOUD STORAGE SYSTEM\n");
    printf("============================================\n");

    while (1) {
        printf("\n1. Login\n");
        printf("2. Create New Account\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (login_existing_user()) {
                    printf("\nWelcome, %s!\n", current_username);
                    return;
                }
                break;
            case 2:
                create_account();
                break;
            case 3:
                printf("\nGoodbye!\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
}

// ================== CREATE ACCOUNT ==================
static void create_account() {
    char username[50], password[50], path[100];
    FILE *f;

    printf("\n--- Create Account ---\n");
    printf("Username: ");
    scanf("%s", username);

    sprintf(path, "users/%s.txt", username);

    // Check if user exists
    f = fopen(path, "r");
    if (f != NULL) {
        printf("Username already exists!\n");
        fclose(f);
        return;
    }

    printf("Password: ");
    scanf("%s", password);

    unsigned int pass_hash = generate_key_hash(password);

    f = fopen(path, "w");
    if (f == NULL) {
        printf("Error creating account.\n");
        return;
    }

    fprintf(f, "%u\n", pass_hash);
    fclose(f);

    printf("Account created! Please login.\n");
}

// ================== LOGIN ==================
static int login_existing_user() {
    char username[50], password[50], path[100], user_folder[100];
    unsigned int stored_hash;
    FILE *f;

    printf("\n--- Login ---\n");
    printf("Username: ");
    scanf("%s", username);

    sprintf(path, "users/%s.txt", username);

    f = fopen(path, "r");
    if (f == NULL) {
        printf("User not found.\n");
        return 0;
    }

    if (fscanf(f, "%u", &stored_hash) != 1) {
        printf("User data corrupted.\n");
        fclose(f);
        return 0;
    }
    fclose(f);

    printf("Password: ");
    scanf("%s", password);

    unsigned int input_hash = generate_key_hash(password);

    if (input_hash != stored_hash) {
        printf("Incorrect password.\n");
        return 0;
    }

    // Set current user
    strcpy(current_username, username);
    current_password_hash = stored_hash;

    // Create user's personal folder
    sprintf(user_folder, "cloud_storage/%s", username);
    char cmd[150];
    sprintf(cmd, "mkdir -p %s", user_folder);
    system(cmd);

    return 1;
}

// ================== VERIFY PASSWORD ==================
static int verify_user_password_for_delete() {
    char password[50];

    printf("Enter password to confirm: ");
    scanf("%s", password);

    unsigned int input_hash = generate_key_hash(password);

    if (input_hash != current_password_hash) {
        printf("Incorrect password.\n");
        return 0;
    }
    return 1;
}

// ================== UPLOAD FILE ==================
void upload_file() {
    char filename[100], key[50], file_id[100];
    char encrypted_path[200], key_hash_str[50], user_folder[100];
    FILE *input, *output;
    
    printf("\n--- Upload File ---\n");
    printf("Filename: ");
    scanf("%s", filename);
    
    input = fopen(filename, "rb");
    if(input == NULL) {
        printf("File not found!\n");
        return;
    }
    
    printf("Encryption key: ");
    scanf("%s", key);
    
    // Generate file ID
    generate_file_id(file_id);
    
    // User-specific path
    get_user_folder_path(user_folder);
    sprintf(encrypted_path, "%s/%s_encrypted.bin", user_folder, file_id);
    
    // Read file
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    char *buffer = (char*)malloc(file_size);
    fread(buffer, 1, file_size, input);
    fclose(input);
    
    // Encrypt
    xor_encrypt(buffer, file_size, key);
    
    // Save
    output = fopen(encrypted_path, "wb");
    fwrite(buffer, 1, file_size, output);
    fclose(output);
    free(buffer);
    
    // Update metadata
    unsigned int key_hash = generate_key_hash(key);
    sprintf(key_hash_str, "%u", key_hash);
    add_metadata(file_id, filename, 1, key_hash_str, current_username);
    
    printf("\nFile uploaded!\n");
    printf("File ID: %s\n", file_id);
    printf("Remember your ID and key!\n");
}

// ================== DOWNLOAD FILE ==================
void download_file() {
    char file_id[100], key[50], encrypted_path[200];
    char output_name[100], stored_key_hash[50], input_key_hash_str[50];
    FILE *input, *output;
    
    printf("\n--- Download File ---\n");
    printf("File ID: ");
    scanf("%s", file_id);
    
    // Search in user's files only
    if(!search_metadata(file_id, encrypted_path, stored_key_hash, current_username)) {
        printf("File not found!\n");
        return;
    }
    
    printf("Decryption key: ");
    scanf("%s", key);
    
    // Verify key
    unsigned int input_key_hash = generate_key_hash(key);
    sprintf(input_key_hash_str, "%u", input_key_hash);
    
    if(strcmp(stored_key_hash, input_key_hash_str) != 0) {
        printf("Wrong key!\n");
        return;
    }
    
    // Open encrypted file
    input = fopen(encrypted_path, "rb");
    if(input == NULL) {
        printf("File missing!\n");
        return;
    }
    
    // Read and decrypt
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    char *buffer = (char*)malloc(file_size);
    fread(buffer, 1, file_size, input);
    fclose(input);
    
    xor_decrypt(buffer, file_size, key);
    
    // Save
    printf("Output filename: ");
    scanf("%s", output_name);
    
    output = fopen(output_name, "wb");
    fwrite(buffer, 1, file_size, output);
    fclose(output);
    free(buffer);
    
    printf("\nFile downloaded!\n");
}

// ================== DELETE FILE ==================
void delete_file() {
    char file_id[100], encrypted_path[200], dummy_hash[50];
    
    printf("\n--- Delete File ---\n");
    printf("File ID: ");
    scanf("%s", file_id);
    
    // Search in user's files
    if(!search_metadata(file_id, encrypted_path, dummy_hash, current_username)) {
        printf("File not found!\n");
        return;
    }

    // Verify password
    if (!verify_user_password_for_delete()) {
        return;
    }
    
    // Delete
    if(remove(encrypted_path) == 0) {
        delete_metadata(file_id, current_username);
        printf("\nFile deleted!\n");
    } else {
        printf("Cannot delete file!\n");
    }
}

// ================== LIST FILES ==================
void list_files() {
    char meta_path[150], user_folder[100];
    
    get_user_folder_path(user_folder);
    sprintf(meta_path, "%s/metadata.txt", user_folder);
    
    FILE *meta = fopen(meta_path, "r");
    
    if(meta == NULL) {
        printf("\nNo files uploaded yet!\n");
        return;
    }
    
    printf("\n--- Your Files ---\n");
    printf("%-20s %-30s\n", "File ID", "Original Name");
    printf("--------------------------------------------------\n");
    
    char line[256], file_id[100], original_name[100], hash[50];
    int version;
    
    while(fgets(line, sizeof(line), meta)) {
        sscanf(line, "%[^|]|%[^|]|v%d|%s", file_id, original_name, &version, hash);
        printf("%-20s %-30s\n", file_id, original_name);
    }
    
    fclose(meta);
}
