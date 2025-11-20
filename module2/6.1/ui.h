#ifndef UI_H
#define UI_H

#include "phonebook.h"

// функции отображения
void displayContact(const Contact* contact);
void displayContactList(const Phonebook* pb);

// основные обработчики UI
void showMainMenu(void);
void handleAddContact(Phonebook* pb);
void handleEditContact(Phonebook* pb);
void handleDeleteContact(Phonebook* pb);
void handleFindContact(Phonebook* pb);
void handleListContacts(Phonebook* pb);

// вспомогательные функции
void clearInputBuffer(void);
int getIntInput(const char* prompt);
void getStringInput(const char* prompt, char* buffer, int bufferSize);
void printError(const char* message);
void printSuccess(const char* message);

#endif