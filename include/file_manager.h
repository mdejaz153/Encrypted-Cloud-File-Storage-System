#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

// Function to generate unique file ID
void generate_file_id(char *file_id);

// Function to add entry in metadata file
void add_metadata(char *file_id, char *original_name, int version);

// Function to search file in metadata
int search_metadata(char *file_id, char *encrypted_filename);

// Function to delete entry from metadata
void delete_metadata(char *file_id);

#endif
