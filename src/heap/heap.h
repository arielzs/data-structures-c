#ifndef HEAP_H
#define HEAP_H

/* Ponto de entrada do submenu de heaps, chamado pelo menu.c */
void runHeapMenu(void);

typedef enum
{
    HEAP_MAX,
    HEAP_MIN
} HeapType;

typedef struct
{
    int *array;
    int size;
    int capacity;
    int isHeap;
    HeapType type;
} Heap;

/* ===================== Max-Heap ===================== */

Heap *maxHeapCreate(int capacity);
void maxHeapDestroy(Heap *h);
void maxHeapPrint(Heap *h);
int maxHeapResize(Heap *h, int newCapacity);
void maxHeapInsert(Heap *h, int data);
int maxHeapExtract(Heap *h, int *max);
int maxHeapUpdate(Heap *h, int index, int newData);
void maxHeapBuild(Heap *h);
void maxHeapSort(Heap *h);

/* ===================== Min-Heap ===================== */

Heap *minHeapCreate(int capacity);
void minHeapDestroy(Heap *h);
void minHeapPrint(Heap *h);
int minHeapResize(Heap *h, int newCapacity);
void minHeapInsert(Heap *h, int data);
int minHeapExtract(Heap *h, int *min);
int minHeapUpdate(Heap *h, int index, int newData);
void minHeapBuild(Heap *h);
void minHeapSort(Heap *h);

#endif