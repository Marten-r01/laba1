#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
typedef struct{
    void** data;
    size_t size;
    size_t capacity;
}DinamicArray;
DinamicArray* createDinamicArray(size_t capacity){
    DinamicArray* array = malloc(sizeof(DinamicArray));
    if (!array) return NULL;
    
    array->data = (void**)malloc(capacity * sizeof(void*));
    if (!array->data) {
        free(array);
        return NULL;
    }
    
    array->size = 0;
    array->capacity = capacity;
    return array;
}
void freeDinamicArray(DinamicArray* array){
    if (!array) return;
    free(array->data);
    free(array);
}
void pushback(DinamicArray* array, void* element){
    if (!array || !element) return;

    if (array->size >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        void** new_data = (void**)realloc(array->data, new_capacity * sizeof(void*));
        if (!new_data) return; 
        
        array->data = new_data;
        array->capacity = new_capacity;
    }
    
    array->data[array->size++] = element;
}
void* getElement(DinamicArray* array, size_t index){
    if (!array || index >= array->size) return NULL;
    return array->data[index];
}
DinamicArray* map(DinamicArray* array, void* (*func)(void*)){
    DinamicArray* result = createDinamicArray(array->size);
    for (size_t i =0; i<array->size; i++){
        pushback(result,func(getElement(array, i)));
    }
    return result;
}
DinamicArray* where (DinamicArray* array, bool (*predicate)(void*)){
    DinamicArray* result = createDinamicArray(array->size);
    for (size_t i=0; i<array->size;i++){
        if (predicate(getElement(array, i))){
            pushback(result, getElement(array,i));
        }
    }
    return result;
}
DinamicArray* concat(DinamicArray* array1, DinamicArray* array2) {
    DinamicArray* result = createDinamicArray(array1->size + array2->size);
    for (size_t i = 0; i < array1->size; i++) {
        pushback(result, getElement(array1, i));
    }
    for (size_t i = 0; i < array2->size; i++) {
        pushback(result, getElement(array2, i));
    }
    return result;
}

void freeArray(DinamicArray* arr) {
    if (arr) {
        // Освобождаем каждую персону в массиве
        for (size_t i = 0; i < arr->size; i++) {
            free(arr->data[i]);
        }
        // Освобождаем сам массив данных
        free(arr->data);
        // Освобождаем структуру массива
        free(arr);
    }
}
#endif