#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "phonebook.h"

void initPhonebook(Phonebook* pb) {
    if (pb == NULL) return;
    
    pb->root = NULL;
    pb->count = 0;
    pb->nextUid = 1;
}

void freePhonebookRecursive(Contact* node) {
    if (node == NULL) return;
    
    freePhonebookRecursive(node->left);
    freePhonebookRecursive(node->right);
    freeContact(node);
}

void freePhonebook(Phonebook* pb) {
    if (pb == NULL) return;
    
    freePhonebookRecursive(pb->root);
    pb->root = NULL;
    pb->count = 0;
    pb->nextUid = 1;
}

Contact* insertContactToTree(Contact* root, Contact* newContact) {
    if (root == NULL) return newContact;
    
    if (newContact->uid < root->uid) {
        root->left = insertContactToTree(root->left, newContact);
    } else {
        root->right = insertContactToTree(root->right, newContact);
    }
    
    return root;
}

int addContactToPhonebook(Phonebook* pb, const char* firstName, const char* lastName, ...) {
    if (pb == NULL || firstName == NULL || lastName == NULL) return 0;
    if (pb->count >= MAX_CONTACTS) return 0;
    
    Contact* newContact = createContact(firstName, lastName);
    if (newContact == NULL) return 0;
    
    setContactUid(newContact, pb->nextUid++);
    newContact->left = NULL;
    newContact->right = NULL;
    
    // Обработка дополнительных параметров только если они есть
    va_list args;
    va_start(args, lastName);
    
    const char* paramType = va_arg(args, const char*);
    if (paramType != NULL) {
        do {
            if (strcmp(paramType, "workplace") == 0) {
                const char* workplace = va_arg(args, const char*);
                if (workplace != NULL && strlen(workplace) > 0) {
                    setContactWorkplace(newContact, workplace);
                }
            }
            else if (strcmp(paramType, "position") == 0) {
                const char* position = va_arg(args, const char*);
                if (position != NULL && strlen(position) > 0) {
                    setContactPosition(newContact, position);
                }
            }
            else if (strcmp(paramType, "phone") == 0) {
                const char* phone = va_arg(args, const char*);
                if (phone != NULL && strlen(phone) > 0) {
                    addPhoneToContact(newContact, phone);
                }
            }
            else if (strcmp(paramType, "email") == 0) {
                const char* email = va_arg(args, const char*);
                if (email != NULL && strlen(email) > 0) {
                    addEmailToContact(newContact, email);
                }
            }
            else if (strcmp(paramType, "social") == 0) {
                const char* platform = va_arg(args, const char*);
                const char* profile = va_arg(args, const char*);
                if (platform != NULL && profile != NULL && 
                    strlen(platform) > 0 && strlen(profile) > 0) {
                    addSocialToContact(newContact, platform, profile);
                }
            }
        } while ((paramType = va_arg(args, const char*)) != NULL);
    }
    
    va_end(args);
    pb->root = insertContactToTree(pb->root, newContact);
    pb->count++;
    
    if (pb->count % 10 == 0) {
        balanceTree(pb);
    }
    
    return 1;
}

