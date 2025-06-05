#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "interfaic.h"
#include "DinamicArray.h"
#include "Person.h"

DinamicArray** arrays = NULL;
size_t num_arrays = 0;



void runInterface() {
    while (1) {
        printf("\n=== МЕНЮ ===\n");
        printf("1) Создать новый массив\n");
        printf("2) Добавить персону\n");
        printf("3) Удалить массив\n");
        printf("4) Список массивов\n");
        printf("5) Вывести содержимое массива\n");
        printf("6) Применить map (получить полные имена)\n");
        printf("7) Применить where (только студенты)\n");
        printf("8) Объединить массивы (concat)\n");
        printf("0) Выход\n");
        printf("> ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("[Ошибка] Некорректный ввод\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: //createNewArray(); break;
            case 2: addPersonToArray(); break;
            case 3: //removeArray(); break;
            case 4: //listAllArrays(); break;
            case 5: //printArrayContents(); break;
            case 6: applyMapOperation(); break;
            case 7: //applyWhereOperation(); break;
            case 8: //applyConcatOperation(); break;
            case 0:
                printf("Завершение работы...\n");
                cleanup();
                return;
            default:
                printf("[Ошибка] Неверный пункт меню\n");
        }
    }
}

void* getFullNameWrapper(void* person) {
    Person* p = (Person*)person;
    char* full_name = malloc(150 * sizeof(char));
    snprintf(full_name, 150, "%s %s %s", p->name, p->f_name, p->surname);
    return full_name;
}

void createNewArray() {
    size_t capacity;
    printf("Введите емкость нового массива: ");
    scanf("%zu", &capacity);
    
    DinamicArray* new_arr = createArray(capacity);
    if (!new_arr) {
        printf("Ошибка создания массива\n");
        return;
    }
    
    // Добавляем в глобальный список
    arrays = realloc(arrays, (num_arrays + 1) * sizeof(DinamicArray*));
    arrays[num_arrays++] = new_arr;
    printf("Создан массив #%zu\n", num_arrays);
}

void addPersonToArray() {
    size_t arr_idx;
    printf("Введите номер массива (1-%zu): ", num_arrays);
    scanf("%zu", &arr_idx);
    
    if (arr_idx < 1 || arr_idx > num_arrays) {
        printf("Неверный номер массива\n");
        return;
    }
    char first[50], middle[50], last[50];
    int id;
    time_t birthDate;
    
    printf("Введите имя: ");
    scanf("%49s", first);
    printf("Введите отчество: ");
    scanf("%49s", middle);
    printf("Введите фамилию: ");
    scanf("%49s", last);
    printf("Введите номер зачетки или номер кафедры: ");
    scanf("%d", &id);
    
    Person* p = createPerson(first, middle, last, id, time(NULL));
    pushBack(arrays[arr_idx-1], p);
    printf("Персона добавлена\n");
}

void applyMapOperation() {
    size_t src_idx;
    printf("Введите номер исходного массива: ");
    scanf("%zu", &src_idx);
    
    if (src_idx < 1 || src_idx > num_arrays) {
        printf("Неверный номер массива\n");
        return;
    }
    
    DinamicArray* names = map(arrays[src_idx-1], &getFullNameWrapper);
    arrays = realloc(arrays, (num_arrays + 1) * sizeof(DinamicArray*));
    arrays[num_arrays++] = names;
    printf("Создан новый массив #%zu с именами\n", num_arrays);
}

DinamicArray* createArray(size_t capacity) {
    DinamicArray* arr = (DinamicArray*)malloc(sizeof(DinamicArray));
    arr->data = malloc(capacity * sizeof(Person*));
    arr->size = 0;
    arr->capacity = capacity;
    return arr;
}

void pushBack(DinamicArray* arr, Person* p) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(Person*));
    }
    arr->data[arr->size++] = p;
}

void cleanup() {
    for (size_t i = 0; i < num_arrays; i++) {
        freeArray(arrays[i]);
    }
    free(arrays);
}
