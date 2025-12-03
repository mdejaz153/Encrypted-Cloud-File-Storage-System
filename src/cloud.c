#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cloud.h"

int initialize_cloud(void) {
    // Just try to open metadata file in append mode.
    // Assumes directory "cloud_storage" already exists.
    FILE *f = fopen(METADATA_FILE, "a+");
    if (f == NULL) {
        printf("Error: Could not open metadata file.\n");
        printf("Make sure folder '%s' exists in your project directory.\n", CLOUD_DIR);
        return 0; // failure
    }
    fclose(f);
    return 1; // success
}

int generate_file_id(void) {
    // Generate pseudo-random 5-digit ID
    // (simple, collisions unlikely for small usage)
    srand
