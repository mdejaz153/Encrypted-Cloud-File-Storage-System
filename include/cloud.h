#ifndef CLOUD_H
#define CLOUD_H

#include <stddef.h>

#define CLOUD_DIR "cloud_storage"
#define METADATA_FILE "cloud_storage/metadata.txt"
#define MAX_FILENAME 256

typedef struct {
    int file_id;
    char original_name[MAX_FILENAME];
    int version;
} FileRecord;

// Cloud initialization
int initialize_cloud(void);

// File ID and version handling
int generate_file_id(void);
int get_next_version(int file_id, char *original_name_out, size_t name_size);

// Metadata handling
int save_file_record(int file_id, const char *original_name, int version);

// Utility functions
void build_encrypted_filename(int file_id, int version, char *out, size_t size);
int encrypted_file_exists(int file_id, int version);
int delete_encrypted_file(int file_id, int version);
void list_files(void);

#endif // CLOUD_H
