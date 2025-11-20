# Как и что поменяли в задании 2.1  
**Доработать решение задачи 2.1 (список контактов) так, чтобы
для хранения данных использовался двухсвязный упорядоченный список.**  

**1. contact.h**  
Убираем MAX_CONTACTS (теперь ограничение только по памяти)  
Добавляем указатели prev и next в структуру Contact для двухсвязного списка
```c
    struct Contact* prev;
    struct Contact* next;
```

**2. phonebook.h**  
Заменили массив контактов на указатели head и tail  
*было:*  
```c
typedef struct Phonebook {
    Contact* contacts[MAX_CONTACTS];
    int count;
    unsigned int nextUid;
} Phonebook;

```
*стало:*
```c
typedef struct Phonebook {
    Contact* head;
    Contact* tail;
    int count;
    unsigned int nextUid;
} Phonebook;
```
Новые функции для работы со списком:
- insertContactSorted() - вставка в упорядоченный список
- findContactByUidInList() - рекурсивный поиск
- compareContacts() - сравнение контактов для сортировки  
```c
void insertContactSorted(Phonebook* pb, Contact* newContact);
Contact* findContactByUidInList(const Contact* head, unsigned int uid);
int compareContacts(const Contact* a, const Contact* b);
```

**3. phonebook.c**  
initPhonebook(): Инициализация указателей списка  
```c
void initPhonebook(Phonebook* pb) {
    if (pb == NULL) return;
    
    pb->head = NULL;
    pb->tail = NULL;
    pb->count = 0;
    pb->nextUid = 1;
}
```
freePhonebook(): Освобождение памяти с обходом всего списка  
```c
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
```
insertContactSorted(): Вставка контакта в алфавитном порядке (по фамилии, затем по имени)  
```c
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
```
compareContacts(): Функция сравнения для сортировки  
```c
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
```

deleteContactFromPhonebook(): Обновление связей при удалении  
```c
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
```
findContactByUidInList(): Рекурсивный поиск по UID  
```c
Contact* findContactByUidInList(const Contact* head, unsigned int uid) {
    if (head == NULL) return NULL;
    
    if (getContactUid(head) == uid) {
        return (Contact*)head;
    }
    
    return findContactByUidInList(head->next, uid);
}
```

**4. contact.c**  
Добавили инициализацию указателей prev и next в createContact()
```c
    contact->prev = NULL;
    contact->next = NULL;
```

## Какие вообще плюсы у такой реализации?
1. Динамическое расширение (нет ограничения кол-ва контактов)
2. Автоматическая сортировака (контакты упорядочены по фамилии и имени)
3. Меньшая трудоемкость
  
    
gcc -Wall -Wextra -std=c99 -o phonebook.exe main.c contact.c phonebook.c ui.c