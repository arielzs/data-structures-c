#include "common.h"
#include "sorting.h"

static void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void bubbleSort(int *array, int size)
{
    int swapped;

    for (int i = 0; i < size - 1; i++)
    {
        swapped = 0;

        for (int j = 0; j < size - 1 - i; j++)
        {
            if (array[j + 1] < array[j])
            {
                swap(&array[j + 1], &array[j]);
                swapped = 1;
            }
        }

        if (!swapped)
        {
            break;
        }
    }
}

void selectionSort(int *array, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int minPos = i;

        for (int j = i + 1; j < size; j++)
        {
            if (array[j] < array[minPos])
            {
                minPos = j;
            }
        }

        if (minPos != i)
        {
            swap(&array[i], &array[minPos]);
        }
    }
}

void insertionSort(int *array, int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }
}

/* Assume valores nao negativos */
void countingSort(int *array, int size)
{
    if (size <= 0)
    {
        return;
    }

    int max = array[0];

    for (int i = 1; i < size; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }

    int *count = calloc(max + 1, sizeof(int));
    int *output = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++)
    {
        count[array[i]]++;
    }

    for (int i = 1; i <= max; i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--)
    {
        output[count[array[i]] - 1] = array[i];
        count[array[i]]--;
    }

    for (int i = 0; i < size; i++)
    {
        array[i] = output[i];
    }

    free(count);
    free(output);
}

static int partitionLomuto(int *array, int start, int end)
{
    int pivot = array[end];

    int i = start - 1;

    for (int j = start; j < end; j++)
    {
        if (array[j] <= pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[end]);

    return i + 1;
}

void quickSortLomuto(int *array, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int pivotIndex = partitionLomuto(array, start, end);

    quickSortLomuto(array, start, pivotIndex - 1);
    quickSortLomuto(array, pivotIndex + 1, end);
}

static int partitionHoare(int *array, int start, int end)
{
    int pivot = array[start + (end - start) / 2];

    int i = start;
    int j = end;

    while (1)
    {
        while (array[i] < pivot)
        {
            i++;
        }

        while (array[j] > pivot)
        {
            j--;
        }

        if (i >= j)
        {
            return j;
        }

        swap(&array[i], &array[j]);

        i++;
        j--;
    }
}

void quickSortHoare(int *array, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int pivotIndex = partitionHoare(array, start, end);

    quickSortHoare(array, start, pivotIndex);
    quickSortHoare(array, pivotIndex + 1, end);
}

static void merge(int *array, int start, int mid, int end)
{
    int leftSize = mid - start + 1;
    int rightSize = end - mid;

    int *tempLeft = malloc(leftSize * sizeof(int));
    int *tempRight = malloc(rightSize * sizeof(int));

    for (int i = 0; i < leftSize; i++)
    {
        tempLeft[i] = array[start + i];
    }

    for (int i = 0; i < rightSize; i++)
    {
        tempRight[i] = array[mid + 1 + i];
    }

    int leftIndex = 0;
    int rightIndex = 0;
    int mergedIndex = start;

    while (leftIndex < leftSize && rightIndex < rightSize)
    {
        if (tempLeft[leftIndex] <= tempRight[rightIndex])
        {
            array[mergedIndex] = tempLeft[leftIndex];
            leftIndex++;
        }
        else
        {
            array[mergedIndex] = tempRight[rightIndex];
            rightIndex++;
        }

        mergedIndex++;
    }

    while (leftIndex < leftSize)
    {
        array[mergedIndex] = tempLeft[leftIndex];
        leftIndex++;
        mergedIndex++;
    }

    while (rightIndex < rightSize)
    {
        array[mergedIndex] = tempRight[rightIndex];
        rightIndex++;
        mergedIndex++;
    }

    free(tempLeft);
    free(tempRight);
}

void mergeSort(int *array, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int mid = start + (end - start) / 2;

    mergeSort(array, start, mid);
    mergeSort(array, mid + 1, end);

    merge(array, start, mid, end);
}

static void heapifyDown(int *array, int size, int index)
{
    while (1)
    {
        int max = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && array[left] > array[max])
        {
            max = left;
        }

        if (right < size && array[right] > array[max])
        {
            max = right;
        }

        if (array[index] >= array[max])
        {
            break;
        }

        swap(&array[index], &array[max]);

        index = max;
    }
}

static void buildMaxHeap(int *array, int size)
{
    int lastParent = (size / 2) - 1;

    for (int i = lastParent; i >= 0; i--)
    {
        heapifyDown(array, size, i);
    }
}

void heapSort(int *array, int size)
{
    buildMaxHeap(array, size);

    while (size > 1)
    {
        swap(&array[0], &array[size - 1]);

        size--;

        heapifyDown(array, size, 0);
    }
}

void sortArray(int *array, int size, SortType type)
{
    if (size <= 1)
    {
        return;
    }

    switch (type)
    {
    case SORT_BUBBLE:
        bubbleSort(array, size);
        break;

    case SORT_SELECTION:
        selectionSort(array, size);
        break;

    case SORT_INSERTION:
        insertionSort(array, size);
        break;

    case SORT_COUNTING:
        countingSort(array, size);
        break;

    case SORT_MERGE:
        mergeSort(array, 0, size - 1);
        break;

    case SORT_QUICK_LOMUTO:
        quickSortLomuto(array, 0, size - 1);
        break;

    case SORT_QUICK_HOARE:
        quickSortHoare(array, 0, size - 1);
        break;

    case SORT_HEAP:
        heapSort(array, size);
        break;
    }
}