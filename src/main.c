#include <stdio.h>
#include <stdlib.h>
#include "../include/cloud_operations.h"

int main() {
    int choice;
    
    // Create cloud_storage folder if not exists
    system("mkdir -p cloud_storage");

    // ===== USER LOGIN / SIGNUP =====
    login_page();
    
    // Simple ASCII banner (no special symbols)
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
