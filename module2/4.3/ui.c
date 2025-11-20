#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

void displayContact(const Contact* contact) {
    if (contact == NULL) {
        printf("Контакт не существует.\n");
        return;
    }
    
    const char* firstName = getContactFirstName(contact);
    const char* lastName = getContactLastName(contact);
    
    printf("\n=== Контакт #%d ===\n", getContactUid(contact));
    printf("Имя: %s %s\n", 
           firstName ? firstName : "[не указано]",
           lastName ? lastName : "[не указано]");
    
    const char* workplace = getContactWorkplace(contact);
    const char* position = getContactPosition(contact);
    
    if (workplace != NULL && workplace[0] != '\0')
        printf("Место работы: %s\n", workplace);
    if (position != NULL && position[0] != '\0')
        printf("Должность: %s\n", position);
    

    int phoneCount = getContactPhoneCount(contact);
    if (phoneCount > 0) {
        printf("Телефоны (%d):\n", phoneCount);
        for (int i = 0; i < phoneCount; i++) {
            const char* phone = getContactPhone(contact, i);
            printf("  %d. %s\n", i + 1, phone ? phone : "[ошибка]");
        }
    } else {
        printf("Телефоны: не указаны\n");
    }
    

    int emailCount = getContactEmailCount(contact);
    if (emailCount > 0) {
        printf("Emails (%d):\n", emailCount);
        for (int i = 0; i < emailCount; i++) {
            const char* email = getContactEmail(contact, i);
            printf("  %d. %s\n", i + 1, email ? email : "[ошибка]");
        }
    } else {
        printf("Emails: не указаны\n");
    }
    

    int socialCount = getContactSocialCount(contact);
    if (socialCount > 0) {
        printf("Соцсети (%d):\n", socialCount);
        for (int i = 0; i < socialCount; i++) {
            const char* platform = getContactSocialPlatform(contact, i);
            const char* profile = getContactSocialProfile(contact, i);
            
            if (platform && profile) {
                printf("  %s: %s\n", platform, profile);
            } else {
                printf("  [платформа]: [профиль]\n");
            }
        }
    } else {
        printf("Соцсети: не указаны\n");
    }
    printf("==================\n");
}

void inOrderTraversalForDisplay(Contact* node, int* index) {
    if (node == NULL) return;
    
    inOrderTraversalForDisplay(node->left, index);
    
    const char* firstName = getContactFirstName(node);
    const char* lastName = getContactLastName(node);
    const char* position = getContactPosition(node);
    unsigned int uid = getContactUid(node);
    
    printf("%d. %s %s", (*index) + 1, 
           firstName ? firstName : "[не указано]",
           lastName ? lastName : "[не указано]");
    
    if (position != NULL && position[0] != '\0') {
        printf(" - %s", position);
    }
    
    printf(" [UID: %d]\n", uid);
    
    (*index)++;
    
    inOrderTraversalForDisplay(node->right, index);
}

void displayContactList(const Phonebook* pb) {
    if (pb == NULL) {
        printf("Телефонная книга не инициализирована.\n");
        return;
    }
    
    int count = getPhonebookContactCount(pb);
    if (count == 0) {
        printf("Телефонная книга пуста.\n");
        return;
    }
    
    printf("\n=== СПИСОК КОНТАКТОВ (%d) ===\n", count);
    int index = 0;
    inOrderTraversalForDisplay(pb->root, &index);
    printf("========================\n");
}

