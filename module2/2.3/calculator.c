#include <stdio.h>
#include <math.h>
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

// как пример быстрого добавления новой функции (+ определить ее в массиве указателей)
double power(double a, double b) {
    return pow(a, b);
}

void display_menu() {
    printf("------------------------------------\n");
    printf("           КАЛЬКУЛЯТОР\n");
    printf("------------------------------------\n");
    printf("1. Сложение (+)\n");
    printf("2. Вычитание (-)\n");
    printf("3. Умножение (*)\n");
    printf("4. Деление (/)\n");
    printf("5. Степень (^)\n");
    printf("0. Выход\n");
}

void run_calculator() {
    int choice;
    double a, b, result;
    
    // массивы указателей на функции и их описаний
    double (*operations[])(double, double) = {add, subtract, multiply, divide, power};
    char* symbols[] = {"+", "-", "*", "/", "^"};
    char* names[] = {"сложения", "вычитания", "умножения", "деления", "возведения в степень"};
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        display_menu();
        choice = get_input("\nВыберите операцию: ");
        
        if (choice == 0) {
            break;
        }
        
        if (choice < 1 || choice > operation_count) {
            print_error("\nОшибка: Неверный выбор операции!\n");
            continue;
        }
        
        a = get_input("Введите первое число: ");
        b = get_input("Введите второе число: ");
        
        // вызов через указатель
        int index = choice - 1;
        result = operations[index](a, b);

        printf("Результат %.2lf %s %.2lf = %.2lf\n", 
               a, symbols[index], b, result);

        if (choice != 4 || (choice == 4 && b != 0)) {
            printf("Операция %s завершена успешно!\n\n", names[index]);
        }
    }
}