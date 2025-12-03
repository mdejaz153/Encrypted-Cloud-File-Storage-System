#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

// Generate unique file ID
void generate_file_id(char *file_id);

// Add metadata entry (user-specific)
void add_metadata(char *file_id, char *original_name, int version, char *key_hash, char *username);

// Search metadata (user-specific)
int search_metadata(char *file_id, char *encrypted_filename, char *stored_key_hash, char *username);

// Delete metadata (user-specific)
void delete_metadata(char *file_id, char *username);

// Generate hash of key
unsigned int generate_key_hash(char *key);

#endif
