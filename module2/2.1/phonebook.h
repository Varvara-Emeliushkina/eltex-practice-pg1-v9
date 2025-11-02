#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "contact.h"

typedef struct Phonebook {
    Contact* contacts[MAX_CONTACTS];
    int count;
    unsigned int nextUid;
} Phonebook;

// функции управления телефонной книгой
void initPhonebook(Phonebook* pb);
void freePhonebook(Phonebook* pb);

// функция с переменным числом параметров
int addContactToPhonebook(Phonebook* pb, const char* firstName, const char* lastName, ...);

// функции работы с контактами
int deleteContactFromPhonebook(Phonebook* pb, unsigned int uid);
Contact* findContactByUid(const Phonebook* pb, unsigned int uid);
Contact* findContactByName(const Phonebook* pb, const char* firstName, const char* lastName);
int getPhonebookContactCount(const Phonebook* pb);
Contact* getContactByIndex(const Phonebook* pb, int index);

// функция поиска всех контактов с одинаковым именем
int findAllContactsByName(const Phonebook* pb, const char* firstName, const char* lastName, 
                         Contact* results[], int maxResults);

#endif