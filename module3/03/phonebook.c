#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "phonebook.h"

void initPhonebook(Phonebook* pb) {
    if (pb == NULL) return;
    
    pb->count = 0;
    pb->nextUid = 1;
    
    for (int i = 0; i < MAX_CONTACTS; i++) {
        pb->contacts[i] = NULL;
    }
}

void freePhonebook(Phonebook* pb) {
    if (pb == NULL) return;
    
    for (int i = 0; i < pb->count; i++) {
        if (pb->contacts[i] != NULL) {
            freeContact(pb->contacts[i]);
        }
    }
}

// функция загрузки телефонной книги из файла (небуферизованный ввод)
int loadPhonebookFromFile(Phonebook* pb, const char* filename) {
    if (pb == NULL || filename == NULL) return 0;
    
    // открываем файл для чтения
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        // файл не существует это ок при первом запуске
        return 1;
    }
    
    // считываем количество контактов
    int count;
    ssize_t bytes_read = read(fd, &count, sizeof(int));
    if (bytes_read != sizeof(int)) {
        close(fd);
        return 0;
    }
    
    // считываем nextUid
    bytes_read = read(fd, &pb->nextUid, sizeof(unsigned int));
    if (bytes_read != sizeof(unsigned int)) {
        close(fd);
        return 0;
    }
    
    // считываем контакты
    for (int i = 0; i < count && i < MAX_CONTACTS; i++) {
        Contact* contact = (Contact*)malloc(sizeof(Contact));
        if (contact == NULL) {
            close(fd);
            return 0;
        }
        
        bytes_read = read(fd, contact, sizeof(Contact));
        if (bytes_read != sizeof(Contact)) {
            free(contact);
            close(fd);
            return 0;
        }
        
        pb->contacts[i] = contact;
        pb->count++;
    }
    
    close(fd);
    return 1;
}

// функция сохранения телефонной книги в файл (небуферизованный вывод)
int savePhonebookToFile(const Phonebook* pb, const char* filename) {
    if (pb == NULL || filename == NULL) return 0;
    
    // открываем файл для записи (создаем, если не существует)
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return 0;
    }
    
    // записываем количество контактов
    ssize_t bytes_written = write(fd, &pb->count, sizeof(int));
    if (bytes_written != sizeof(int)) {
        close(fd);
        return 0;
    }
    
    // записываем nextUid
    bytes_written = write(fd, &pb->nextUid, sizeof(unsigned int));
    if (bytes_written != sizeof(unsigned int)) {
        close(fd);
        return 0;
    }
    
    // записываем контакты
    for (int i = 0; i < pb->count; i++) {
        if (pb->contacts[i] != NULL) {
            bytes_written = write(fd, pb->contacts[i], sizeof(Contact));
            if (bytes_written != sizeof(Contact)) {
                close(fd);
                return 0;
            }
        }
    }
    
    close(fd);
    return 1;
}

// функция с переменным числом параметров
int addContactToPhonebook(Phonebook* pb, const char* firstName, const char* lastName, ...) {
    if (pb == NULL || firstName == NULL || lastName == NULL) return 0;
    if (pb->count >= MAX_CONTACTS) return 0;
    
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
    pb->contacts[pb->count] = newContact;
    pb->count++;
    return 1;
}

int deleteContactFromPhonebook(Phonebook* pb, unsigned int uid) {
    if (pb == NULL) return 0;
    
    for (int i = 0; i < pb->count; i++) {
        if (pb->contacts[i] != NULL && getContactUid(pb->contacts[i]) == uid) {
            freeContact(pb->contacts[i]);
            
            // сдвигаем контакты
            for (int j = i; j < pb->count - 1; j++) {
                pb->contacts[j] = pb->contacts[j + 1];
            }
            
            pb->contacts[pb->count - 1] = NULL;
            pb->count--;
            return 1;
        }
    }
    return 0;
}

Contact* findContactByUid(const Phonebook* pb, unsigned int uid) {
    if (pb == NULL) return NULL;
    
    for (int i = 0; i < pb->count; i++) {
        if (pb->contacts[i] != NULL && getContactUid(pb->contacts[i]) == uid) {
            return pb->contacts[i];
        }
    }
    return NULL;
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
    return pb->contacts[index];
}

// функция поиска всех контактов с одинаковым именем
int findAllContactsByName(const Phonebook* pb, const char* firstName, const char* lastName, 
                         Contact* results[], int maxResults) {
    if (pb == NULL || firstName == NULL || lastName == NULL || results == NULL || maxResults <= 0) {
        return 0;
    }
    
    int foundCount = 0;
    
    for (int i = 0; i < pb->count && foundCount < maxResults; i++) {
        if (pb->contacts[i] != NULL) {
            const char* contactFirstName = getContactFirstName(pb->contacts[i]);
            const char* contactLastName = getContactLastName(pb->contacts[i]);
            
            if (contactFirstName && contactLastName &&
                strcmp(contactFirstName, firstName) == 0 &&
                strcmp(contactLastName, lastName) == 0) {
                results[foundCount] = pb->contacts[i];
                foundCount++;
            }
        }
    }
    
    return foundCount;
}