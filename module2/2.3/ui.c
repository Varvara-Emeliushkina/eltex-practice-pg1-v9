#include "ui.h"
#include <stdio.h>

double get_input(const char* prompt) {
    double value;
    printf("%s", prompt);
    
    while (scanf("%lf", &value) != 1) {
        print_error("Ошибка: Введите корректное число!\n");
        clear_input_buffer();
        printf("%s", prompt);
    }
    clear_input_buffer();
    
    return value;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_error(const char* message) {
    printf("%s", message);
}