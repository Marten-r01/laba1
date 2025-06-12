#include "DynamicArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функции для операций map
void* map_increaseId(void* data) {
    PersonBase* base = (PersonBase*)data;
    if (base->type == STUDENT_TYPE) {
        Student* s = (Student*)data;
        return createStudent(s->base.firstName, s->base.lastName, s->base.id + 100, s->studentId);
    } else {
        Teacher* t = (Teacher*)data;
        return createTeacher(t->base.firstName, t->base.lastName, t->base.id + 100, t->department);
    }
}

void* map_addTitle(void* data) {
    PersonBase* base = (PersonBase*)data;
    char newFirstName[256];
    
    if (base->type == STUDENT_TYPE) {
        Student* s = (Student*)data;
        snprintf(newFirstName, sizeof(newFirstName), "Student %s", s->base.firstName);
        return createStudent(newFirstName, s->base.lastName, s->base.id, s->studentId);
    } else {
        Teacher* t = (Teacher*)data;
        snprintf(newFirstName, sizeof(newFirstName), "Prof. %s", t->base.firstName);
        return createTeacher(newFirstName, t->base.lastName, t->base.id, t->department);
    }
}

// Предикаты для операций where
int where_students(void* data) {
    PersonBase* base = (PersonBase*)data;
    return base->type == STUDENT_TYPE;
}

int where_teachers(void* data) {
    PersonBase* base = (PersonBase*)data;
    return base->type == TEACHER_TYPE;
}

int where_idGreaterThan(void* data, int threshold) {
    PersonBase* base = (PersonBase*)data;
    return base->id > threshold;
}

// Вспомогательные функции
void printMainMenu() {
    printf("\n=== Main Menu ===\n");
    printf("1. Create new array\n");
    printf("2. Add element\n");
    printf("3. Perform map operation\n");
    printf("4. Perform where operation\n");
    printf("5. Concatenate arrays\n");
    printf("6. Print current array\n");
    printf("7. Print all arrays\n");
    printf("8. Delete array\n");
    printf("9. Exit\n");
    printf("Select option: ");
}

void printMapMenu() {
    printf("\n--- Map Operations ---\n");
    printf("1. Increase ID by 100\n");
    printf("2. Add title to name\n");
    printf("Select map operation: ");
}

void printWhereMenu() {
    printf("\n--- Where Operations ---\n");
    printf("1. Filter students\n");
    printf("2. Filter teachers\n");
    printf("3. Filter by ID greater than\n");
    printf("Select where operation: ");
}

void printArrayMenu() {
    printf("\n--- Array Selection ---\n");
    printf("1. Array A\n");
    printf("2. Array B\n");
    printf("3. Array C\n");
    printf("Select array: ");
}

void addElementToArray(DynamicArray* array) {
    printf("\n--- Add Element ---\n");
    printf("1. Add student\n");
    printf("2. Add teacher\n");
    printf("Select type: ");
    
    int typeChoice;
    scanf("%d", &typeChoice);
    getchar(); // consume newline
    
    char firstName[50], lastName[50], extraInfo[50];
    int id;
    
    printf("Enter first name: ");
    fgets(firstName, sizeof(firstName), stdin);
    firstName[strcspn(firstName, "\n")] = 0;
    
    printf("Enter last name: ");
    fgets(lastName, sizeof(lastName), stdin);
    lastName[strcspn(lastName, "\n")] = 0;
    
    printf("Enter ID: ");
    scanf("%d", &id);
    getchar(); // consume newline
    
    if (typeChoice == 1) {
        printf("Enter student ID: ");
        fgets(extraInfo, sizeof(extraInfo), stdin);
        extraInfo[strcspn(extraInfo, "\n")] = 0;
        
        Student* student = createStudent(firstName, lastName, id, extraInfo);
        addStudent(array, student);
        printf("Student added successfully!\n");
    } else {
        printf("Enter department: ");
        fgets(extraInfo, sizeof(extraInfo), stdin);
        extraInfo[strcspn(extraInfo, "\n")] = 0;
        
        Teacher* teacher = createTeacher(firstName, lastName, id, extraInfo);
        addTeacher(array, teacher);
        printf("Teacher added successfully!\n");
    }
}

