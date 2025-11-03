#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "permissions.h"

// структура для хранения информации о файле
typedef struct {
    char* filename;
    FilePermissions perms;
    mode_t original_mode;
} FileInfo;

// для работы с файлами
FileInfo* get_file_info(const char* filename);
void free_file_info(FileInfo* file_info);

#endif