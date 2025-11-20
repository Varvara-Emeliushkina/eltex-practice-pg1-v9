#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

Contact* createContact(const char* firstName, const char* lastName) {
    if (firstName == NULL || lastName == NULL) return NULL;
    
    Contact* contact = (Contact*)malloc(sizeof(Contact));
    if (contact == NULL) return NULL;
    
    // инициализируем указатели как NULL
    contact->firstName = NULL;
    contact->lastName = NULL;
    
    contact->firstName = malloc(strlen(firstName) + 1);
    if (contact->firstName == NULL) {
        free(contact);
        return NULL;
    }
    
    contact->lastName = malloc(strlen(lastName) + 1);
    if (contact->lastName == NULL) {
        free(contact->firstName);
        free(contact);
        return NULL;
    }
    
    strcpy(contact->firstName, firstName);
    strcpy(contact->lastName, lastName);

    // инициализация остальных полей
    contact->uid = 0;
    contact->workplace[0] = '\0';
    contact->position[0] = '\0';
    contact->phoneCount = 0;
    contact->emailCount = 0;
    contact->socialCount = 0;
    contact->profileCount = 0;
    
    contact->left = NULL;
    contact->right = NULL;
    
    return contact;
}

void freeContact(Contact* contact) {
    if (contact == NULL) return;
    
    free(contact->firstName);
    free(contact->lastName);
    free(contact);
}

int addPhoneToContact(Contact* contact, const char* phone) {
    if (contact == NULL || phone == NULL) return 0;
    if (contact->phoneCount >= MAX_PHONES) return 0;
    
    strncpy(contact->phones[contact->phoneCount], phone, MAX_STRING_LENGTH - 1);
    contact->phones[contact->phoneCount][MAX_STRING_LENGTH - 1] = '\0';
    contact->phoneCount++;
    return 1;
}

int addEmailToContact(Contact* contact, const char* email) {
    if (contact == NULL || email == NULL) return 0;
    if (contact->emailCount >= MAX_EMAILS) return 0;
    
    strncpy(contact->emails[contact->emailCount], email, MAX_STRING_LENGTH - 1);
    contact->emails[contact->emailCount][MAX_STRING_LENGTH - 1] = '\0';
    contact->emailCount++;
    return 1;
}

int addSocialToContact(Contact* contact, const char* platform, const char* profile) {
    if (contact == NULL || platform == NULL || profile == NULL) return 0;
    if (contact->socialCount >= MAX_SOCIALS) return 0;
    
    strncpy(contact->socials[contact->socialCount].platform, platform, MAX_STRING_LENGTH - 1);
    contact->socials[contact->socialCount].platform[MAX_STRING_LENGTH - 1] = '\0';
    
    strncpy(contact->socials[contact->socialCount].profile, profile, MAX_STRING_LENGTH - 1);
    contact->socials[contact->socialCount].profile[MAX_STRING_LENGTH - 1] = '\0';
    
    contact->socialCount++;
    return 1;
}

int removePhoneFromContact(Contact* contact, int index) {
    if (contact == NULL || index < 0 || index >= contact->phoneCount) return 0;
    
    for (int i = index; i < contact->phoneCount - 1; i++) {
        strncpy(contact->phones[i], contact->phones[i + 1], MAX_STRING_LENGTH - 1);
        contact->phones[i][MAX_STRING_LENGTH - 1] = '\0';
    }
    
    if (contact->phoneCount > 0) {
        contact->phones[contact->phoneCount - 1][0] = '\0';
    }
    
    contact->phoneCount--;
    return 1;
}

int removeEmailFromContact(Contact* contact, int index) {
    if (contact == NULL || index < 0 || index >= contact->emailCount) return 0;
    
    for (int i = index; i < contact->emailCount - 1; i++) {
        strncpy(contact->emails[i], contact->emails[i + 1], MAX_STRING_LENGTH - 1);
        contact->emails[i][MAX_STRING_LENGTH - 1] = '\0';
    }
    
    if (contact->emailCount > 0) {
        contact->emails[contact->emailCount - 1][0] = '\0';
    }
    
    contact->emailCount--;
    return 1;
}

