// Person.h
#ifndef PERSON_H
#define PERSON_H

#include <stdlib.h>

typedef enum {
    STUDENT,
    TEACHER
} PersonType;

typedef struct {
    char* firstName;
    char* lastName;
    int id;
} PersonBase;

typedef struct {
    PersonBase base;
    char* studentId;
} Student;

typedef struct {
    PersonBase base;
    char* department;
} Teacher;

// Создание объектов
Student* createStudent(const char* firstName, const char* lastName, int id, const char* studentId);
Teacher* createTeacher(const char* firstName, const char* lastName, int id, const char* department);

// Освобождение памяти
void freeStudent(Student* student);
void freeTeacher(Teacher* teacher);

#endif // PERSON_H