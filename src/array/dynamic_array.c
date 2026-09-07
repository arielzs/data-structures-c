#include "common.h"
#include "array.h"

#define INITIAL_CAPACITY 10

void dynamicArrayCreate(DynamicArray *array)
{
    array->data = malloc(INITIAL_CAPACITY * sizeof(int));
    array->size = 0;
    array->capacity = INITIAL_CAPACITY;
}

void dynamicArrayClear(DynamicArray *array)
{
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}

int dynamicArrayIsEmpty(DynamicArray *array)
{
    return array->size == 0;
}

int dynamicArraySize(DynamicArray *array)
{
    return array->size;
}

static void dynamicArrayResize(DynamicArray *array)
{
    int newCapacity = array->capacity * 2;

    int *temp = realloc(array->data, newCapacity * sizeof(int));

    if (temp == NULL)
    {
        printf("Falha ao alocar memoria!\n");
        return;
    }

    array->data = temp;
    array->capacity = newCapacity;
}

void dynamicArrayPush(DynamicArray *array, int data)
{
    if (array->size == array->capacity)
    {
        dynamicArrayResize(array);
    }

    array->data[array->size] = data;
    array->size++;
}

void dynamicArrayInsert(DynamicArray *array, int index, int data)
{
    if (index < 0 || index > array->size)
    {
        return;
    }

    if (array->size == array->capacity)
    {
        dynamicArrayResize(array);
    }

    for (int i = array->size; i > index; i--)
    {
        array->data[i] = array->data[i - 1];
    }

    array->data[index] = data;
    array->size++;
}

int dynamicArrayRemoveAt(DynamicArray *array, int index)
{
    if (index < 0 || index >= array->size)
    {
        return -1;
    }

    int removed = array->data[index];

    for (int i = index; i < array->size - 1; i++)
    {
        array->data[i] = array->data[i + 1];
    }

    array->size--;

    return removed;
}

int dynamicArrayGet(DynamicArray *array, int index)
{
    if (index < 0 || index >= array->size)
    {
        return -1;
    }

    return array->data[index];
}

void dynamicArraySet(DynamicArray *array, int index, int data)
{
    if (index < 0 || index >= array->size)
    {
        return;
    }

    array->data[index] = data;
}

int dynamicArrayContains(DynamicArray *array, int data)
{
    for (int i = 0; i < array->size; i++)
    {
        if (array->data[i] == data)
        {
            return i;
        }
    }

    return -1;
}

int dynamicArrayCountOccurrences(DynamicArray *array, int data)
{
    int count = 0;

    for (int i = 0; i < array->size; i++)
    {
        if (array->data[i] == data)
        {
            count++;
        }
    }

    return count;
}

void dynamicArrayPrint(DynamicArray *array)
{
    printf("[");

    for (int i = 0; i < array->size; i++)
    {
        printf("%d", array->data[i]);

        if (i < array->size - 1)
        {
            printf(", ");
        }
    }

    printf("]\n");
}

void dynamicArrayCopy(DynamicArray *destination, DynamicArray *source)
{
    dynamicArrayClear(destination);
    dynamicArrayCreate(destination);

    while (destination->capacity < source->size)
    {
        dynamicArrayResize(destination);
    }

    for (int i = 0; i < source->size; i++)
    {
        destination->data[i] = source->data[i];
    }

    destination->size = source->size;
}

int dynamicArrayCompare(DynamicArray *array1, DynamicArray *array2)
{
    if (array1->size != array2->size)
    {
        return 0;
    }

    for (int i = 0; i < array1->size; i++)
    {
        if (array1->data[i] != array2->data[i])
        {
            return 0;
        }
    }

    return 1;
}

void dynamicArrayRemoveOccurrences(DynamicArray *array, int data)
{
    int i = 0;

    while (i < array->size)
    {
        if (array->data[i] == data)
        {
            dynamicArrayRemoveAt(array, i);
        }
        else
        {
            i++;
        }
    }
}

void dynamicArrayRemoveDuplicates(DynamicArray *array)
{
    for (int i = 0; i < array->size; i++)
    {
        int j = i + 1;

        while (j < array->size)
        {
            if (array->data[i] == array->data[j])
            {
                dynamicArrayRemoveAt(array, j);
            }
            else
            {
                j++;
            }
        }
    }
}

void dynamicArrayReverse(DynamicArray *array)
{
    int left = 0;
    int right = array->size - 1;

    while (left < right)
    {
        int temp = array->data[left];
        array->data[left] = array->data[right];
        array->data[right] = temp;

        left++;
        right--;
    }
}