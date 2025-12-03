#include <stdio.h>
#include <stdlib.h>
#include "../include/cloud_operations.h"

#ifdef _WIN32
    #include <direct.h>  // For _mkdir on Windows
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

// Cross-platform directory creation
void create_directory(const char *path) {
    #ifdef _WIN32
        _mkdir(path);
    #else
        mkdir(path, 0777);
    #endif
}

int main() {
    int choice;
    
    // Create cloud_storage folder if not exists
    create_directory("cloud_storage");

    // User login/signup
    login_page();
    
    // Main menu banner
    printf("\n============================================\n");
    printf("   ENCRYPTED CLOUD FILE STORAGE SYSTEM\n");
    printf("============================================\n");
    
    while(1) {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Upload File\n");
        printf("2. Download File\n");
        printf("3. Delete File\n");
        printf("4. List All Files\n");
        printf("5. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                upload_file();
                break;
            case 2:
                download_file();
                break;
            case 3:
                delete_file();
                break;
            case 4:
                list_files();
                break;
            case 5:
                printf("\nThank you for using Cloud Storage!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    }
    
    return 0;
}
