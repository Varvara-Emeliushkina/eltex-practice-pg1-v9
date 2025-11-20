#ifndef CONTACT_H
#define CONTACT_H

#define MAX_PHONES 5
#define MAX_EMAILS 5
#define MAX_SOCIALS 5
#define MAX_STRING_LENGTH 100

// вложенная структура для соцсетей
typedef struct {
    char platform[MAX_STRING_LENGTH];
    char profile[MAX_STRING_LENGTH];
} SocialMedia;

// структура контакта
typedef struct Contact {
    unsigned int uid;
    char* firstName; // динамически
    char* lastName; // динамически  
    char workplace[MAX_STRING_LENGTH];
    char position[MAX_STRING_LENGTH];
    char phones[MAX_PHONES][MAX_STRING_LENGTH];
    int phoneCount;
    char emails[MAX_EMAILS][MAX_STRING_LENGTH];
    int emailCount;
    SocialMedia socials[MAX_SOCIALS];
    int socialCount;
    int profileCount;
    
    struct Contact* prev;
    struct Contact* next;
} Contact;

// функции создания и управления контактом
Contact* createContact(const char* firstName, const char* lastName);
void freeContact(Contact* contact);

// функции добавления данных
int addPhoneToContact(Contact* contact, const char* phone);
int addEmailToContact(Contact* contact, const char* email);
int addSocialToContact(Contact* contact, const char* platform, const char* profile);

// функции удаления данных
int removePhoneFromContact(Contact* contact, int index);
int removeEmailFromContact(Contact* contact, int index);
int removeSocialFromContact(Contact* contact, int index);

// геттеры
unsigned int getContactUid(const Contact* contact);
const char* getContactFirstName(const Contact* contact);
const char* getContactLastName(const Contact* contact);
const char* getContactWorkplace(const Contact* contact);
const char* getContactPosition(const Contact* contact);
int getContactPhoneCount(const Contact* contact);
const char* getContactPhone(const Contact* contact, int index);
int getContactEmailCount(const Contact* contact);
const char* getContactEmail(const Contact* contact, int index);
int getContactSocialCount(const Contact* contact);
int getContactProfileCount(const Contact* contact);
const char* getContactSocialPlatform(const Contact* contact, int index);
const char* getContactSocialProfile(const Contact* contact, int index);

// сеттеры
void setContactUid(Contact* contact, unsigned int uid);
void setContactWorkplace(Contact* contact, const char* workplace);
void setContactPosition(Contact* contact, const char* position);
int setContactFirstName(Contact* contact, const char* firstName);
int setContactLastName(Contact* contact, const char* lastName);

#endif