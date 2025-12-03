#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cloud.h"

// Make encrypted file name
void build_filename(int id, int version, char *out) {
    sprintf(out, CLOUD_FOLDER "%d_v%d.enc", id, version);
}

// Generate a unique 5-digit ID
int generate_file_id() {
    srand(time(NULL));
    return (rand() % 90000) + 10000;
}

// Read metadata and find next version
int get_next_version(int id) {
    FILE *fp = fopen(META_FILE, "r");
    if (!fp) return 1;

    int fid, ver, maxv = 0;
    char name[200];

    while (fscanf(fp, "%d %s %d", &fid, name, &ver) != EOF) {
        if (fid == id && ver > maxv) {
            maxv = ver;
        }
    }

    fclose(fp);
    return maxv + 1;
}

// Save file info to metadata
void save_metadata(int id, const char *name, int version) {
    FILE *fp = fopen(META_FILE, "a");
    if (!fp) return;
    fprintf(fp, "%d %s %d\n", id, name, version);
    fclose(fp);
}

// Check if encrypted file exists
int file_exists_cloud(int id, int version) {
    char fname[200];
    build_filename(id, version, fname);

    FILE *fp = fopen(fname, "rb");
    if (fp) {
        fclose(fp);
        return 1;
    }
    return 0;
}

// Delete encrypted file
int delete_file_cloud(int id, int version) {
    char fname[200];
    build_filename(id, version, fname);
    return remove(fname) == 0;
}

// Display all stored files
void list_files() {
    FILE *fp = fopen(META_FILE, "r");
    if (!fp) {
        printf("No files uploaded yet.\n");
        return;
    }

    int id, version;
    char name[200];

    printf("\nStored Files:\n");
    printf("ID        Name         Version\n");
    printf("----------------------------------\n");

    while (fscanf(fp, "%d %s %d", &id, name, &version) != EOF) {
        printf("%d     %s     v%d\n", id, name, version);
    }

    fclose(fp);
}
