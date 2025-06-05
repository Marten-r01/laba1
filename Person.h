#ifndef PERSON_H
#define PERSON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct {
    char* name ;
    char* surname ;
    char* f_name;
    int id;
    time_t birthDate;
}Person;
Person* createPerson(const char* first, const char* middle, const char* last, int id, time_t birth_date) {
    Person* p = (Person*)malloc(sizeof(Person));
    if (!p) return NULL;

    p->name = strdup(first);
    p->surname = strdup(middle);
    p->f_name = strdup(last);
    p->id = id;
    p->birthDate = birth_date;
    
    return p;
}
int IsStudent (void* person){
    Person* p = (Person*)person;
    if (p->id>100) return 1;
    return 0;
}
char* getFullName(const Person* person) {
    if (!person) return NULL;
    
    size_t len = strlen(person->name) + strlen(person->surname) + 
                strlen(person->f_name) + 3; 
    char* fullName = (char*)malloc(len);
    
    snprintf(fullName, len, "%s %s %s", 
             person->name, person->surname, person->f_name);
    return fullName;
}
void* personToFullName(void* person) {
    Person* p = (Person*)person;
    return getFullName(p);
}
void freePerson(Person* person) {
    if (!person) return;
    
    free(person->name);
    free(person->surname);
    free(person->f_name);
    free(person);
}
void printPerson(const Person* person) {
    if (!person) return;
    
    char* fullName = getFullName(person);
    printf("Person: %s\n", fullName);
    char birthDateStr[20];
    strftime(birthDateStr, 20, "%d.%m.%Y", localtime(&person->birthDate));
    printf("Birth date: %s\n", birthDateStr);
    if (IsStudent((Person*)person)==1)
        printf("record number: %d\n", person->id);
    else printf("kafedra: %d\n", person->id);
    free(fullName);
}
#endif