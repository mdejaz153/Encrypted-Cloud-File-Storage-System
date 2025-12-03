#include <stdio.h>
#include "encryption.h"
#include "cloud.h"

int main() {

    int choice;

    while (1) {

        printf("\n=== Encrypted Cloud Storage ===\n");
        printf("1. Upload New File\n");
        printf("2. Upload New Version\n");
        printf("3. Decrypt File\n");
        printf("4. Delete File\n");
        printf("5. List Files\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            break;
        }

        if (choice == 1) {
            char in[200], name[200], out[200];
            int key;

            printf("Enter input file path: ");
            scanf("%s", in);

            printf("Enter display name: ");
            scanf("%s", name);

            printf("Enter key: ");
            scanf("%d", &key);

            int id = generate_file_id();
            int v = 1;

            build_filename(id, v, out);
            encrypt_file(in, out, key);
            save_metadata(id, name, v);

            printf("Upload Success → ID: %d  Version: v1\n", id);
        }

        else if (choice == 2) {
            int id, key;
            char in[200], out[200], name[200] = "same";

            printf("Enter file ID: ");
            scanf("%d", &id);

            printf("Enter new file path: ");
            scanf("%s", in);

            printf("Enter key: ");
            scanf("%d", &key);

            int v = get_next_version(id);
            build_filename(id, v, out);

            encrypt_file(in, out, key);
            save_metadata(id, name, v);

            printf("New Version Uploaded → v%d\n", v);
        }

        else if (choice == 3) {
            int id, v, key;
            char in[200], out[200];

            printf("Enter file ID: ");
            scanf("%d", &id);

            printf("Enter version: ");
            scanf("%d", &v);

            printf("Enter key: ");
            scanf("%d", &key);

            if (!file_exists_cloud(id, v)) {
                printf("File not found.\n");
                continue;
            }

            build_filename(id, v, in);

            printf("Save decrypted as: ");
            scanf("%s", out);

            decrypt_file(in, out, key);
        }

        else if (choice == 4) {
            int id, v;

            printf("Enter file ID: ");
            scanf("%d", &id);

            printf("Enter version: ");
            scanf("%d", &v);

            if (delete_file_cloud(id, v))
                printf("File deleted.\n");
            else
                printf("Delete failed.\n");
        }

        else if (choice == 5) {
            list_files();
        }

        else {
            printf("Invalid option.\n");
        }
    }

    return 0;
}
