#include "common.h"
#include "array.h"

void staticArrayCreate(StaticArray *array)
{
    array->size = 0;
}

void staticArrayClear(StaticArray *array)
{
    array->size = 0;
}

int staticArrayIsEmpty(StaticArray *array)
{
    return array->size == 0;
}

int staticArrayIsFull(StaticArray *array)
{
    return array->size == STATIC_ARRAY_CAPACITY;
}

int staticArraySize(StaticArray *array)
{
    return array->size;
}

int staticArrayPush(StaticArray *array, int data)
{
    if (staticArrayIsFull(array))
    {
        printf("Array cheio!\n");
        return 0;
    }

    array->data[array->size] = data;
    array->size++;

    return 1;
}

int staticArrayInsert(StaticArray *array, int index, int data)
{
    if (index < 0 || index > array->size)
    {
        printf("Posicao invalida!\n");
        return 0;
    }

    if (staticArrayIsFull(array))
    {
        printf("Array cheio!\n");
        return 0;
    }

    for (int i = array->size; i > index; i--)
    {
        array->data[i] = array->data[i - 1];
    }

    array->data[index] = data;
    array->size++;

    return 1;
}

int staticArrayRemoveAt(StaticArray *array, int index)
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

int staticArrayGet(StaticArray *array, int index)
{
    if (index < 0 || index >= array->size)
    {
        return -1;
    }

    return array->data[index];
}

void staticArraySet(StaticArray *array, int index, int data)
{
    if (index < 0 || index >= array->size)
    {
        return;
    }

    array->data[index] = data;
}

int staticArrayContains(StaticArray *array, int data)
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

int staticArrayCountOccurrences(StaticArray *array, int data)
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

void staticArrayPrint(StaticArray *array)
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

void staticArrayCopy(StaticArray *destination, StaticArray *source)
{
    for (int i = 0; i < source->size; i++)
    {
        destination->data[i] = source->data[i];
    }

    destination->size = source->size;
}

int staticArrayCompare(StaticArray *array1, StaticArray *array2)
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

void staticArrayRemoveOccurrences(StaticArray *array, int data)
{
    int i = 0;

    while (i < array->size)
    {
        if (array->data[i] == data)
        {
            staticArrayRemoveAt(array, i);
        }
        else
        {
            i++;
        }
    }
}

void staticArrayRemoveDuplicates(StaticArray *array)
{
    for (int i = 0; i < array->size; i++)
    {
        int j = i + 1;

        while (j < array->size)
        {
            if (array->data[i] == array->data[j])
            {
                staticArrayRemoveAt(array, j);
            }
            else
            {
                j++;
            }
        }
    }
}

void staticArrayReverse(StaticArray *array)
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