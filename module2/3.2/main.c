#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "ui.h"

int main() {

    setlocale(LC_ALL, "ru_RU.UTF-8");

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
                process_simulation_input();
                break;
            case 2:
                printf("Выход из программы.\n");
                return 0;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
                break;
        }
    }
    
    return 0;
}