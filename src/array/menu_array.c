#include "common.h"
#include "utils.h"
#include "array.h"
#include "sorting.h"

typedef enum
{
    ARRAY_DYNAMIC,
    ARRAY_STATIC
} ArrayType;

typedef struct
{
    int id;
    ArrayType type;
    union
    {
        DynamicArray dynamic;
        StaticArray staticArr;
    } data;
} ArrayInstance;

static ArrayInstance *arrays = NULL;
static int totalArrays = 0;
static int arraysCapacity = 0;
static int active = -1; /* indice em 'arrays', -1 = nenhum array ativo */

static const char *arrayTypeName(ArrayType type)
{
    if (type == ARRAY_DYNAMIC)
    {
        return "Dynamic";
    }

    return "Static";
}

/* Devolve o ponteiro pro vetor de inteiros e o tamanho atual do array
   ativo, independente do tipo. Usado pelos comandos que nao precisam
   saber qual e o tipo (ex: ordenar). */
static void getActiveRaw(int **dataOut, int *sizeOut)
{
    if (arrays[active].type == ARRAY_DYNAMIC)
    {
        *dataOut = arrays[active].data.dynamic.data;
        *sizeOut = arrays[active].data.dynamic.size;
    }
    else
    {
        *dataOut = arrays[active].data.staticArr.data;
        *sizeOut = arrays[active].data.staticArr.size;
    }
}

