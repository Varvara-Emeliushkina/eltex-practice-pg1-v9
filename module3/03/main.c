#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "phonebook.h"
#include "ui.h"

int main() {

    setlocale(LC_ALL, "ru_RU.UTF-8");       

    Phonebook phonebook;
    initPhonebook(&phonebook);

    // загружаем телефонную книгу из файла при запуске
    printf("Загрузка телефонной книги из файла...\n");
    if (loadPhonebookFromFile(&phonebook, PHONEBOOK_FILENAME)) {
        printf("Телефонная книга успешно загружена. Контактов: %d\n", getPhonebookContactCount(&phonebook));
    } else {
        printf("Не удалось загрузить телефонную книгу из файла. Будет создана новая.\n");
    }

    int choice;

    printf("=========================================\n");
    printf("        ТЕЛЕФОННАЯ КНИГА\n");
    printf("=========================================\n");
    
    do {
        showMainMenu();
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printError("Неверный ввод");
            continue;
        }
        clearInputBuffer();
        
        switch (choice) {
            case 1: handleAddContact(&phonebook); break;
            case 2: handleEditContact(&phonebook); break;
            case 3: handleDeleteContact(&phonebook); break;
            case 4: handleFindContact(&phonebook); break;
            case 5: handleListContacts(&phonebook); break;
            case 0: 
                printf("Завершение работы программы...\n");
                break;
            default: 
                printError("Неверный выбор. Попробуйте снова.");
                break;
        }
    } while (choice != 0);

    // сохраняем телефонную книгу в файл при завершении
    printf("Сохранение телефонной книги в файл...\n");
    if (savePhonebookToFile(&phonebook, PHONEBOOK_FILENAME)) {
        printf("Телефонная книга успешно сохранена. Контактов: %d\n", getPhonebookContactCount(&phonebook));
    } else {
        printError("Не удалось сохранить телефонную книгу в файл!");
    }
    
    freePhonebook(&phonebook);
    printf("Программа завершена. До свидания!\n");
    
    return 0;
}