#include "file_operations.h"
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>

FileInfo* get_file_info(const char* filename) {
    struct stat file_stat;
    
    if (stat(filename, &file_stat) == -1) {
        return NULL;
    }
    
    FileInfo* file_info = malloc(sizeof(FileInfo));
    if (!file_info) return NULL;
    
    // динамически выделяем память для имени файла
    file_info->filename = malloc(strlen(filename) + 1);
    if (!file_info->filename) {
        free(file_info);
        return NULL;
    }
    strcpy(file_info->filename, filename);
    
    file_info->perms = numeric_to_permissions(file_stat.st_mode);
    file_info->original_mode = file_stat.st_mode;
    
    return file_info;
}

void free_file_info(FileInfo* file_info) {
    if (file_info) {
        free(file_info->filename);
        free(file_info);
    }
}