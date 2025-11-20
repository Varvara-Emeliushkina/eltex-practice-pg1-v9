#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

typedef double (*math_func)(double, double);

typedef struct {
    char name[50];
    math_func function;
    HMODULE dll;
} DynamicFunction;

int load_functions(const char* dir, DynamicFunction** functions) {
    WIN32_FIND_DATA find_data;
    char search_path[260];
    int count = 0;
    
    /* создаем путь для поиска
        тут конечно можно сделать и проще, создать массив на какое-то кол-во функций
        и malloc-ом выделить им память, но мы поступим как умные и лишний раз пройдемся
        по файлам, зато оптимально используем память и при добавлении доп. функций
        нам не нужно будет заботиться об изменения макс их кол-ва при загрузке
    */
    snprintf(search_path, sizeof(search_path), "%s\\*.dll", dir);
    
    // подсчитываем количество DLL
    HANDLE hFind = FindFirstFile(search_path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE) return 0;
    
    do {
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            count++;
        }
    } while (FindNextFile(hFind, &find_data));
    FindClose(hFind);
    
    if (count == 0) return 0;
    
    // выделяем память
    *functions = malloc(count * sizeof(DynamicFunction));
    
    // загружаем функции
    hFind = FindFirstFile(search_path, &find_data);
    int index = 0;
    
    do {
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char dll_path[260];
            snprintf(dll_path, sizeof(dll_path), "%s\\%s", dir, find_data.cFileName);
            
            // загружаем DLL
            HMODULE dll = LoadLibrary(dll_path);
            if (dll) {
                // имя функции = имя файла без .dll
                char func_name[50];
                strncpy(func_name, find_data.cFileName, sizeof(func_name));
                func_name[strlen(func_name) - 4] = '\0'; // убираем .dll
                
                // получаем указатель на функцию
                math_func func = (math_func)GetProcAddress(dll, func_name);
                if (func) {
                    strcpy((*functions)[index].name, func_name);
                    (*functions)[index].function = func;
                    (*functions)[index].dll = dll;
                    index++;
                    printf("Загружена функция: %s\n", func_name);
                } else {
                    FreeLibrary(dll);
                }
            }
        }
    } while (FindNextFile(hFind, &find_data) && index < count);
    
    FindClose(hFind);
    return index;
}

int main() {

    setlocale(LC_ALL, "ru_RU.UTF-8");

    DynamicFunction* functions = NULL;
    int count = load_functions("libs", &functions);
    
    if (count == 0) {
        printf("Не найдено функций в папке libs!\n");
        return 1;
    }
    
    printf("\nЗагружено %d функций:\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, functions[i].name);
    }
    
    // основной цикл калькулятора
    int choice;
    double a, b;
    
    while (1) {
        printf("\n=== !КАЛЬКУЛЯТОР ===\n");
        for (int i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, functions[i].name);
        }
        printf("0. Выход\n");
        printf("Выберите операцию: ");
        scanf("%d", &choice);
        
        if (choice == 0) break;
        if (choice < 1 || choice > count) {
            printf("Неверный выбор!\n");
            continue;
        }
        
        printf("Введите два числа: ");
        scanf("%lf %lf", &a, &b);
        
        double result = functions[choice - 1].function(a, b);
        printf("Результат: %.2f\n", result);
    }
    
    // выгружаем библиотеки
    for (int i = 0; i < count; i++) {
        FreeLibrary(functions[i].dll);
    }
    free(functions);
    
    return 0;
}