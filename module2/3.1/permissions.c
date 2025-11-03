#include "permissions.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

FilePermissions numeric_to_permissions(mode_t mode) {
    FilePermissions perms = {0};
    
    perms.owner_read = (mode & S_IRUSR) ? 1 : 0;
    perms.owner_write = (mode & S_IWUSR) ? 1 : 0;
    perms.owner_execute = (mode & S_IXUSR) ? 1 : 0;
    
    perms.group_read = (mode & S_IRGRP) ? 1 : 0;
    perms.group_write = (mode & S_IWGRP) ? 1 : 0;
    perms.group_execute = (mode & S_IXGRP) ? 1 : 0;
    
    perms.others_read = (mode & S_IROTH) ? 1 : 0;
    perms.others_write = (mode & S_IWOTH) ? 1 : 0;
    perms.others_execute = (mode & S_IXOTH) ? 1 : 0;
    
    return perms;
}

FilePermissions string_to_permissions(const char* str) {
    FilePermissions perms = {0};
    
    if (strlen(str) == 3 && isdigit(str[0]) && isdigit(str[1]) && isdigit(str[2])) {
        
        int owner = str[0] - '0';
        int group = str[1] - '0';
        int others = str[2] - '0';
        
        perms.owner_read = (owner & 4) ? 1 : 0;
        perms.owner_write = (owner & 2) ? 1 : 0;
        perms.owner_execute = (owner & 1) ? 1 : 0;
        
        perms.group_read = (group & 4) ? 1 : 0;
        perms.group_write = (group & 2) ? 1 : 0;
        perms.group_execute = (group & 1) ? 1 : 0;
        
        perms.others_read = (others & 4) ? 1 : 0;
        perms.others_write = (others & 2) ? 1 : 0;
        perms.others_execute = (others & 1) ? 1 : 0;

    } else if (strlen(str) == 9) {

        perms.owner_read = (str[0] == 'r') ? 1 : 0;
        perms.owner_write = (str[1] == 'w') ? 1 : 0;
        perms.owner_execute = (str[2] == 'x') ? 1 : 0;
        
        perms.group_read = (str[3] == 'r') ? 1 : 0;
        perms.group_write = (str[4] == 'w') ? 1 : 0;
        perms.group_execute = (str[5] == 'x') ? 1 : 0;
        
        perms.others_read = (str[6] == 'r') ? 1 : 0;
        perms.others_write = (str[7] == 'w') ? 1 : 0;
        perms.others_execute = (str[8] == 'x') ? 1 : 0;
    }
    
    return perms;
}

mode_t permissions_to_mode(FilePermissions perms) {
    mode_t mode = 0;
    
    if (perms.owner_read) mode |= S_IRUSR;
    if (perms.owner_write) mode |= S_IWUSR;
    if (perms.owner_execute) mode |= S_IXUSR;
    
    if (perms.group_read) mode |= S_IRGRP;
    if (perms.group_write) mode |= S_IWGRP;
    if (perms.group_execute) mode |= S_IXGRP;
    
    if (perms.others_read) mode |= S_IROTH;
    if (perms.others_write) mode |= S_IWOTH;
    if (perms.others_execute) mode |= S_IXOTH;
    
    return mode;
}

char* permissions_to_symbolic(FilePermissions perms) {
    char* symbolic = malloc(10 * sizeof(char));
    if (!symbolic) return NULL;
    
    symbolic[0] = perms.owner_read ? 'r' : '-';
    symbolic[1] = perms.owner_write ? 'w' : '-';
    symbolic[2] = perms.owner_execute ? 'x' : '-';
    
    symbolic[3] = perms.group_read ? 'r' : '-';
    symbolic[4] = perms.group_write ? 'w' : '-';
    symbolic[5] = perms.group_execute ? 'x' : '-';
    
    symbolic[6] = perms.others_read ? 'r' : '-';
    symbolic[7] = perms.others_write ? 'w' : '-';
    symbolic[8] = perms.others_execute ? 'x' : '-';
    symbolic[9] = '\0';
    
    return symbolic;
}

char* permissions_to_binary(FilePermissions perms) {
    char* binary = malloc(10 * sizeof(char));
    if (!binary) return NULL;
    
    binary[0] = perms.owner_read ? '1' : '0';
    binary[1] = perms.owner_write ? '1' : '0';
    binary[2] = perms.owner_execute ? '1' : '0';
    
    binary[3] = perms.group_read ? '1' : '0';
    binary[4] = perms.group_write ? '1' : '0';
    binary[5] = perms.group_execute ? '1' : '0';
    
    binary[6] = perms.others_read ? '1' : '0';
    binary[7] = perms.others_write ? '1' : '0';
    binary[8] = perms.others_execute ? '1' : '0';
    binary[9] = '\0';
    
    return binary;
}

int permissions_to_numeric(FilePermissions perms) {
    int owner = perms.owner_read * 4 + perms.owner_write * 2 + perms.owner_execute;
    int group = perms.group_read * 4 + perms.group_write * 2 + perms.group_execute;
    int others = perms.others_read * 4 + perms.others_write * 2 + perms.others_execute;
    
    return owner * 100 + group * 10 + others;
}