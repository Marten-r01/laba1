// test_DynamicArray.c
#include "DynamicArray.h"
#include <assert.h>
#include <stdio.h>

// Предикаты для тестирования where
int isStudent(void* data) {
    // В реальной реализации нужно проверять тип
    return 1; // Всегда возвращаем true для теста
}

int isTeacher(void* data) {
    // В реальной реализации нужно проверять тип
    return 1; // Всегда возвращаем true для теста
}

// Функция для map (увеличивает ID на 100)
void* increaseId(void* data) {
    // Определяем тип
    ArrayElement* element = (ArrayElement*)data;
    
    if (element->type == STUDENT_TYPE) {
        Student* s = (Student*)element->data;
        return createStudent(s->base.firstName, s->base.lastName, s->base.id + 100, s->studentId);
    } else {
        Teacher* t = (Teacher*)element->data;
        return createTeacher(t->base.firstName, t->base.lastName, t->base.id + 100, t->department);
    }
}

void testDynamicArray() {
    // Тестирование создания и добавления
    DynamicArray* array = createDynamicArray(2);
    
    Student* s1 = createStudent("Ivan", "Ivanov", 1, "S12345");
    Student* s2 = createStudent("Petr", "Petrov", 2, "S67890");
    Teacher* t1 = createTeacher("Anna", "Sidorova", 101, "Computer Science");
    
    addStudent(array, s1);
    addStudent(array, s2);
    addTeacher(array, t1);
    
    assert(array->size == 3);
    assert(array->capacity >= 3);
    
    // Тестирование where
    DynamicArray* students = where(array, isStudent);
    assert(students->size == 2);
    
    // Тестирование map
    DynamicArray* mapped = map(array, increaseId);
    assert(mapped->size == 3);
    
    // Проверка изменения ID
    Student* ms = (Student*)mapped->elements[0].data;
    assert(ms->base.id == 101);
    
    // Тестирование concat
    DynamicArray* combined = concat(array, mapped);
    assert(combined->size == 6);
    
    // Освобождение памяти
    freeDynamicArray(array);
    freeDynamicArray(students);
    freeDynamicArray(mapped);
    freeDynamicArray(combined);
    
    printf("All tests passed successfully!\n");
}

