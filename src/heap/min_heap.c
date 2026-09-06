#include "common.h"
#include "heap.h"

Heap *minHeapCreate(int capacity)
{
    if (capacity <= 0)
    {
        return NULL;
    }

    Heap *h = malloc(sizeof(Heap));

    if (h == NULL)
    {
        return NULL;
    }

    h->array = malloc(capacity * sizeof(int));

    if (h->array == NULL)
    {
        free(h);
        return NULL;
    }

    h->size = 0;
    h->capacity = capacity;
    h->isHeap = 1;
    h->type = HEAP_MIN;

    return h;
}

void minHeapDestroy(Heap *h)
{
    if (h == NULL)
    {
        return;
    }

    free(h->array);
    free(h);
}

void minHeapPrint(Heap *h)
{
    if (h == NULL)
    {
        return;
    }

    printf("data: [");

    for (int i = 0; i < h->size; i++)
    {
        printf("%d", h->array[i]);

        if (i < h->size - 1)
        {
            printf(", ");
        }
    }

    printf("]\n");

    printf("\nCapacidade: %d\n", h->capacity);
    printf("Tamanho: %d\n", h->size);

    if (h->isHeap)
    {
        printf("Estado: Min-Heap\n");
    }
    else
    {
        printf("Estado: Vetor Ordenado / Nao eh uma Heap\n");
    }
}

int minHeapResize(Heap *h, int newCapacity)
{
    if (h == NULL)
    {
        return 0;
    }

    if (newCapacity <= 0)
    {
        return 0;
    }

    if (h->size > newCapacity)
    {
        return 0;
    }

    int *temp = realloc(h->array, newCapacity * sizeof(int));

    if (temp == NULL)
    {
        return 0;
    }

    h->array = temp;
    h->capacity = newCapacity;

    return 1;
}

static void minHeapifyUp(Heap *h, int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (h->array[index] < h->array[parent])
        {
            int temp = h->array[index];
            h->array[index] = h->array[parent];
            h->array[parent] = temp;

            index = parent;
        }
        else
        {
            break;
        }
    }
}

static void minHeapifyDown(Heap *h, int index)
{
    while (2 * index + 1 < h->size)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int minChild = leftChild;

        if (rightChild < h->size &&
            h->array[rightChild] < h->array[leftChild])
        {
            minChild = rightChild;
        }

        if (h->array[index] <= h->array[minChild])
        {
            break;
        }

        int temp = h->array[minChild];
        h->array[minChild] = h->array[index];
        h->array[index] = temp;

        index = minChild;
    }
}

void minHeapInsert(Heap *h, int data)
{
    if (h == NULL)
    {
        return;
    }

    if (!h->isHeap)
    {
        printf("Erro: o vetor nao e uma Heap\n");
        printf("Faca BuildHeap antes de inserir\n");
        return;
    }

    if (h->size == h->capacity)
    {
        if (!minHeapResize(h, h->capacity * 2))
        {
            return;
        }
    }

    int index = h->size;

    h->array[index] = data;

    h->size++;

    minHeapifyUp(h, index);
}

int minHeapExtract(Heap *h, int *min)
{
    if (h == NULL || h->size == 0 || min == NULL)
    {
        return 0;
    }

    if (!h->isHeap)
    {
        return 0;
    }

    *min = h->array[0];

    int last = h->size - 1;

    h->array[0] = h->array[last];

    h->size--;

    minHeapifyDown(h, 0);

    return 1;
}

int minHeapUpdate(Heap *h, int index, int newData)
{
    if (h == NULL ||
        h->size == 0 ||
        index < 0 ||
        index >= h->size)
    {
        return 0;
    }

    if (!h->isHeap)
    {
        return 0;
    }

    int oldData = h->array[index];

    h->array[index] = newData;

    if (newData < oldData)
    {
        minHeapifyUp(h, index);
    }
    else if (newData > oldData)
    {
        minHeapifyDown(h, index);
    }

    return 1;
}

void minHeapBuild(Heap *h)
{
    if (h == NULL || h->size == 0)
    {
        return;
    }

    int lastParent = (h->size / 2) - 1;

    for (int i = lastParent; i >= 0; i--)
    {
        minHeapifyDown(h, i);
    }

    h->isHeap = 1;
}

void minHeapSort(Heap *h)
{
    if (h == NULL || h->size <= 1)
    {
        return;
    }

    if (!h->isHeap)
    {
        return;
    }

    int originalSize = h->size;

    while (h->size > 1)
    {
        int temp = h->array[h->size - 1];

        h->array[h->size - 1] = h->array[0];

        h->array[0] = temp;

        h->size--;

        minHeapifyDown(h, 0);
    }

    h->size = originalSize;

    h->isHeap = 0;
}