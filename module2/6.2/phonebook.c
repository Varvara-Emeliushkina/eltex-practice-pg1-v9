#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "phonebook.h"

void initPhonebook(Phonebook* pb) {
    if (pb == NULL) return;
    
    pb->head = NULL;
    pb->tail = NULL;
    pb->count = 0;
    pb->nextUid = 1;
}

void freePhonebook(Phonebook* pb) {
    if (pb == NULL) return;
    
    Contact* current = pb->head;
    while (current != NULL) {
        Contact* next = current->next;
        freeContact(current);
        current = next;
    }
    
    pb->head = NULL;
    pb->tail = NULL;
    pb->count = 0;
}

// функция сравнения контактов для упорядочивания (по фамилии, затем по имени)
int compareContacts(const Contact* a, const Contact* b) {
    if (a == NULL || b == NULL) return 0;
    
    const char* aLastName = getContactLastName(a);
    const char* bLastName = getContactLastName(b);
    const char* aFirstName = getContactFirstName(a);
    const char* bFirstName = getContactFirstName(b);
    
    // сравниваем фамилии
    int lastNameCompare = strcmp(aLastName ? aLastName : "", bLastName ? bLastName : "");
    if (lastNameCompare != 0) {
        return lastNameCompare;
    }
    
    // если фамилии одинаковые, сравниваем имена
    return strcmp(aFirstName ? aFirstName : "", bFirstName ? bFirstName : "");
}

// вставка контакта в упорядоченный список
void insertContactSorted(Phonebook* pb, Contact* newContact) {
    if (pb == NULL || newContact == NULL) return;
    
    // Если список пустой
    if (pb->head == NULL) {
        pb->head = newContact;
        pb->tail = newContact;
        newContact->prev = NULL;
        newContact->next = NULL;
        return;
    }
    
    // поиск места для вставки
    Contact* current = pb->head;
    Contact* prev = NULL;
    
    while (current != NULL && compareContacts(newContact, current) > 0) {
        prev = current;
        current = current->next;
    }
    
    // вставка в начало списка
    if (prev == NULL) {
        newContact->next = pb->head;
        newContact->prev = NULL;
        pb->head->prev = newContact;
        pb->head = newContact;
    }
    // вставка в конец списка
    else if (current == NULL) {
        newContact->prev = pb->tail;
        newContact->next = NULL;
        pb->tail->next = newContact;
        pb->tail = newContact;
    }
    // вставка в середину списка
    else {
        newContact->prev = prev;
        newContact->next = current;
        prev->next = newContact;
        current->prev = newContact;
    }
}

// функция с переменным числом параметров
int addContactToPhonebook(Phonebook* pb, const char* firstName, const char* lastName, ...) {
    if (pb == NULL || firstName == NULL || lastName == NULL) return 0;
    
    // создаем контакт с обязательными полями
    Contact* newContact = createContact(firstName, lastName);
    if (newContact == NULL) return 0;
    
    setContactUid(newContact, pb->nextUid++);
    
    // обрабатываем дополнительные параметры
    va_list args;
    va_start(args, lastName);
    
    const char* paramType;
    while ((paramType = va_arg(args, const char*)) != NULL) {
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
            if (platform != NULL && profile != NULL && strlen(platform) > 0 && strlen(profile) > 0) {
                addSocialToContact(newContact, platform, profile);
            }
        }
    }
    
    va_end(args);
    // вставляем контакт в упорядоченный список
    insertContactSorted(pb, newContact);
    pb->count++;
    return 1;
}

int deleteContactFromPhonebook(Phonebook* pb, unsigned int uid) {
    if (pb == NULL) return 0;
    
    Contact* contact = findContactByUid(pb, uid);
    if (contact == NULL) return 0;
    
    // обновляем связи
    if (contact->prev != NULL) {
        contact->prev->next = contact->next;
    } else {
        pb->head = contact->next;
    }
    
    if (contact->next != NULL) {
        contact->next->prev = contact->prev;
    } else {
        pb->tail = contact->prev;
    }
    
    freeContact(contact);
    pb->count--;
    return 1;
}

// pекурсивный поиск по UID в списке
Contact* findContactByUidInList(const Contact* head, unsigned int uid) {
    if (head == NULL) return NULL;
    
    if (getContactUid(head) == uid) {
        return (Contact*)head;
    }
    
    return findContactByUidInList(head->next, uid);
}

Contact* findContactByUid(const Phonebook* pb, unsigned int uid) {
    if (pb == NULL) return NULL;
    return findContactByUidInList(pb->head, uid);;
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

Contact* getContactByIndex(const Phonebook* pb, int index) {
    if (pb == NULL || index < 0 || index >= pb->count) return NULL;
    
    Contact* current = pb->head;
    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }
    
    return current;
}

// функция поиска всех контактов с одинаковым именем
int findAllContactsByName(const Phonebook* pb, const char* firstName, const char* lastName, 
                         Contact* results[], int maxResults) {
    if (pb == NULL || firstName == NULL || lastName == NULL || results == NULL || maxResults <= 0) {
        return 0;
    }
    
    int foundCount = 0;
    
    Contact* current = pb->head;
    
    while (current != NULL && foundCount < maxResults) {
        const char* contactFirstName = getContactFirstName(current);
        const char* contactLastName = getContactLastName(current);
        
        if (contactFirstName && contactLastName &&
            strcmp(contactFirstName, firstName) == 0 &&
            strcmp(contactLastName, lastName) == 0) {
            results[foundCount] = current;
            foundCount++;
        }
        
        current = current->next;
    }
    
    return foundCount;
}