void inter() {
    DynamicArray* arrays[3] = {NULL, NULL, NULL};
    int currentArray = 0; // 0: Array A, 1: Array B, 2: Array C
    int choice;
    
    printf("=== Dynamic Array Manager (Students & Teachers) ===\n");
    
    do {
        printMainMenu();
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch(choice) {
            case 1: { // Create new array
                printArrayMenu();
                int arrayChoice;
                scanf("%d", &arrayChoice);
                getchar();
                
                if (arrayChoice >= 1 && arrayChoice <= 3) {
                    int index = arrayChoice - 1;
                    if (arrays[index] != NULL) {
                        freeDynamicArray(arrays[index]);
                    }
                    
                    size_t capacity;
                    printf("Enter initial capacity: ");
                    scanf("%zu", &capacity);
                    getchar();
                    
                    arrays[index] = createDynamicArray(capacity);
                    currentArray = index;
                    printf("Array %c created successfully!\n", 'A' + index);
                } else {
                    printf("Invalid array selection!\n");
                }
                break;
            }
            
            case 2: { // Add element
                if (arrays[currentArray] == NULL) {
                    printf("No array selected! Create an array first.\n");
                    break;
                }
                addElementToArray(arrays[currentArray]);
                break;
            }
            
            case 3: { // Map operation
                if (arrays[currentArray] == NULL) {
                    printf("No array selected!\n");
                    break;
                }
                
                printMapMenu();
                int mapChoice;
                scanf("%d", &mapChoice);
                getchar();
                
                DynamicArray* result = NULL;
                
                switch(mapChoice) {
                    case 1:
                        result = map(arrays[currentArray], map_increaseId);
                        break;
                    case 2:
                        result = map(arrays[currentArray], map_addTitle);
                        break;
                    default:
                        printf("Invalid map operation!\n");
                        break;
                }
                
                if (result) {
                    printf("Result of map operation:\n");
                    printArray(result);
                    
                    // Find next available array
                    int targetArray = -1;
                    for (int i = 0; i < 3; i++) {
                        if (i != currentArray && arrays[i] == NULL) {
                            targetArray = i;
                            break;
                        }
                    }
                    
                    if (targetArray == -1) {
                        printf("No available array to store result. Overwriting current array? (y/n): ");
                        char response;
                        scanf("%c", &response);
                        getchar();
                        
                        if (response == 'y' || response == 'Y') {
                            freeDynamicArray(arrays[currentArray]);
                            arrays[currentArray] = result;
                            printf("Current array replaced with result.\n");
                        } else {
                            freeDynamicArray(result);
                            printf("Result discarded.\n");
                        }
                    } else {
                        arrays[targetArray] = result;
                        printf("Result stored in array %c\n", 'A' + targetArray);
                        currentArray = targetArray;
                    }
                }
                break;
            }
            
            case 4: { // Where operation
                if (arrays[currentArray] == NULL) {
                    printf("No array selected!\n");
                    break;
                }
                
                printWhereMenu();
                int whereChoice;
                scanf("%d", &whereChoice);
                getchar();
                
                DynamicArray* result = NULL;
                
                switch(whereChoice) {
                    case 1:
                        result = where(arrays[currentArray], where_students);
                        break;
                    case 2:
                        result = where(arrays[currentArray], where_teachers);
                        break;
                    case 3: {
                        int threshold;
                        printf("Enter ID threshold: ");
                        scanf("%d", &threshold);
                        getchar();
                        
                        // Wrap threshold in a closure
                        int (*predicate)(void*) = (int (*)(void*)) [](void* data) -> int {
                            return where_idGreaterThan(data, threshold);
                        };
                        
                        result = where(arrays[currentArray], predicate);
                        break;
                    }
                    default:
                        printf("Invalid where operation!\n");
                        break;
                }
                
                if (result) {
                    printf("Result of where operation (%d elements):\n", (int)result->size);
                    printArray(result);
                    
                    // Find next available array
                    int targetArray = -1;
                    for (int i = 0; i < 3; i++) {
                        if (i != currentArray && arrays[i] == NULL) {
                            targetArray = i;
                            break;
                        }
                    }
                    
                    if (targetArray == -1) {
                        printf("No available array to store result. Overwriting current array? (y/n): ");
                        char response;
                        scanf("%c", &response);
                        getchar();
                        
                        if (response == 'y' || response == 'Y') {
                            freeDynamicArray(arrays[currentArray]);
                            arrays[currentArray] = result;
                            printf("Current array replaced with result.\n");
                        } else {
                            freeDynamicArray(result);
                            printf("Result discarded.\n");
                        }
                    } else {
                        arrays[targetArray] = result;
                        printf("Result stored in array %c\n", 'A' + targetArray);
                        currentArray = targetArray;
                    }
                }
                break;
            }
            
            case 5: { // Concatenate
                printArrayMenu();
                printf("Select first array: ");
                int array1;
                scanf("%d", &array1);
                getchar();
                
                printArrayMenu();
                printf("Select second array: ");
                int array2;
                scanf("%d", &array2);
                getchar();
                
                if (array1 < 1 || array1 > 3 || array2 < 1 || array2 > 3) {
                    printf("Invalid array selection!\n");
                    break;
                }
                
                DynamicArray* arr1 = arrays[array1 - 1];
                DynamicArray* arr2 = arrays[array2 - 1];
                
                if (!arr1 || !arr2) {
                    printf("One or both arrays are not initialized!\n");
                    break;
                }
                
                DynamicArray* result = concat(arr1, arr2);
                
                // Find next available array
                int targetArray = -1;
                for (int i = 0; i < 3; i++) {
                    if (arrays[i] == NULL) {
                        targetArray = i;
                        break;
                    }
                }
                
                if (targetArray == -1) {
                    printf("No available array to store result. Overwrite an array? (1=A, 2=B, 3=C, 0=cancel): ");
                    int overwrite;
                    scanf("%d", &overwrite);
                    getchar();
                    
                    if (overwrite >= 1 && overwrite <= 3) {
                        freeDynamicArray(arrays[overwrite - 1]);
                        arrays[overwrite - 1] = result;
                        currentArray = overwrite - 1;
                        printf("Array %c replaced with concatenation result.\n", 'A' + overwrite - 1);
                    } else {
                        freeDynamicArray(result);
                        printf("Operation canceled. Result discarded.\n");
                    }
                } else {
                    arrays[targetArray] = result;
                    currentArray = targetArray;
                    printf("Concatenation result stored in array %c\n", 'A' + targetArray);
                }
                break;
            }
            
            case 6: { // Print current array
                if (arrays[currentArray] == NULL) {
                    printf("Current array is empty!\n");
                } else {
                    printf("\n=== Array %c (%zu elements) ===\n", 
                           'A' + currentArray, arrays[currentArray]->size);
                    printArray(arrays[currentArray]);
                }
                break;
            }
            
            case 7: { // Print all arrays
                for (int i = 0; i < 3; i++) {
                    if (arrays[i]) {
                        printf("\n=== Array %c (%zu elements) ===\n", 'A' + i, arrays[i]->size);
                        printArray(arrays[i]);
                    } else {
                        printf("\nArray %c: Not initialized\n", 'A' + i);
                    }
                }
                break;
            }
            
            case 8: { // Delete array
                printArrayMenu();
                int arrayChoice;
                scanf("%d", &arrayChoice);
                getchar();
                
                if (arrayChoice >= 1 && arrayChoice <= 3) {
                    int index = arrayChoice - 1;
                    if (arrays[index]) {
                        freeDynamicArray(arrays[index]);
                        arrays[index] = NULL;
                        printf("Array %c deleted.\n", 'A' + index);
                        
                        if (currentArray == index) {
                            // Switch to first available array
                            currentArray = 0;
                            for (int i = 0; i < 3; i++) {
                                if (arrays[i]) {
                                    currentArray = i;
                                    break;
                                }
                            }
                        }
                    } else {
                        printf("Array %c is already empty.\n", 'A' + index);
                    }
                } else {
                    printf("Invalid array selection!\n");
                }
                break;
            }
            
            case 9: // Exit
                printf("Exiting program...\n");
                break;
                
            default:
                printf("Invalid option! Please try again.\n");
        }
    } while (choice != 9);
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (arrays[i]) {
            freeDynamicArray(arrays[i]);
        }
    }
}