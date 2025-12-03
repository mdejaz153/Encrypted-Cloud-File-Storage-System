#include <stdio.h>
#include "encryption.h"
#include "cloud.h"

int main() {

    int choice;

    printf("\n=== Encrypted Cloud File Storage System ===\n");

    while (1) {

        printf("\n1. Upload New File");
        printf("\n2. Upload New Version");
        printf("\n3. Decrypt File");
        printf("\n4. Delete File");
        printf("\n5. List Files");
        printf("\n0. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Exiting...\n");
            break;
        }

        if (choice == 1) {
            char in[200], name[200], out[200];
            int key;

            printf("Enter input file path: ");
            scanf("%s", in);

            printf("Enter display name: ");
            scanf("%s", name);

            printf("Enter encryption key: ");
            scanf("%d", &key);

            int id = generate_file_id();
            int v = 1;

            build_filename(id, v, out);
            encrypt_file(in, out, key);
            save_metadata(id, name, v);

            printf("Uploaded → ID: %d  Version: v1\n", id);
        }

        else if (choice == 2) {
            int id, key;
            char in[200], out[200], name[200]="same";

            printf("Enter File ID: ");
            scanf("%d", &id);

            printf("Enter input file path: ");
            scanf("%s", in);

            printf("Enter key: ");
            scanf("%d", &key);

            int v = get_next_version(id);
            build_filename(id, v, out);

            encrypt_file(in, out, key);
            save_metadata(id, name, v);

            printf("New Version Uploaded: v%d\n", v);
        }

        else if (choice == 3) {
            int id, v, key;
            char out[200], in[200];

            printf("Enter File ID: ");
            scanf("%d", &id);

            printf("Enter Version: ");
            scanf("%d", &v);

            printf("Enter key: ");
            scanf("%d", &key);

            if (!file_exists_cloud(id, v)) {
                printf("File not found.\n");
                continue;
            }

            build_filename(id, v, in);

            printf("Enter output file name: ");
            scanf("%s", out);

            decrypt_file(in, out, key);
        }

        else if (choice == 4) {
            int id, v;

            printf("Enter File ID: ");
            scanf("%d", &id);

            printf("Enter Version: ");
            scanf("%d", &v);

            if (delete_file_cloud(id, v))
                printf("Deleted.\n");
            else
                printf("Could not delete.\n");
        }

        else if (choice == 5) {
            list_files();
        }

        else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
