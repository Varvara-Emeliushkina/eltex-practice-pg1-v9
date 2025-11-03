#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_permissions(FilePermissions perms, const char* label) {
    char* symbolic = permissions_to_symbolic(perms);
    char* binary = permissions_to_binary(perms);
    int numeric = permissions_to_numeric(perms);
    
    if (!symbolic || !binary) {
        printf("Ошибка выделения памяти\n");
        free(symbolic);
        free(binary);
        return;
    }
    
    if (label && label[0] != '\0') {
        printf("=== %s ===\n", label);
    }
    printf("Буквенное представление: %s\n", symbolic);
    printf("Цифровое представление: %03d\n", numeric);
    printf("Битовое представление: %s\n", binary);
    printf("------------------------------\n");
    
    free(symbolic);
    free(binary);
}

void display_file_info(FileInfo* file_info) {
    if (!file_info) {
        printf("Ошибка: информация о файле не доступна\n");
        return;
    }
    
    printf("\n=== Права доступа файла: %s ===\n", file_info->filename);
    display_permissions(file_info->perms, NULL);
}

void display_menu(void) {
    printf("\n=== Управление правами доступа к файлам ===\n");
    printf("1. Ввести права доступа\n");
    printf("2. Показать права доступа файла\n");
    printf("3. Изменить права доступа\n");
    printf("4. Выйти\n");
    printf("Выберите опцию: ");
}

char* read_input(void) {
    char buffer[1024];
    if (scanf("%1023s", buffer) != 1) {
        return NULL;
    }
    
    char* result = malloc(strlen(buffer) + 1);
    if (result) {
        strcpy(result, buffer);
    }
    return result;
}

void process_numeric_input(void) {
    printf("Введите права доступа (3 цифры или 9 символов): ");
    
    char* input = read_input();
    if (!input) {
        printf("Ошибка чтения ввода\n");
        return;
    }
    
    FilePermissions perms = string_to_permissions(input);
    int numeric = permissions_to_numeric(perms);
    
    if (numeric >= 0 && numeric <= 777) {
        printf("\n");
        display_permissions(perms, "Введенные права доступа");
    } else {
        printf("Ошибка: неверный формат прав доступа\n");
    }
    
    free(input);
}

void process_file_input(void) {
    printf("Введите полный путь к файлу: ");
    
    char* filename = read_input();
    if (!filename) {
        printf("Ошибка чтения имени файла\n");
        return;
    }
    
    FileInfo* file_info = get_file_info(filename);
    if (file_info) {
        display_file_info(file_info);
        free_file_info(file_info);
    } else {
        printf("Ошибка: не удалось получить информацию о файле '%s'\n", filename);
    }
    
    free(filename);
}

void process_modification_input(void) {
    printf("Выберите базовые права:\n");
    printf("1. Использовать текущие права файла\n");
    printf("2. Ввести новые базовые права\n");
    printf("Выберите опцию: ");
    
    char* choice = read_input();
    if (!choice) {
        printf("Ошибка чтения ввода\n");
        return;
    }
    
    FilePermissions perms = {0};
    
    if (strcmp(choice, "1") == 0) {
        printf("Введите полный путь к файлу: ");
        char* filename = read_input();
        if (!filename) {
            printf("Ошибка чтения имени файла\n");
            free(choice);
            return;
        }
        
        FileInfo* file_info = get_file_info(filename);
        if (!file_info) {
            printf("Ошибка чтения файла '%s'\n", filename);
            free(choice);
            free(filename);
            return;
        }
        
        perms = file_info->perms;
        free_file_info(file_info);
        free(filename);
        
    } else if (strcmp(choice, "2") == 0) {
        printf("Введите базовые права: ");
        char* rights = read_input();
        if (!rights) {
            printf("Ошибка чтения ввода\n");
            free(choice);
            return;
        }
        
        perms = string_to_permissions(rights);
        int numeric = permissions_to_numeric(perms);
        if (numeric < 0 || numeric > 777) {
            printf("Ошибка: неверные базовые права\n");
            free(choice);
            free(rights);
            return;
        }
        free(rights);
    } else {
        printf("Неверный выбор\n");
        free(choice);
        return;
    }
    
    printf("\nТекущие права:\n");
    display_permissions(perms, NULL);
    
    printf("Введите новые права: ");
    char* new_rights = read_input();
    if (!new_rights) {
        printf("Ошибка чтения ввода\n");
        free(choice);
        return;
    }
    
    FilePermissions new_perms = string_to_permissions(new_rights);
    int numeric = permissions_to_numeric(new_perms);
    
    if (numeric >= 0 && numeric <= 777) {
        printf("\nИзмененные права:\n");
        display_permissions(new_perms, NULL);
    } else {
        printf("Ошибка: неверный формат новых прав\n");
    }
    
    free(choice);
    free(new_rights);
}