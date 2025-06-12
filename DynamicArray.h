// DynamicArray.h
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>
#include "Person.h"

typedef enum {
    STUDENT_TYPE,
    TEACHER_TYPE
} ElementType;

typedef struct {
    ElementType type;
    void* data;
} ArrayElement;

typedef struct {
    ArrayElement* elements;
    size_t size;
    size_t capacity;
} DynamicArray;

// Создание и уничтожение
DynamicArray* createDynamicArray(size_t initialCapacity);
void freeDynamicArray(DynamicArray* array);

// Добавление элементов
void addStudent(DynamicArray* array, Student* student);
void addTeacher(DynamicArray* array, Teacher* teacher);

// Операции
DynamicArray* map(const DynamicArray* array, void* (*mapper)(void*));
DynamicArray* where(const DynamicArray* array, int (*predicate)(void*));
DynamicArray* concat(const DynamicArray* array1, const DynamicArray* array2);

// Вспомогательные функции
void printArray(const DynamicArray* array);

#endif // DYNAMIC_ARRAY_H