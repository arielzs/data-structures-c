#ifndef ARRAY_H
#define ARRAY_H

/* Ponto de entrada do submenu de arrays, chamado pelo menu.c */
void runArrayMenu(void);

/* ===================== Array Dinamico (em dynamic_array.c) ===================== */

typedef struct
{
    int *data;
    int size;
    int capacity;
} DynamicArray;

void dynamicArrayCreate(DynamicArray *array);
void dynamicArrayClear(DynamicArray *array);
int dynamicArrayIsEmpty(DynamicArray *array);
int dynamicArraySize(DynamicArray *array);
void dynamicArrayPush(DynamicArray *array, int data);
void dynamicArrayInsert(DynamicArray *array, int index, int data);
int dynamicArrayRemoveAt(DynamicArray *array, int index);
int dynamicArrayGet(DynamicArray *array, int index);
void dynamicArraySet(DynamicArray *array, int index, int data);
int dynamicArrayContains(DynamicArray *array, int data);
int dynamicArrayCountOccurrences(DynamicArray *array, int data);
void dynamicArrayPrint(DynamicArray *array);
void dynamicArrayCopy(DynamicArray *destination, DynamicArray *source);
int dynamicArrayCompare(DynamicArray *array1, DynamicArray *array2);
void dynamicArrayRemoveOccurrences(DynamicArray *array, int data);
void dynamicArrayRemoveDuplicates(DynamicArray *array);
void dynamicArrayReverse(DynamicArray *array);

/* ===================== Array Estatico (em static_array.c) ===================== */

#define STATIC_ARRAY_CAPACITY 100

typedef struct
{
    int data[STATIC_ARRAY_CAPACITY];
    int size;
} StaticArray;

void staticArrayCreate(StaticArray *array);
void staticArrayClear(StaticArray *array);
int staticArrayIsEmpty(StaticArray *array);
int staticArrayIsFull(StaticArray *array);
int staticArraySize(StaticArray *array);
void staticArrayPush(StaticArray *array, int data);
void staticArrayInsert(StaticArray *array, int index, int data);
int staticArrayRemoveAt(StaticArray *array, int index);
int staticArrayGet(StaticArray *array, int index);
void staticArraySet(StaticArray *array, int index, int data);
int staticArrayContains(StaticArray *array, int data);
int staticArrayCountOccurrences(StaticArray *array, int data);
void staticArrayPrint(StaticArray *array);
void staticArrayCopy(StaticArray *destination, StaticArray *source);
int staticArrayCompare(StaticArray *array1, StaticArray *array2);
void staticArrayRemoveOccurrences(StaticArray *array, int data);
void staticArrayRemoveDuplicates(StaticArray *array);
void staticArrayReverse(StaticArray *array);

#endif