#include <assert.h>
#include "common.h"
#include "sorting.h"

static int isSortedAscending(int *array, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (array[i] > array[i + 1])
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    int original[] = {9, 3, 7, 1, 8, 2, 5, 4, 6, 0};
    int size = 10;
    int buffer[10];

    /* Bubble */
    for (int i = 0; i < size; i++)
        buffer[i] = original[i];

    sortArray(buffer, size, SORT_BUBBLE);
    assert(isSortedAscending(buffer, size) == 1);

    /* Selection */
    for (int i = 0; i < size; i++)
        buffer[i] = original[i];

    sortArray(buffer, size, SORT_SELECTION);
    assert(isSortedAscending(buffer, size) == 1);

    /* Insertion */
    for (int i = 0; i < size; i++)
        buffer[i] = original[i];

    sortArray(buffer, size, SORT_INSERTION);
    assert(isSortedAscending(buffer, size) == 1);

    /* Counting */
    for (int i = 0; i < size; i++)
        buffer[i] = original[i];

    sortArray(buffer, size, SORT_COUNTING);
    assert(isSortedAscending(buffer, size) == 1);

    /* Merge */
    for (int i = 0; i < size; i++)
        buffer[i] = original[i];

    sortArray(buffer, size, SORT_MERGE);
    assert(isSortedAscending(buffer, size) == 1);

    /* Quick Sort - particao de Lomuto */
    for (int i = 0; i < size; i++)
        buffer[i] = original[i];

    sortArray(buffer, size, SORT_QUICK_LOMUTO);
    assert(isSortedAscending(buffer, size) == 1);

    /* Quick Sort - particao de Hoare */
    for (int i = 0; i < size; i++)
        buffer[i] = original[i];

    sortArray(buffer, size, SORT_QUICK_HOARE);
    assert(isSortedAscending(buffer, size) == 1);

    /* Heap Sort */
    for (int i = 0; i < size; i++)
        buffer[i] = original[i];

    sortArray(buffer, size, SORT_HEAP);
    assert(isSortedAscending(buffer, size) == 1);

    /* ===== Casos-limite: array vazio e com 1 elemento nao devem quebrar ===== */
    int emptyArray[1];
    sortArray(emptyArray, 0, SORT_BUBBLE);

    int singleElement[1] = {42};
    sortArray(singleElement, 1, SORT_MERGE);
    assert(singleElement[0] == 42);

    /* ===== Ja ordenado continua ordenado (bubble sort tem short-circuit) ===== */
    int alreadySorted[] = {1, 2, 3, 4, 5};
    sortArray(alreadySorted, 5, SORT_BUBBLE);
    assert(isSortedAscending(alreadySorted, 5) == 1);

    return 0;
}