void showMainMenu(void) {
    printf("\n=== ТЕЛЕФОННАЯ КНИГА (БИНАРНОЕ ДЕРЕВО) ===\n");
    printf("1. Добавить контакт\n");
    printf("2. Редактировать контакт\n");
    printf("3. Удалить контакт\n");
    printf("4. Найти контакт\n");
    printf("5. Показать все контакты\n");
    printf("6. Показать структуру дерева\n");
    printf("7. Сбалансировать дерево\n");
    printf("8. Добавить тестовые данные\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

void handleAddContact(Phonebook* pb) {
    char firstName[100], lastName[100];
    char workplace[100] = "", position[100] = "", phone[100] = "", email[100] = "", platform[100] = "", profile[100] = "";
    int choice;
    
    printf("\n--- ДОБАВЛЕНИЕ КОНТАКТА ---\n");
    
    getStringInput("Введите имя: ", firstName, sizeof(firstName));
    getStringInput("Введите фамилию: ", lastName, sizeof(lastName));
    
    printf("\nДобавить дополнительные поля?\n");
    printf("1. Только имя и фамилия\n");
    printf("2. Добавить все доступные поля\n");
    printf("Выберите вариант: ");
    
    scanf("%d", &choice);
    clearInputBuffer();
    
    int result = 0;
    
    if (choice == 1) {
        result = addContactToPhonebook(pb, firstName, lastName, NULL);
    } else {
        printf("\nЗаполнение дополнительных полей:\n");
        
        getStringInput("Место работы (или Enter чтобы пропустить): ", workplace, sizeof(workplace));
        getStringInput("Должность (или Enter чтобы пропустить): ", position, sizeof(position));
        getStringInput("Телефон (или Enter чтобы пропустить): ", phone, sizeof(phone));
        getStringInput("Email (или Enter чтобы пропустить): ", email, sizeof(email));
        getStringInput("Соцсеть - платформа (или Enter чтобы пропустить): ", platform, sizeof(platform));
        
        if (strlen(platform) > 0) {
            getStringInput("Соцсеть - профиль: ", profile, sizeof(profile));
        }
        
        if (strlen(platform) > 0) {
            result = addContactToPhonebook(pb, firstName, lastName,
                                         "workplace", workplace,
                                         "position", position,
                                         "phone", phone,
                                         "email", email,
                                         "social", platform, profile,
                                         NULL);
        } else {
            result = addContactToPhonebook(pb, firstName, lastName,
                                         "workplace", workplace,
                                         "position", position,
                                         "phone", phone,
                                         "email", email,
                                         NULL);
        }
    }
    
    if (result) {
        printSuccess("Контакт успешно добавлен!");
    } else {
        printError("Не удалось добавить контакт. Возможно, книга переполнена.");
    }
}

void handleEditContact(Phonebook* pb) {
    unsigned int uid;
    
    printf("\n--- РЕДАКТИРОВАНИЕ КОНТАКТА ---\n");
    displayContactList(pb);
    
    if (getPhonebookContactCount(pb) == 0) {
        printf("Нет контактов для редактирования.\n");
        return;
    }
    
    uid = getIntInput("Введите UID контакта для редактирования: ");
    
    Contact* contact = findContactByUid(pb, uid);
    if (contact == NULL) {
        printError("Контакт с указанным UID не найден");
        return;
    }
    
    printf("\nТекущие данные контакта:\n");
    displayContact(contact);
    
    char newValue[100];
    int fieldChoice;
    
    printf("\nКакое поле вы хотите отредактировать?\n");
    printf("1. Имя\n");
    printf("2. Фамилия\n");
    printf("3. Место работы\n");
    printf("4. Должность\n");
    printf("5. Добавить телефон\n");
    printf("6. Удалить телефон\n");
    printf("7. Добавить email\n");
    printf("8. Удалить email\n");
    printf("9. Добавить соцсеть\n");
    printf("10. Удалить соцсеть\n");
    printf("0. Отмена\n");
    printf("Выберите поле: ");
    
    scanf("%d", &fieldChoice);
    clearInputBuffer();
    
    switch (fieldChoice) {
        case 1:
            getStringInput("Введите новое имя: ", newValue, sizeof(newValue));
            if (setContactFirstName(contact, newValue)) {
                printSuccess("Имя успешно обновлено!");
            } else {
                printError("Ошибка при обновлении имени");
            }
            break;
            
        case 2:
            getStringInput("Введите новую фамилию: ", newValue, sizeof(newValue));
            if (setContactLastName(contact, newValue)) {
                printSuccess("Фамилия успешно обновлена!");
            } else {
                printError("Ошибка при обновлении фамилии");
            }
            break;
            
        case 3:
            getStringInput("Введите новое место работы: ", newValue, sizeof(newValue));
            setContactWorkplace(contact, newValue);
            printSuccess("Место работы обновлено!");
            break;
            
        case 4:
            getStringInput("Введите новую должность: ", newValue, sizeof(newValue));
            setContactPosition(contact, newValue);
            printSuccess("Должность обновлена!");
            break;
            
        case 5:
            getStringInput("Введите новый телефон: ", newValue, sizeof(newValue));
            if (addPhoneToContact(contact, newValue)) {
                printSuccess("Телефон добавлен!");
            } else {
                printError("Не удалось добавить телефон. Возможно, достигнут лимит.");
            }
            break;
            
        case 6: {
            int phoneCount = getContactPhoneCount(contact);
            if (phoneCount == 0) {
                printError("Нет телефонов для удаления");
                break;
            }
            
            printf("\nСписок телефонов:\n");
            for (int i = 0; i < phoneCount; i++) {
                printf("%d. %s\n", i + 1, getContactPhone(contact, i));
            }
            
            int phoneIndex = getIntInput("Введите номер телефона для удаления: ");
            if (phoneIndex < 1 || phoneIndex > phoneCount) {
                printError("Неверный номер телефона");
                break;
            }
            
            if (removePhoneFromContact(contact, phoneIndex - 1)) {
                printSuccess("Телефон успешно удален!");
            } else {
                printError("Ошибка при удалении телефона");
            }
            break;
        }
            
        case 7:
            getStringInput("Введите новый email: ", newValue, sizeof(newValue));
            if (addEmailToContact(contact, newValue)) {
                printSuccess("Email добавлен!");
            } else {
                printError("Не удалось добавить email. Возможно, достигнут лимит.");
            }
            break;
            
        case 8: {
            int emailCount = getContactEmailCount(contact);
            if (emailCount == 0) {
                printError("Нет email для удаления");
                break;
            }
            
            printf("\nСписок email:\n");
            for (int i = 0; i < emailCount; i++) {
                printf("%d. %s\n", i + 1, getContactEmail(contact, i));
            }
            
            int emailIndex = getIntInput("Введите номер email для удаления: ");
            if (emailIndex < 1 || emailIndex > emailCount) {
                printError("Неверный номер email");
                break;
            }
            
            if (removeEmailFromContact(contact, emailIndex - 1)) {
                printSuccess("Email успешно удален!");
            } else {
                printError("Ошибка при удалении email");
            }
            break;
        }
            
        case 9: {
            char platform[100], profile[100];
            getStringInput("Введите платформу: ", platform, sizeof(platform));
            getStringInput("Введите профиль: ", profile, sizeof(profile));
            if (addSocialToContact(contact, platform, profile)) {
                printSuccess("Соцсеть добавлена!");
            } else {
                printError("Не удалось добавить соцсеть. Возможно, достигнут лимит.");
            }
            break;
        }
            
        case 10: {
            int socialCount = getContactSocialCount(contact);
            if (socialCount == 0) {
                printError("Нет соцсетей для удаления");
                break;
            }
            
            printf("\nСписок соцсетей:\n");
            for (int i = 0; i < socialCount; i++) {
                printf("%d. %s: %s\n", i + 1, 
                       getContactSocialPlatform(contact, i),
                       getContactSocialProfile(contact, i));
            }
            
            int socialIndex = getIntInput("Введите номер соцсети для удаления: ");
            if (socialIndex < 1 || socialIndex > socialCount) {
                printError("Неверный номер соцсети");
                break;
            }
            
            if (removeSocialFromContact(contact, socialIndex - 1)) {
                printSuccess("Соцсеть успешно удалена!");
            } else {
                printError("Ошибка при удалении соцсети");
            }
            break;
        }
            
        case 0:
            printf("Редактирование отменено.\n");
            return;
            
        default:
            printError("Неверный выбор поля");
            return;
    }
    
    printf("\nОбновленные данные контакта:\n");
    displayContact(contact);
}

void handleDeleteContact(Phonebook* pb) {
    unsigned int uid;
    
    printf("\n--- УДАЛЕНИЕ КОНТАКТА ---\n");
    displayContactList(pb);
    
    if (getPhonebookContactCount(pb) == 0) {
        printf("Нет контактов для удаления.\n");
        return;
    }
    
    uid = getIntInput("Введите UID контакта для удаления: ");
    
    Contact* contact = findContactByUid(pb, uid);
    if (contact == NULL) {
        printError("Контакт с указанным UID не найден");
        return;
    }
    
    printf("\nВы собираетесь удалить контакт:\n");
    displayContact(contact);
    
    printf("Вы уверены? (1 - да, 0 - нет): ");
    int confirm;
    if (scanf("%d", &confirm) != 1) {
        clearInputBuffer();
        printError("Неверный ввод");
        return;
    }
    clearInputBuffer();
    
    if (confirm != 1) {
        printf("Удаление отменено.\n");
        return;
    }
    
    if (deleteContactFromPhonebook(pb, uid)) {
        printSuccess("Контакт успешно удален!");
    } else {
        printError("Ошибка при удалении контакта");
    }
}

void handleFindContact(Phonebook* pb) {
    int searchType;
    
    printf("\n--- ПОИСК КОНТАКТА ---\n");
    printf("Выберите тип поиска:\n");
    printf("1. По UID (уникальный идентификатор)\n");
    printf("2. По имени и фамилии\n");
    printf("0. Отмена\n");
    printf("Выберите тип поиска: ");
    
    scanf("%d", &searchType);
    clearInputBuffer();
    
    switch (searchType) {
        case 1: {
            unsigned int uid = getIntInput("Введите UID для поиска: ");
            Contact* contact = findContactByUid(pb, uid);
            if (contact != NULL) {
                printf("\nКОНТАКТ НАЙДЕН:\n");
                displayContact(contact);
            } else {
                printError("Контакт с указанным UID не найден");
            }
            break;
        }
            
        case 2: {
            char firstName[100], lastName[100];
            
            // явная инициализация
            memset(firstName, 0, sizeof(firstName));
            memset(lastName, 0, sizeof(lastName));
            
            getStringInput("Введите имя: ", firstName, sizeof(firstName));
            getStringInput("Введите фамилию: ", lastName, sizeof(lastName));
            
            // Разрешаем поиск только по имени или только по фамилии
            if (strlen(firstName) == 0 && strlen(lastName) == 0) {
                printError("Введите хотя бы имя или фамилию");
                break;
            }
            
            Contact* foundContacts[10];
            int foundCount = findAllContactsByName(pb, 
                (strlen(firstName) > 0) ? firstName : NULL,
                (strlen(lastName) > 0) ? lastName : NULL,
                foundContacts, 10);
            
            if (foundCount > 0) {
                if (foundCount == 1) {
                    printf("\nНАЙДЕН 1 КОНТАКТ:\n");
                    displayContact(foundContacts[0]);
                } else {
                    printf("\nНАЙДЕНО КОНТАКТОВ: %d\n", foundCount);
                    printf("Выберите нужный контакт:\n");
                    
                    for (int i = 0; i < foundCount; i++) {
                        printf("%d. %s %s", i + 1, 
                            getContactFirstName(foundContacts[i]),
                            getContactLastName(foundContacts[i]));
                        
                        const char* workplace = getContactWorkplace(foundContacts[i]);
                        if (workplace && strlen(workplace) > 0) {
                            printf(" - Работа: %s", workplace);
                        }
                        
                        const char* position = getContactPosition(foundContacts[i]);
                        if (position && strlen(position) > 0) {
                            printf(" - Должность: %s", position);
                        }
                        
                        printf(" [UID: %d]\n", getContactUid(foundContacts[i]));
                    }
                    
                    printf("\nВведите номер контакта для просмотра (0 - отмена): ");
                    int choice = getIntInput("");
                    
                    if (choice > 0 && choice <= foundCount) {
                        printf("\n--- ДЕТАЛИ КОНТАКТА ---\n");
                        displayContact(foundContacts[choice - 1]);
                    } else if (choice != 0) {
                        printError("Неверный выбор");
                    }
                }
            } else {
                printError("Контакты с указанным именем и фамилией не найдены");
            }
            break;
        }
            
        case 0:
            printf("Поиск отменен.\n");
            return;
            
        default:
            printError("Неверный тип поиска");
            return;
    }
}

void handleListContacts(Phonebook* pb) {
    printf("\n--- СПИСОК ВСЕХ КОНТАКТОВ ---\n");
    displayContactList(pb);
}

void handleBalanceTree(Phonebook* pb) {
    printf("\n--- БАЛАНСИРОВКА ДЕРЕВА ---\n");
    balanceTree(pb);
}

void handleDisplayTree(Phonebook* pb) {
    printf("\n--- СТРУКТУРА ДЕРЕВА ---\n");
    displayTree(pb);
}

void handleTestData(Phonebook* pb) {
    printf("\n--- ТЕСТОВЫЕ ДАННЫЕ ---\n");
    addTestData(pb);
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getIntInput(const char* prompt) {
    int value;
    printf("%s", prompt);
    if (scanf("%d", &value) != 1) {
        clearInputBuffer();
        return -1;
    }
    clearInputBuffer();
    return value;
}

void getStringInput(const char* prompt, char* buffer, int bufferSize) {
    printf("%s", prompt);
    fflush(stdout);
    
    // инициализируем буфер
    memset(buffer, 0, bufferSize);
    
    if (fgets(buffer, bufferSize, stdin) != NULL) {
        // удаляем символ новой строки
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

void printError(const char* message) {
    printf("ОШИБКА: %s\n", message);
}

void printSuccess(const char* message) {
    printf("УСПЕХ: %s\n", message);
}