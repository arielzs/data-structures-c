#include "common.h"
#include "array.h"

#define INITIAL_CAPACITY 10

void arrayCreate(Array *array)
{
    array->data = malloc(INITIAL_CAPACITY * sizeof(int));
    array->size = 0;
    array->capacity = INITIAL_CAPACITY;
}

void arrayClear(Array *array)
{
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}

int arrayIsEmpty(Array *array)
{
    return array->size == 0;
}

int arraySize(Array *array)
{
    return array->size;
}

static void arrayResize(Array *array)
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

void arrayPush(Array *array, int data)
{
    if (array->size == array->capacity)
    {
        arrayResize(array);
    }

    array->data[array->size] = data;
    array->size++;
}

void arrayInsert(Array *array, int index, int data)
{
    if (index < 0 || index > array->size)
    {
        return;
    }

    if (array->size == array->capacity)
    {
        arrayResize(array);
    }

    for (int i = array->size; i > index; i--)
    {
        array->data[i] = array->data[i - 1];
    }

    array->data[index] = data;
    array->size++;
}

int arrayRemoveAt(Array *array, int index)
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

int arrayGet(Array *array, int index)
{
    if (index < 0 || index >= array->size)
    {
        return -1;
    }

    return array->data[index];
}

void arraySet(Array *array, int index, int data)
{
    if (index < 0 || index >= array->size)
    {
        return;
    }

    array->data[index] = data;
}

int arrayContains(Array *array, int data)
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

int arrayCountOccurrences(Array *array, int data)
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

void arrayPrint(Array *array)
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

void arrayCopy(Array *destination, Array *source)
{
    arrayClear(destination);
    arrayCreate(destination);

    while (destination->capacity < source->size)
    {
        arrayResize(destination);
    }

    for (int i = 0; i < source->size; i++)
    {
        destination->data[i] = source->data[i];
    }

    destination->size = source->size;
}

int arrayCompare(Array *array1, Array *array2)
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

void arrayRemoveOccurrences(Array *array, int data)
{
    int i = 0;

    while (i < array->size)
    {
        if (array->data[i] == data)
        {
            arrayRemoveAt(array, i);
        }
        else
        {
            i++;
        }
    }
}

void arrayRemoveDuplicates(Array *array)
{
    for (int i = 0; i < array->size; i++)
    {
        int j = i + 1;

        while (j < array->size)
        {
            if (array->data[i] == array->data[j])
            {
                arrayRemoveAt(array, j);
            }
            else
            {
                j++;
            }
        }
    }
}

void arrayReverse(Array *array)
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