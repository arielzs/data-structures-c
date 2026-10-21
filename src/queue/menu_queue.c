#include "common.h"
#include "utils.h"
#include "queue.h"

typedef struct
{
    int id;
    Queue data;
} QueueInstance;

static QueueInstance *queues = NULL;
static int totalQueues = 0;
static int queuesCapacity = 0;
static int active = -1; /* indice em 'queues', -1 = nenhuma fila ativa */

static void createQueueInstance()
{
    if (totalQueues == queuesCapacity)
    {
        if (queuesCapacity == 0)
        {
            queuesCapacity = 4;
        }
        else
        {
            queuesCapacity = queuesCapacity * 2;
        }

        queues = realloc(queues, queuesCapacity * sizeof(QueueInstance));

        if (queues == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    queues[totalQueues].id = totalQueues + 1;
    queueCreate(&queues[totalQueues].data);

    active = totalQueues;
    totalQueues++;

    printf("Queue %d created and set as active!\n", queues[active].id);
}

static void listQueueInstances()
{
    if (totalQueues == 0)
    {
        printf("No queues created yet. Use command [1] to create one.\n");
        return;
    }

    printf("=== Created queues ===\n");

    for (int i = 0; i < totalQueues; i++)
    {
        if (i == active)
        {
            printf("Queue %d (active)\n", queues[i].id);
        }
        else
        {
            printf("Queue %d\n", queues[i].id);
        }
    }
}

static void switchQueueInstance(int number)
{
    if (number < 1 || number > totalQueues)
    {
        printf("Invalid queue!\n");
        return;
    }

    active = number - 1;
    printf("Active queue is now: %d\n", queues[active].id);
}

/* Pede o numero de outra fila (diferente da ativa) e devolve o indice dela,
   ou -1 se invalida. Usado pelos comandos de copiar/comparar/juntar/intercalar */
static int chooseOtherQueue(const char *prompt)
{
    listQueueInstances();

    int number = utilsReadInt(prompt);

    if (number < 1 || number > totalQueues)
    {
        printf("Invalid queue!\n");
        return -1;
    }

    if (number - 1 == active)
    {
        printf("Choose a queue different from the active one!\n");
        return -1;
    }

    return number - 1;
}

void runQueueMenu()
{
    int command, data, k;
    int running = 1;

    while (running)
    {
        printf("\n=== QUEUES MENU");

        if (active != -1)
        {
            printf(" | Active queue: %d", queues[active].id);
        }

        printf(" ===\n");

        printf("[0] Back to main menu\n");
        printf("[1] Create a new queue\n");
        printf("[2] List the created queues\n");
        printf("[3] Switch the active queue\n");
        printf("[4] Show the current queue\n");
        printf("[5] Insert at the back of the queue\n");
        printf("[6] Remove from the front of the queue\n");
        printf("[7] Show the front of the queue\n");
        printf("[8] Search for a value in the queue\n");
        printf("[9] Count occurrences of a value\n");
        printf("[10] Copy the active queue (creates a new one)\n");
        printf("[11] Compare two queues\n");
        printf("[12] Show the middle element\n");
        printf("[13] Remove occurrences\n");
        printf("[14] Remove duplicates\n");
        printf("[15] Insert in order\n");
        printf("[16] Sort the queue (insertion sort)\n");
        printf("[17] Merge queues (copy; the other stays unchanged)\n");
        printf("[18] Merge queues (move; the other becomes empty)\n");
        printf("[19] Interleave queues\n");
        printf("[20] Rotate the queue\n");
        printf("[21] Rotate the queue K times\n");
        printf("[22] Sort with Merge Sort\n");
        printf("[23] Check the ordering\n");

        command = utilsReadInt("Choice: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("You need to create a queue first! (command 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
            createQueueInstance();
            break;

        case 2:
            listQueueInstances();
            break;

        case 3:
        {
            listQueueInstances();

            int number = utilsReadInt("Which queue do you want to activate?\n");

            switchQueueInstance(number);
            break;
        }

        case 4:
            queuePrint(&queues[active].data);
            utilsPause();

            break;

        case 5:
            data = utilsReadInt("Which element should be added?\n");

            queueEnqueue(&queues[active].data, data);

            printf("Element added!\n");

            break;

        case 6:
        {
            int removed = queueDequeue(&queues[active].data);

            if (removed != -1)
            {
                printf("Element %d removed!\n", removed);
            }

            break;
        }

        case 7:
            if (queueIsEmpty(&queues[active].data))
            {
                printf("Queue is empty!\n");
            }
            else
            {
                printf("The front is: %d\n", queuePeek(&queues[active].data));
            }

            break;

        case 8:
        {
            data = utilsReadInt("Which element should be searched?\n");

            int pos = queueContains(&queues[active].data, data);

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

        case 9:
            if (queueIsEmpty(&queues[active].data))
            {
                printf("Queue is empty!\n");
            }
            else
            {
                data = utilsReadInt("Which element should be counted?\n");

                int count = queueCountOccurrences(&queues[active].data, data);

                printf("Element %d appears %d time(s) in the queue\n", data, count);
            }

            break;

        case 10:
        {
            int sourceIndex = active;
            int sourceId = queues[sourceIndex].id;

            createQueueInstance();
            queueCopy(&queues[active].data, &queues[sourceIndex].data);

            printf("Queue %d copied to new queue %d!\n", sourceId, queues[active].id);

            break;
        }

        case 11:
        {
            listQueueInstances();

            int firstNumber = utilsReadInt("First queue:\n");
            int secondNumber = utilsReadInt("Second queue:\n");

            if (firstNumber < 1 || firstNumber > totalQueues || secondNumber < 1 || secondNumber > totalQueues)
            {
                printf("Invalid queue!\n");
                break;
            }

            if (queueCompare(&queues[firstNumber - 1].data, &queues[secondNumber - 1].data))
            {
                printf("Queues are equal!\n");
            }
            else
            {
                printf("Queues are different!\n");
            }

            break;
        }

        case 12:
        {
            int middle = queueFindMiddle(&queues[active].data);

            if (middle == -1)
            {
                printf("Queue is empty!\n");
            }
            else
            {
                printf("The middle element is: %d\n", middle);
            }

            break;
        }

        case 13:
        {
            data = utilsReadInt("Which value?\n");

            int removeds = queueRemoveOccurrences(&queues[active].data, data);

            if (removeds == 0)
            {
                printf("No occurrences found\n");
            }
            else
            {
                printf("%d occurrence(s) removed\n", removeds);
            }

            break;
        }

        case 14:
            queueRemoveDuplicates(&queues[active].data);

            printf("Duplicates removed!\n");

            break;

        case 15:
            data = utilsReadInt("Which element?\n");

            queueInsertSorted(&queues[active].data, data);

            printf("Element %d inserted in order!\n", data);

            break;

        case 16:
            queueSort(&queues[active].data);

            printf("Queue sorted!\n");

            break;

        case 17:
        {
            int otherIndex = chooseOtherQueue("Copy elements from which queue into the active one?\n");

            if (otherIndex == -1)
            {
                break;
            }

            queueMergeCopy(&queues[active].data, &queues[otherIndex].data);

            printf("Queues merged! (the source queue is unchanged)\n");

            break;
        }

        case 18:
        {
            int otherIndex = chooseOtherQueue("Move elements from which queue into the active one?\n");

            if (otherIndex == -1)
            {
                break;
            }

            queueMergeMove(&queues[active].data, &queues[otherIndex].data);

            printf("Queues merged! (the source queue is now empty)\n");

            break;
        }

        case 19:
        {
            int otherIndex = chooseOtherQueue("Interleave the active queue with which queue?\n");

            if (otherIndex == -1)
            {
                break;
            }

            queueInterleave(&queues[active].data, &queues[otherIndex].data);

            printf("Queues interleaved!\n");

            break;
        }

        case 20:
            queueRotate(&queues[active].data);

            printf("Queue rotated!\n");

            break;

        case 21:
            k = utilsReadInt("How many times?\n");

            queueRotateK(&queues[active].data, k);

            printf("Queue rotated %d times!\n", k);

            break;

        case 22:
            queueMergeSort(&queues[active].data);

            printf("Queue sorted!\n");

            break;

        case 23:
        {
            int sorted = queueIsSorted(&queues[active].data);

            if (sorted == 1)
            {
                printf("Queue is in ascending order\n");
            }
            else if (sorted == 2)
            {
                printf("Queue is in descending order\n");
            }
            else if (sorted == -1)
            {
                printf("Queue is empty\n");
            }
            else
            {
                printf("Queue is not sorted\n");
            }

            break;
        }

        default:
            printf("Enter a valid command!\n");
            break;
        }
    }
}