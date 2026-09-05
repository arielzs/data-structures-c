#ifndef ARRAY_H
#define ARRAY_H

/* Ponto de entrada do submenu de arrays, chamado pelo menu.c */
void runArrayMenu(void);

typedef struct
{
    int *data;
    int size;
    int capacity;
} Array;

void arrayCreate(Array *array);
void arrayClear(Array *array);
int arrayIsEmpty(Array *array);
int arraySize(Array *array);
void arrayPush(Array *array, int data);
void arrayInsert(Array *array, int index, int data);
int arrayRemoveAt(Array *array, int index);
int arrayGet(Array *array, int index);
void arraySet(Array *array, int index, int data);
int arrayContains(Array *array, int data);
int arrayCountOccurrences(Array *array, int data);
void arrayPrint(Array *array);
void arrayCopy(Array *destination, Array *source);
int arrayCompare(Array *array1, Array *array2);
void arrayRemoveOccurrences(Array *array, int data);
void arrayRemoveDuplicates(Array *array);
void arrayReverse(Array *array);

#endif