int removeSocialFromContact(Contact* contact, int index) {
    if (contact == NULL || index < 0 || index >= contact->socialCount) return 0;
    
    for (int i = index; i < contact->socialCount - 1; i++) {
        strncpy(contact->socials[i].platform, contact->socials[i + 1].platform, MAX_STRING_LENGTH - 1);
        contact->socials[i].platform[MAX_STRING_LENGTH - 1] = '\0';
        
        strncpy(contact->socials[i].profile, contact->socials[i + 1].profile, MAX_STRING_LENGTH - 1);
        contact->socials[i].profile[MAX_STRING_LENGTH - 1] = '\0';
    }
    
    if (contact->socialCount > 0) {
        contact->socials[contact->socialCount - 1].platform[0] = '\0';
        contact->socials[contact->socialCount - 1].profile[0] = '\0';
    }
    
    contact->socialCount--;
    return 1;
}

unsigned int getContactUid(const Contact* contact) {
    return contact ? contact->uid : 0;
}

const char* getContactFirstName(const Contact* contact) {
    return contact ? contact->firstName : NULL;
}

const char* getContactLastName(const Contact* contact) {
    return contact ? contact->lastName : NULL;
}

const char* getContactWorkplace(const Contact* contact) {
    return contact ? contact->workplace : NULL;
}

const char* getContactPosition(const Contact* contact) {
    return contact ? contact->position : NULL;
}

int getContactPhoneCount(const Contact* contact) {
    return contact ? contact->phoneCount : 0;
}

const char* getContactPhone(const Contact* contact, int index) {
    if (contact == NULL || index < 0 || index >= contact->phoneCount) return NULL;
    return contact->phones[index];
}

int getContactEmailCount(const Contact* contact) {
    return contact ? contact->emailCount : 0;
}

const char* getContactEmail(const Contact* contact, int index) {
    if (contact == NULL || index < 0 || index >= contact->emailCount) return NULL;
    return contact->emails[index];
}

int getContactSocialCount(const Contact* contact) {
    return contact ? contact->socialCount : 0;
}

int getContactProfileCount(const Contact* contact) {
    return contact ? contact->profileCount : 0;
}

const char* getContactSocialPlatform(const Contact* contact, int index) {
    if (contact == NULL || index < 0 || index >= contact->socialCount) return NULL;
    return contact->socials[index].platform;
}

const char* getContactSocialProfile(const Contact* contact, int index) {
    if (contact == NULL || index < 0 || index >= contact->socialCount) return NULL;
    return contact->socials[index].profile;
}

void setContactUid(Contact* contact, unsigned int uid) {
    if (contact) contact->uid = uid;
}

void setContactWorkplace(Contact* contact, const char* workplace) {
    if (contact && workplace) {
        strncpy(contact->workplace, workplace, MAX_STRING_LENGTH - 1);
        contact->workplace[MAX_STRING_LENGTH - 1] = '\0';
    }
}

void setContactPosition(Contact* contact, const char* position) {
    if (contact && position) {
        strncpy(contact->position, position, MAX_STRING_LENGTH - 1);
        contact->position[MAX_STRING_LENGTH - 1] = '\0';
    }
}

int setContactFirstName(Contact* contact, const char* firstName) {
    if (contact == NULL || firstName == NULL || strlen(firstName) == 0) return 0;
    
    char* newFirstName = malloc(strlen(firstName) + 1);
    if (newFirstName == NULL) return 0;
    
    free(contact->firstName);
    contact->firstName = newFirstName;
    strcpy(contact->firstName, firstName);
    return 1;
}

int setContactLastName(Contact* contact, const char* lastName) {
    if (contact == NULL || lastName == NULL || strlen(lastName) == 0) return 0;
    
    char* newLastName = malloc(strlen(lastName) + 1);
    if (newLastName == NULL) return 0;
    
    free(contact->lastName);
    contact->lastName = newLastName;
    strcpy(contact->lastName, lastName);
    return 1;
}