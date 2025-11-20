#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "phonebook.h"
#include "ui.h"

int main() {
  
    setlocale(LC_ALL, "ru_RU.UTF-8");       
    
    Phonebook phonebook;
    initPhonebook(&phonebook);
    
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
    
    freePhonebook(&phonebook);
    printf("Программа завершена. До свидания!\n");
    
    return 0;
}