Contact* findMinNode(Contact* node) {
    if (node == NULL) return NULL;
    
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Contact* deleteContactFromTree(Contact* root, unsigned int uid, int* found) {
    if (root == NULL) return NULL;
    
    if (uid < root->uid) {
        root->left = deleteContactFromTree(root->left, uid, found);
    } else if (uid > root->uid) {
        root->right = deleteContactFromTree(root->right, uid, found);
    } else {
        *found = 1;
        
        // Случай 1: Узел без левого потомка
        if (root->left == NULL) {
            Contact* temp = root->right;
            freeContact(root);
            return temp;
        } 
        // Случай 2: Узел без правого потомка
        else if (root->right == NULL) {
            Contact* temp = root->left;
            freeContact(root);
            return temp;
        } 
        // Случай 3: Узел с двумя потомками
        else {
            // Находим минимальный узел в правом поддереве
            Contact* minNode = findMinNode(root->right);
            
            // Создаем временные копии данных минимального узла
            char* tempFirstName = malloc(strlen(minNode->firstName) + 1);
            char* tempLastName = malloc(strlen(minNode->lastName) + 1);
            
            if (tempFirstName == NULL || tempLastName == NULL) {
                // Если не удалось выделить память, освобождаем ВЕСЬ узел
                free(tempFirstName);
                free(tempLastName);
                
                // Удаляем текущий узел полностью и объединяем поддеревья
                Contact* leftTree = root->left;
                Contact* rightTree = root->right;
                
                // Полностью освобождаем текущий узел
                freeContact(root);
                
                // Если левое поддерево пустое, возвращаем правое
                if (leftTree == NULL) return rightTree;
                
                // Находим крайний правый узел левого поддерева
                Contact* current = leftTree;
                while (current->right != NULL) {
                    current = current->right;
                }
                // Присоединяем правое поддерево к левому
                current->right = rightTree;
                return leftTree;
            }
            
            // Копируем данные из минимального узла
            strcpy(tempFirstName, minNode->firstName);
            strcpy(tempLastName, minNode->lastName);
            
            // Освобождаем старые данные и заменяем новыми
            free(root->firstName);
            free(root->lastName);
            root->firstName = tempFirstName;
            root->lastName = tempLastName;
            root->uid = minNode->uid;
            
            // Копируем остальные поля
            strncpy(root->workplace, minNode->workplace, MAX_STRING_LENGTH - 1);
            root->workplace[MAX_STRING_LENGTH - 1] = '\0';
            
            strncpy(root->position, minNode->position, MAX_STRING_LENGTH - 1);
            root->position[MAX_STRING_LENGTH - 1] = '\0';
            
            // Копируем телефоны
            root->phoneCount = minNode->phoneCount;
            for (int i = 0; i < minNode->phoneCount; i++) {
                strncpy(root->phones[i], minNode->phones[i], MAX_STRING_LENGTH - 1);
                root->phones[i][MAX_STRING_LENGTH - 1] = '\0';
            }
            
            // Копируем emails
            root->emailCount = minNode->emailCount;
            for (int i = 0; i < minNode->emailCount; i++) {
                strncpy(root->emails[i], minNode->emails[i], MAX_STRING_LENGTH - 1);
                root->emails[i][MAX_STRING_LENGTH - 1] = '\0';
            }
            
            // Копируем соцсети
            root->socialCount = minNode->socialCount;
            for (int i = 0; i < minNode->socialCount; i++) {
                strncpy(root->socials[i].platform, minNode->socials[i].platform, MAX_STRING_LENGTH - 1);
                root->socials[i].platform[MAX_STRING_LENGTH - 1] = '\0';
                strncpy(root->socials[i].profile, minNode->socials[i].profile, MAX_STRING_LENGTH - 1);
                root->socials[i].profile[MAX_STRING_LENGTH - 1] = '\0';
            }
            
            // Удаляем минимальный узел из правого поддерева
            root->right = deleteContactFromTree(root->right, minNode->uid, found);
        }
    }
    return root;
}

int deleteContactFromPhonebook(Phonebook* pb, unsigned int uid) {
    if (pb == NULL) return 0;
    
    int found = 0;
    pb->root = deleteContactFromTree(pb->root, uid, &found);
    
    if (found) {
        pb->count--;
    }
    return found;
}

Contact* findContactInTree(Contact* root, unsigned int uid) {
    if (root == NULL) return NULL;
    
    if (uid == root->uid) return root;
    if (uid < root->uid) return findContactInTree(root->left, uid);
    return findContactInTree(root->right, uid);
}

Contact* findContactByUid(const Phonebook* pb, unsigned int uid) {
    if (pb == NULL) return NULL;
    return findContactInTree(pb->root, uid);
}

void findAllContactsByNameInTree(Contact* root, const char* firstName, const char* lastName, 
                                Contact* results[], int* foundCount, int maxResults) {
    if (root == NULL || *foundCount >= maxResults) return;
    
    // Сначала левое поддерево
    findAllContactsByNameInTree(root->left, firstName, lastName, results, foundCount, maxResults);
    
    // Проверяем текущий узел
    if (root->firstName != NULL && root->lastName != NULL) {
        int firstNameMatch = (firstName == NULL || strlen(firstName) == 0) ? 
                            1 : (strcmp(root->firstName, firstName) == 0);
        int lastNameMatch = (lastName == NULL || strlen(lastName) == 0) ? 
                           1 : (strcmp(root->lastName, lastName) == 0);
        
        if (firstNameMatch && lastNameMatch) {
            results[*foundCount] = root;
            (*foundCount)++;
        }
    }
    
    if (*foundCount >= maxResults) return;
    
    // Затем правое поддерево
    findAllContactsByNameInTree(root->right, firstName, lastName, results, foundCount, maxResults);
}

Contact* findContactByName(const Phonebook* pb, const char* firstName, const char* lastName) {
    if (pb == NULL || firstName == NULL || lastName == NULL) return NULL;
    
    Contact* results[1];
    int found = findAllContactsByName(pb, firstName, lastName, results, 1);
    
    return (found > 0) ? results[0] : NULL;
}

int getPhonebookContactCount(const Phonebook* pb) {
    return pb ? pb->count : 0;
}

Contact* getContactByIndexRecursive(Contact* node, int* currentIndex, int targetIndex) {
    if (node == NULL) return NULL;
    
    Contact* leftResult = getContactByIndexRecursive(node->left, currentIndex, targetIndex);
    if (leftResult != NULL) return leftResult;
    
    if (*currentIndex == targetIndex) return node;
    (*currentIndex)++;
    
    return getContactByIndexRecursive(node->right, currentIndex, targetIndex);
}

Contact* getContactByIndex(const Phonebook* pb, int index) {
    if (pb == NULL || index < 0 || index >= pb->count) return NULL;
    
    int currentIndex = 0;
    return getContactByIndexRecursive(pb->root, &currentIndex, index);
}

int findAllContactsByName(const Phonebook* pb, const char* firstName, const char* lastName, 
                         Contact* results[], int maxResults) {
    if (pb == NULL || maxResults <= 0) {
        return 0;
    }
    
    int foundCount = 0;
    findAllContactsByNameInTree(pb->root, firstName, lastName, results, &foundCount, maxResults);
    return foundCount;
}

void inOrderTraversalToArray(Contact* root, Contact** array, int* index) {
    if (root == NULL) return;
    
    inOrderTraversalToArray(root->left, array, index);
    array[*index] = root;
    (*index)++;
    inOrderTraversalToArray(root->right, array, index);
}

Contact** treeToArray(const Phonebook* pb, int* size) {
    if (pb == NULL || pb->count == 0) {
        *size = 0;
        return NULL;
    }
    
    Contact** array = (Contact**)malloc(pb->count * sizeof(Contact*));
    if (array == NULL) {
        *size = 0;
        return NULL;
    }
    
    int index = 0;
    inOrderTraversalToArray(pb->root, array, &index);
    *size = pb->count;
    
    return array;
}

Contact* arrayToTree(Contact** contacts, int start, int end) {
    if (start > end) return NULL;
    
    int mid = (start + end) / 2;
    Contact* root = contacts[mid];
    
    root->left = arrayToTree(contacts, start, mid - 1);
    root->right = arrayToTree(contacts, mid + 1, end);
    
    return root;
}

void balanceTree(Phonebook* pb) {
    if (pb == NULL || pb->count <= 1) return;
    
    int size;
    Contact** contacts = treeToArray(pb, &size);
    
    if (contacts != NULL && size > 0) {
        pb->root = arrayToTree(contacts, 0, size - 1);
        free(contacts);
        printf("Дерево сбалансировано! Всего контактов: %d\n", pb->count);
    }
}

void displayTreeRecursive(Contact* node, int level) {
    if (node == NULL) return;
    
    displayTreeRecursive(node->right, level + 1);
    
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    
    printf("%d: %s %s\n", node->uid, 
           getContactFirstName(node) ? getContactFirstName(node) : "[нет]", 
           getContactLastName(node) ? getContactLastName(node) : "[нет]");
    
    displayTreeRecursive(node->left, level + 1);
}

void displayTree(const Phonebook* pb) {
    if (pb == NULL || pb->root == NULL) {
        printf("Дерево пустое.\n");
        return;
    }
    
    printf("\n=== СТРУКТУРА ДЕРЕВА ===\n");
    displayTreeRecursive(pb->root, 0);
    printf("========================\n");
}

void addTestData(Phonebook* pb) {
    if (pb == NULL) return;
    
    printf("\nДобавление тестовых данных...\n");
    
    // Создаем контакты напрямую через createContact и insertContactToTree
    Contact* contact;
    
    contact = createContact("Иван", "Иванов");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Петр", "Петров");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Мария", "Сидорова");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Алексей", "Смирнов");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Елена", "Кузнецова");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Сергей", "Попов");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Ольга", "Васильева");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Дмитрий", "Новиков");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Анна", "Морозова");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    contact = createContact("Андрей", "Волков");
    if (contact) {
        setContactUid(contact, pb->nextUid++);
        contact->left = NULL;
        contact->right = NULL;
        pb->root = insertContactToTree(pb->root, contact);
        pb->count++;
        printf("Добавлен контакт: %s %s [UID: %d]\n", contact->firstName, contact->lastName, contact->uid);
    }
    
    printf("Добавлено %d тестовых контактов.\n", pb->count);
}