#ifndef INTERFAIC_H
#define INTERFAIC_H

#include "DinamicArray.h"  // Добавьте этот include
#include "Person.h"

void runInterface();
DinamicArray* createArray(size_t capacity);
Person* createPerson(const char* first, const char* middle, const char* last, int id, time_t birth_date);
void pushBack(DinamicArray* arr, Person* p);
DinamicArray* map(DinamicArray* arr, void* (*func)(void*));
void* getFullNameWrapper(void* person);
void addPersonToArray();
void applyMapOperation();
void cleanup();

#endif