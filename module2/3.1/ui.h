#ifndef UI_H
#define UI_H

#include "file_operations.h"

void display_permissions(FilePermissions perms, const char* label);
void display_file_info(FileInfo* file_info);
void display_menu(void);

void process_numeric_input(void);
void process_file_input(void);
void process_modification_input(void);

char* read_input(void);

#endif