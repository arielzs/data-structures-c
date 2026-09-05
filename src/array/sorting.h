#ifndef SORTING_H
#define SORTING_H

typedef enum
{
    SORT_BUBBLE,
    SORT_SELECTION,
    SORT_INSERTION,
    SORT_COUNTING,
    SORT_MERGE,
    SORT_QUICK_LOMUTO,
    SORT_QUICK_HOARE,
    SORT_HEAP
} SortType;

void bubbleSort(int *array, int size);
void selectionSort(int *array, int size);
void insertionSort(int *array, int size);
void countingSort(int *array, int size);
void mergeSort(int *array, int start, int end);
void quickSortLomuto(int *array, int start, int end);
void quickSortHoare(int *array, int start, int end);
void heapSort(int *array, int size);

/* Escolhe o algoritmo pelo tipo e ordena o array inteiro (usado pelo menu do array) */
void sortArray(int *array, int size, SortType type);

#endif