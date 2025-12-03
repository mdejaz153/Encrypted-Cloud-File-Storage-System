#ifndef CLOUD_H
#define CLOUD_H

#define CLOUD_FOLDER "cloud_storage/"
#define META_FILE "cloud_storage/metadata.txt"

void build_filename(int id, int version, char *out);
int generate_file_id();
int get_next_version(int id);
void save_metadata(int id, const char *name, int version);
void list_files();
int file_exists_cloud(int id, int version);
int delete_file_cloud(int id, int version);

#endif
