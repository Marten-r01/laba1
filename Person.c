// Person.c
#include "Person.h"
#include <string.h>
#include <stdlib.h>

Student* createStudent(const char* firstName, const char* lastName, int id, const char* studentId) {
    Student* student = (Student*)malloc(sizeof(Student));
    student->base.firstName = strdup(firstName);
    student->base.lastName = strdup(lastName);
    student->base.id = id;
    student->studentId = strdup(studentId);
    return student;
}

Teacher* createTeacher(const char* firstName, const char* lastName, int id, const char* department) {
    Teacher* teacher = (Teacher*)malloc(sizeof(Teacher));
    teacher->base.firstName = strdup(firstName);
    teacher->base.lastName = strdup(lastName);
    teacher->base.id = id;
    teacher->department = strdup(department);
    return teacher;
}

void freeStudent(Student* student) {
    if (student) {
        free(student->base.firstName);
        free(student->base.lastName);
        free(student->studentId);
        free(student);
    }
}

void freeTeacher(Teacher* teacher) {
    if (teacher) {
        free(teacher->base.firstName);
        free(teacher->base.lastName);
        free(teacher->department);
        free(teacher);
    }
}