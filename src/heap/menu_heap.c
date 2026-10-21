#include "common.h"
#include "utils.h"
#include "heap.h"

typedef struct
{
    int id;
    Heap *heap;
} HeapInstance;

static HeapInstance *heaps = NULL;
static int totalHeaps = 0;
static int heapsCapacity = 0;
static int active = -1; /* indice em 'heaps', -1 = nenhuma heap ativa */

static const char *heapTypeName(HeapType type)
{
    if (type == HEAP_MAX)
    {
        return "Max-Heap";
    }

    return "Min-Heap";
}

static void createHeapInstance(HeapType type)
{
    int capacity = utilsReadInt("Initial capacity?\n");

    if (capacity <= 0)
    {
        printf("Invalid capacity!\n");
        return;
    }

    Heap *heap;

    if (type == HEAP_MAX)
    {
        heap = maxHeapCreate(capacity);
    }
    else
    {
        heap = minHeapCreate(capacity);
    }

    if (heap == NULL)
    {
        printf("Error creating the heap\n");
        return;
    }

    if (totalHeaps == heapsCapacity)
    {
        if (heapsCapacity == 0)
        {
            heapsCapacity = 4;
        }
        else
        {
            heapsCapacity = heapsCapacity * 2;
        }

        heaps = realloc(heaps, heapsCapacity * sizeof(HeapInstance));

        if (heaps == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    heaps[totalHeaps].id = totalHeaps + 1;
    heaps[totalHeaps].heap = heap;

    active = totalHeaps;
    totalHeaps++;

    printf("Heap %d (%s) created and set as active!\n", heaps[active].id, heapTypeName(type));
}

static void listHeapInstances()
{
    if (totalHeaps == 0)
    {
        printf("No heaps created yet. Use command [1] to create one.\n");
        return;
    }

    printf("=== Created heaps ===\n");

    for (int i = 0; i < totalHeaps; i++)
    {
        if (i == active)
        {
            printf("Heap %d - %s (active)\n", heaps[i].id, heapTypeName(heaps[i].heap->type));
        }
        else
        {
            printf("Heap %d - %s\n", heaps[i].id, heapTypeName(heaps[i].heap->type));
        }
    }
}

static void switchHeapInstance(int number)
{
    if (number < 1 || number > totalHeaps)
    {
        printf("Invalid heap!\n");
        return;
    }

    active = number - 1;
    printf("Active heap is now: %d (%s)\n", heaps[active].id, heapTypeName(heaps[active].heap->type));
}

void runHeapMenu()
{
    int command, data;
    int running = 1;

    while (running)
    {
        printf("\n=== HEAPS MENU");

        if (active != -1)
        {
            printf(" | Active heap: %d (%s)", heaps[active].id, heapTypeName(heaps[active].heap->type));
        }

        printf(" ===\n");

        printf("[0] Back to main menu\n");
        printf("[1] Create a new heap\n");
        printf("[2] List the created heaps\n");
        printf("[3] Switch the active heap\n");
        printf("[4] Show the current heap\n");
        printf("[5] Insert element\n");
        printf("[6] Extract the top (largest in Max-Heap, smallest in Min-Heap)\n");
        printf("[7] Update an element\n");
        printf("[8] Increase the capacity\n");
        printf("[9] Sort with HeapSort\n");
        printf("[10] Turn the array into a heap (BuildHeap)\n");

        command = utilsReadInt("Choice: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("You need to create a heap first! (command 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
        {
            int typeChoice = utilsReadInt("Which type of heap?\n[1] Max-Heap\n[2] Min-Heap\n");

            if (typeChoice == 1)
            {
                createHeapInstance(HEAP_MAX);
            }
            else if (typeChoice == 2)
            {
                createHeapInstance(HEAP_MIN);
            }
            else
            {
                printf("Invalid type!\n");
            }

            break;
        }

        case 2:
            listHeapInstances();
            break;

        case 3:
        {
            listHeapInstances();

            int number = utilsReadInt("Which heap do you want to activate?\n");

            switchHeapInstance(number);
            break;
        }

        case 4:
            if (heaps[active].heap->type == HEAP_MAX)
            {
                maxHeapPrint(heaps[active].heap);
            }
            else
            {
                minHeapPrint(heaps[active].heap);
            }

            utilsPause();

            break;

        case 5:
            data = utilsReadInt("Which element?\n");

            if (heaps[active].heap->type == HEAP_MAX)
            {
                maxHeapInsert(heaps[active].heap, data);
            }
            else
            {
                minHeapInsert(heaps[active].heap, data);
            }

            break;

        case 6:
        {
            int top;
            int ok;

            if (heaps[active].heap->type == HEAP_MAX)
            {
                ok = maxHeapExtract(heaps[active].heap, &top);
            }
            else
            {
                ok = minHeapExtract(heaps[active].heap, &top);
            }

            if (!ok)
            {
                printf("Error: could not extract the top\n");
                break;
            }

            printf("Extracted value: %d\n", top);

            break;
        }

        case 7:
        {
            int index = utilsReadInt("Which index?\n");
            int newData = utilsReadInt("To which value?\n");

            int ok;

            if (heaps[active].heap->type == HEAP_MAX)
            {
                ok = maxHeapUpdate(heaps[active].heap, index, newData);
            }
            else
            {
                ok = minHeapUpdate(heaps[active].heap, index, newData);
            }

            if (!ok)
            {
                printf("Error: could not update\n");
                break;
            }

            printf("Update done!\n");

            break;
        }

        case 8:
        {
            int newCapacity = utilsReadInt("New capacity?\n");
            int ok;

            if (heaps[active].heap->type == HEAP_MAX)
            {
                ok = maxHeapResize(heaps[active].heap, newCapacity);
            }
            else
            {
                ok = minHeapResize(heaps[active].heap, newCapacity);
            }

            if (!ok)
            {
                printf("Resize error\n");
                break;
            }

            printf("Capacity changed!\n");

            break;
        }

        case 9:
            if (!heaps[active].heap->isHeap)
            {
                printf("Error: the current array is not a heap\n");
                printf("Run BuildHeap first\n");
                break;
            }

            if (heaps[active].heap->type == HEAP_MAX)
            {
                maxHeapSort(heaps[active].heap);
                printf("Array sorted (ascending)!\n");
            }
            else
            {
                minHeapSort(heaps[active].heap);
                printf("Array sorted (descending)!\n");
            }

            break;

        case 10:
            if (heaps[active].heap->type == HEAP_MAX)
            {
                maxHeapBuild(heaps[active].heap);
            }
            else
            {
                minHeapBuild(heaps[active].heap);
            }

            printf("Heap built!\n");

            break;

        default:
            printf("Enter a valid command!\n");
            break;
        }
    }
}