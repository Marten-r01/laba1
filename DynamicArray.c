// DynamicArray.c
#include "DynamicArray.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

DynamicArray* createDynamicArray(size_t initialCapacity) {
    DynamicArray* array = (DynamicArray*)malloc(sizeof(DynamicArray));
    array->elements = (ArrayElement*)malloc(initialCapacity * sizeof(ArrayElement));
    array->size = 0;
    array->capacity = initialCapacity;
    return array;
}

void freeDynamicArray(DynamicArray* array) {
    if (array) {
        for (size_t i = 0; i < array->size; i++) {
            if (array->elements[i].type == STUDENT_TYPE) {
                freeStudent((Student*)array->elements[i].data);
            } else {
                freeTeacher((Teacher*)array->elements[i].data);
            }
        }
        free(array->elements);
        free(array);
    }
}

static void ensureCapacity(DynamicArray* array) {
    if (array->size >= array->capacity) {
        size_t newCapacity = array->capacity * 2;
        ArrayElement* newElements = (ArrayElement*)realloc(array->elements, newCapacity * sizeof(ArrayElement));
        if (newElements) {
            array->elements = newElements;
            array->capacity = newCapacity;
        }
    }
}

void addStudent(DynamicArray* array, Student* student) {
    ensureCapacity(array);
    array->elements[array->size].type = STUDENT_TYPE;
    array->elements[array->size].data = student;
    array->size++;
}

void addTeacher(DynamicArray* array, Teacher* teacher) {
    ensureCapacity(array);
    array->elements[array->size].type = TEACHER_TYPE;
    array->elements[array->size].data = teacher;
    array->size++;
}

DynamicArray* map(const DynamicArray* array, void* (*mapper)(void*)) {
    DynamicArray* result = createDynamicArray(array->size);
    for (size_t i = 0; i < array->size; i++) {
        void* mapped = mapper(array->elements[i].data);
        
        // Определяем тип результата (предполагаем, что тип сохраняется)
        if (array->elements[i].type == STUDENT_TYPE) {
            addStudent(result, (Student*)mapped);
        } else {
            addTeacher(result, (Teacher*)mapped);
        }
    }
    return result;
}

DynamicArray* where(const DynamicArray* array, int (*predicate)(void*)) {
    DynamicArray* result = createDynamicArray(array->size);
    for (size_t i = 0; i < array->size; i++) {
        if (predicate(array->elements[i].data)) {
            // Копируем элемент (поверхностное копирование)
            ArrayElement element = array->elements[i];
            
            // Глубокое копирование элемента
            void* newData = NULL;
            if (element.type == STUDENT_TYPE) {
                Student* orig = (Student*)element.data;
                newData = createStudent(orig->base.firstName, orig->base.lastName, orig->base.id, orig->studentId);
            } else {
                Teacher* orig = (Teacher*)element.data;
                newData = createTeacher(orig->base.firstName, orig->base.lastName, orig->base.id, orig->department);
            }
            
            if (element.type == STUDENT_TYPE) {
                addStudent(result, (Student*)newData);
            } else {
                addTeacher(result, (Teacher*)newData);
            }
        }
    }
    return result;
}

DynamicArray* concat(const DynamicArray* array1, const DynamicArray* array2) {
    size_t totalSize = array1->size + array2->size;
    DynamicArray* result = createDynamicArray(totalSize);
    
    // Копируем элементы из первого массива
    for (size_t i = 0; i < array1->size; i++) {
        ArrayElement element = array1->elements[i];
        void* newData = NULL;
        
        if (element.type == STUDENT_TYPE) {
            Student* orig = (Student*)element.data;
            newData = createStudent(orig->base.firstName, orig->base.lastName, orig->base.id, orig->studentId);
            addStudent(result, (Student*)newData);
        } else {
            Teacher* orig = (Teacher*)element.data;
            newData = createTeacher(orig->base.firstName, orig->base.lastName, orig->base.id, orig->department);
            addTeacher(result, (Teacher*)newData);
        }
    }
    
    // Копируем элементы из второго массива
    for (size_t i = 0; i < array2->size; i++) {
        ArrayElement element = array2->elements[i];
        void* newData = NULL;
        
        if (element.type == STUDENT_TYPE) {
            Student* orig = (Student*)element.data;
            newData = createStudent(orig->base.firstName, orig->base.lastName, orig->base.id, orig->studentId);
            addStudent(result, (Student*)newData);
        } else {
            Teacher* orig = (Teacher*)element.data;
            newData = createTeacher(orig->base.firstName, orig->base.lastName, orig->base.id, orig->department);
            addTeacher(result, (Teacher*)newData);
        }
    }
    
    return result;
}

void printArray(const DynamicArray* array) {
    for (size_t i = 0; i < array->size; i++) {
        if (array->elements[i].type == STUDENT_TYPE) {
            Student* s = (Student*)array->elements[i].data;
            printf("Student: %s %s (ID: %d, StudentID: %s)\n", 
                   s->base.firstName, s->base.lastName, s->base.id, s->studentId);
        } else {
            Teacher* t = (Teacher*)array->elements[i].data;
            printf("Teacher: %s %s (ID: %d, Department: %s)\n", 
                   t->base.firstName, t->base.lastName, t->base.id, t->department);
        }
    }
    printf("\n");
}