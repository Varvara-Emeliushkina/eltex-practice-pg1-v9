#include <stdio.h>
#include "calculator.h"
#include "ui.h"

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        print_error("Ошибка: Деление на ноль!");
        return 0;
    }
    return a / b;
}

void display_menu() {
    printf("------------------------------------\n");
    printf("           КАЛЬКУЛЯТОР\n");
    printf("------------------------------------\n");
    printf("1. Сложение (+)\n");
    printf("2. Вычитание (-)\n");
    printf("3. Умножение (*)\n");
    printf("4. Деление (/)\n");
    printf("0. Выход\n");
}

void run_calculator() {
    int choice;
    double a, b, result;
    char* operation_name = "";
    
    while (1) {
        display_menu();
        choice = get_input("\nВыберите операцию: ");
        
        if (choice == 0) {
            break;
        }
        
        if (choice < 1 || choice > 4) {
            print_error("\nОшибка: Неверный выбор операции!\n");
            continue;
        }
        
        a = get_input("Введите первое число: ");
        b = get_input("Введите второе число: ");
        
        switch (choice) {
            case 1:
                result = add(a, b);
                operation_name = "сложения";
                printf("Результат %.2lf + %.2lf = %.2f", a, b, result);
                break;
            case 2:
                result = subtract(a, b);
                operation_name = "вычитания";
                printf("Результат %.2lf - %.2lf = %.2f", a, b, result);
                break;
            case 3:
                result = multiply(a, b);
                operation_name = "умножения";
                printf("Результат %.2lf * %.2lf = %.2f", a, b, result);
                break;
            case 4:
                result = divide(a, b);
                if (b != 0) {
                    operation_name = "деления";
                    printf("Результат %.2lf / %.2lf = %.2f", a, b, result);
                }
                break;
        }
        
        printf("\nОперация %s завершена успешно!\n", operation_name);
    }
}