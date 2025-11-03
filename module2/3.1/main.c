#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "ui.h"

int main() {

    setlocale(LC_ALL, "ru_RU.UTF-8");

    printf("Программа для работы с правами доступа к файлам\n");
    
    while (1) {
        display_menu();
        
        char* input = read_input();
        if (!input) {
            printf("Ошибка ввода\n");
            continue;
        }
        
        int choice = atoi(input);
        free(input);
        
        switch (choice) {
            case 1:
                process_numeric_input();
                break;
            case 2:
                process_file_input();
                break;
            case 3:
                process_modification_input();
                break;
            case 4:
                printf("Выход из программы.\n");
                return 0;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
                break;
        }
    }
    
    return 0;
}