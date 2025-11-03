#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <sys/stat.h>

// структура для прав доступа
typedef struct {
    unsigned char owner_read : 1;
    unsigned char owner_write : 1;
    unsigned char owner_execute : 1;
    unsigned char group_read : 1;
    unsigned char group_write : 1;
    unsigned char group_execute : 1;
    unsigned char others_read : 1;
    unsigned char others_write : 1;
    unsigned char others_execute : 1;
} FilePermissions;

// преобразования представлений
FilePermissions numeric_to_permissions(mode_t mode);
FilePermissions string_to_permissions(const char* str);
mode_t permissions_to_mode(FilePermissions perms);

// преобразования между форматами
char* permissions_to_symbolic(FilePermissions perms);
char* permissions_to_binary(FilePermissions perms);
int permissions_to_numeric(FilePermissions perms);

#endif