static void createArrayInstance(ArrayType type)
{
    if (totalArrays == arraysCapacity)
    {
        if (arraysCapacity == 0)
        {
            arraysCapacity = 4;
        }
        else
        {
            arraysCapacity = arraysCapacity * 2;
        }

        arrays = realloc(arrays, arraysCapacity * sizeof(ArrayInstance));

        if (arrays == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    arrays[totalArrays].id = totalArrays + 1;
    arrays[totalArrays].type = type;

    if (type == ARRAY_DYNAMIC)
    {
        dynamicArrayCreate(&arrays[totalArrays].data.dynamic);
    }
    else
    {
        staticArrayCreate(&arrays[totalArrays].data.staticArr);
    }

    active = totalArrays;
    totalArrays++;

    printf("Array %d (%s) created and set as active!\n", arrays[active].id, arrayTypeName(type));
}

static void listArrayInstances()
{
    if (totalArrays == 0)
    {
        printf("No arrays created yet. Use command [1] to create one.\n");
        return;
    }

    printf("=== Created arrays ===\n");

    for (int i = 0; i < totalArrays; i++)
    {
        if (i == active)
        {
            printf("Array %d - %s (active)\n", arrays[i].id, arrayTypeName(arrays[i].type));
        }
        else
        {
            printf("Array %d - %s\n", arrays[i].id, arrayTypeName(arrays[i].type));
        }
    }
}

static void switchArrayInstance(int number)
{
    if (number < 1 || number > totalArrays)
    {
        printf("Invalid array!\n");
        return;
    }

    active = number - 1;
    printf("Active array is now: %d (%s)\n", arrays[active].id, arrayTypeName(arrays[active].type));
}

static void printSortMenu()
{
    printf("\nWhich sorting algorithm?\n");
    printf("[1] Bubble Sort\n");
    printf("[2] Selection Sort\n");
    printf("[3] Insertion Sort\n");
    printf("[4] Counting Sort\n");
    printf("[5] Merge Sort\n");
    printf("[6] Quick Sort (Lomuto partition)\n");
    printf("[7] Quick Sort (Hoare partition)\n");
    printf("[8] Heap Sort\n");
}

static void sortActiveArray()
{
    printSortMenu();

    int choice = utilsReadInt("Choice: ");
    SortType type;

    switch (choice)
    {
    case 1:
        type = SORT_BUBBLE;
        break;

    case 2:
        type = SORT_SELECTION;
        break;

    case 3:
        type = SORT_INSERTION;
        break;

    case 4:
        type = SORT_COUNTING;
        break;

    case 5:
        type = SORT_MERGE;
        break;

    case 6:
        type = SORT_QUICK_LOMUTO;
        break;

    case 7:
        type = SORT_QUICK_HOARE;
        break;

    case 8:
        type = SORT_HEAP;
        break;

    default:
        printf("Invalid algorithm!\n");
        return;
    }

    int *dataPtr;
    int sizeVal;

    getActiveRaw(&dataPtr, &sizeVal);

    sortArray(dataPtr, sizeVal, type);

    printf("Array sorted!\n");
}

void runArrayMenu()
{
    int command, data, index;
    int running = 1;

    while (running)
    {
        printf("\n=== ARRAYS MENU");

        if (active != -1)
        {
            printf(" | Active array: %d (%s)", arrays[active].id, arrayTypeName(arrays[active].type));
        }

        printf(" ===\n");

        printf("[0] Back to main menu\n");
        printf("[1] Create a new array\n");
        printf("[2] List the created arrays\n");
        printf("[3] Switch the active array\n");
        printf("[4] Show the current array\n");
        printf("[5] Insert at the end of the array\n");
        printf("[6] Insert at a position\n");
        printf("[7] Remove from a position\n");
        printf("[8] Show the element at a position\n");
        printf("[9] Change the element at a position\n");
        printf("[10] Search for a value in the array\n");
        printf("[11] Count occurrences of a value\n");
        printf("[12] Copy the active array (creates a new one)\n");
        printf("[13] Compare two arrays (same type)\n");
        printf("[14] Remove occurrences of a value\n");
        printf("[15] Remove duplicates\n");
        printf("[16] Reverse the array\n");
        printf("[17] Show the array size\n");
        printf("[18] Check if the array is empty\n");
        printf("[19] Sort the array\n");
        printf("[20] Check if the array is full (static only)\n");

        command = utilsReadInt("Choice: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("You need to create an array first! (command 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
        {
            int typeChoice = utilsReadInt("Which type of array?\n[1] Dynamic (grows by itself)\n[2] Static (fixed capacity)\n");

            if (typeChoice == 1)
            {
                createArrayInstance(ARRAY_DYNAMIC);
            }
            else if (typeChoice == 2)
            {
                createArrayInstance(ARRAY_STATIC);
            }
            else
            {
                printf("Invalid type!\n");
            }

            break;
        }

        case 2:
            listArrayInstances();
            break;

        case 3:
        {
            listArrayInstances();

            int number = utilsReadInt("Which array do you want to activate?\n");

            switchArrayInstance(number);
            break;
        }

        case 4:
            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                dynamicArrayPrint(&arrays[active].data.dynamic);
            }
            else
            {
                staticArrayPrint(&arrays[active].data.staticArr);
            }

            utilsPause();

            break;

        case 5:
        {
            data = utilsReadInt("Which element should be added?\n");

            int pushed;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                pushed = dynamicArrayPush(&arrays[active].data.dynamic, data);
            }
            else
            {
                pushed = staticArrayPush(&arrays[active].data.staticArr, data);
            }

            if (pushed)
            {
                printf("Element added!\n");
            }

            break;
        }

        case 6:
        {
            index = utilsReadInt("Which position?\n");
            data = utilsReadInt("Which element?\n");

            int inserted;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                inserted = dynamicArrayInsert(&arrays[active].data.dynamic, index, data);
            }
            else
            {
                inserted = staticArrayInsert(&arrays[active].data.staticArr, index, data);
            }

            if (inserted)
            {
                printf("Element inserted!\n");
            }

            break;
        }

        case 7:
        {
            index = utilsReadInt("Which position should be removed?\n");

            int removed;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                removed = dynamicArrayRemoveAt(&arrays[active].data.dynamic, index);
            }
            else
            {
                removed = staticArrayRemoveAt(&arrays[active].data.staticArr, index);
            }

            if (removed == -1)
            {
                printf("Invalid position!\n");
            }
            else
            {
                printf("Element %d removed!\n", removed);
            }

            break;
        }

        case 8:
        {
            index = utilsReadInt("Which position?\n");

            int value;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                value = dynamicArrayGet(&arrays[active].data.dynamic, index);
            }
            else
            {
                value = staticArrayGet(&arrays[active].data.staticArr, index);
            }

            if (value == -1)
            {
                printf("Invalid position!\n");
            }
            else
            {
                printf("The element at position %d is: %d\n", index, value);
            }

            break;
        }

        case 9:
            index = utilsReadInt("Which position?\n");
            data = utilsReadInt("New value?\n");

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                dynamicArraySet(&arrays[active].data.dynamic, index, data);
            }
            else
            {
                staticArraySet(&arrays[active].data.staticArr, index, data);
            }

            printf("Element updated!\n");

            break;

        case 10:
        {
            data = utilsReadInt("Which element should be searched?\n");

            int pos;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                pos = dynamicArrayContains(&arrays[active].data.dynamic, data);
            }
            else
            {
                pos = staticArrayContains(&arrays[active].data.staticArr, data);
            }

            if (pos == -1)
            {
                printf("Element not found!\n");
            }
            else
            {
                printf("Element %d found at position %d\n", data, pos);
            }

            break;
        }

        case 11:
        {
            data = utilsReadInt("Which element should be counted?\n");

            int count;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                count = dynamicArrayCountOccurrences(&arrays[active].data.dynamic, data);
            }
            else
            {
                count = staticArrayCountOccurrences(&arrays[active].data.staticArr, data);
            }

            printf("Element %d appears %d time(s) in the array\n", data, count);

            break;
        }

        case 12:
        {
            int sourceIndex = active;
            int sourceId = arrays[sourceIndex].id;
            ArrayType type = arrays[sourceIndex].type;

            createArrayInstance(type);

            if (type == ARRAY_DYNAMIC)
            {
                dynamicArrayCopy(&arrays[active].data.dynamic, &arrays[sourceIndex].data.dynamic);
            }
            else
            {
                staticArrayCopy(&arrays[active].data.staticArr, &arrays[sourceIndex].data.staticArr);
            }

            printf("Array %d copied to new array %d!\n", sourceId, arrays[active].id);

            break;
        }

        case 13:
        {
            listArrayInstances();

            int firstNumber = utilsReadInt("First array:\n");
            int secondNumber = utilsReadInt("Second array:\n");

            if (firstNumber < 1 || firstNumber > totalArrays || secondNumber < 1 || secondNumber > totalArrays)
            {
                printf("Invalid array!\n");
                break;
            }

            ArrayInstance *first = &arrays[firstNumber - 1];
            ArrayInstance *second = &arrays[secondNumber - 1];

            if (first->type != second->type)
            {
                printf("Only arrays of the same type can be compared!\n");
                break;
            }

            int equal;

            if (first->type == ARRAY_DYNAMIC)
            {
                equal = dynamicArrayCompare(&first->data.dynamic, &second->data.dynamic);
            }
            else
            {
                equal = staticArrayCompare(&first->data.staticArr, &second->data.staticArr);
            }

            if (equal)
            {
                printf("Arrays are equal!\n");
            }
            else
            {
                printf("Arrays are different!\n");
            }

            break;
        }

        case 14:
            data = utilsReadInt("Which element should be removed?\n");

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                dynamicArrayRemoveOccurrences(&arrays[active].data.dynamic, data);
            }
            else
            {
                staticArrayRemoveOccurrences(&arrays[active].data.staticArr, data);
            }

            printf("Occurrences removed!\n");

            break;

        case 15:
            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                dynamicArrayRemoveDuplicates(&arrays[active].data.dynamic);
            }
            else
            {
                staticArrayRemoveDuplicates(&arrays[active].data.staticArr);
            }

            printf("Duplicates removed!\n");

            break;

        case 16:
            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                dynamicArrayReverse(&arrays[active].data.dynamic);
            }
            else
            {
                staticArrayReverse(&arrays[active].data.staticArr);
            }

            printf("Array reversed!\n");

            break;

        case 17:
        {
            int total;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                total = dynamicArraySize(&arrays[active].data.dynamic);
            }
            else
            {
                total = staticArraySize(&arrays[active].data.staticArr);
            }

            printf("Array size: %d\n", total);

            break;
        }

        case 18:
        {
            int empty;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                empty = dynamicArrayIsEmpty(&arrays[active].data.dynamic);
            }
            else
            {
                empty = staticArrayIsEmpty(&arrays[active].data.staticArr);
            }

            if (empty)
            {
                printf("Array is empty!\n");
            }
            else
            {
                printf("Array is not empty!\n");
            }

            break;
        }

        case 19:
            sortActiveArray();
            break;

        case 20:
            if (arrays[active].type != ARRAY_STATIC)
            {
                printf("This command only makes sense for static arrays!\n");
            }
            else if (staticArrayIsFull(&arrays[active].data.staticArr))
            {
                printf("Array is full!\n");
            }
            else
            {
                printf("Array is not full!\n");
            }

            break;

        default:
            printf("Enter a valid command!\n");
            break;
        }
    }
}