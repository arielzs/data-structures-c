#include "common.h"
#include "utils.h"
#include "stack.h"

typedef struct
{
    int id;
    Stack data;
} StackInstance;

static StackInstance *stacks = NULL;
static int totalStacks = 0;
static int stacksCapacity = 0;
static int active = -1; /* indice em 'stacks', -1 = nenhuma pilha ativa */

static void createStackInstance()
{
    if (totalStacks == stacksCapacity)
    {
        if (stacksCapacity == 0)
        {
            stacksCapacity = 4;
        }
        else
        {
            stacksCapacity = stacksCapacity * 2;
        }

        stacks = realloc(stacks, stacksCapacity * sizeof(StackInstance));

        if (stacks == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    stacks[totalStacks].id = totalStacks + 1;
    stackCreate(&stacks[totalStacks].data);

    active = totalStacks;
    totalStacks++;

    printf("Stack %d created and set as active!\n", stacks[active].id);
}

static void listStackInstances()
{
    if (totalStacks == 0)
    {
        printf("No stacks created yet. Use command [1] to create one.\n");
        return;
    }

    printf("=== Created stacks ===\n");

    for (int i = 0; i < totalStacks; i++)
    {
        if (i == active)
        {
            printf("Stack %d (active)\n", stacks[i].id);
        }
        else
        {
            printf("Stack %d\n", stacks[i].id);
        }
    }
}

static void switchStackInstance(int number)
{
    if (number < 1 || number > totalStacks)
    {
        printf("Invalid stack!\n");
        return;
    }

    active = number - 1;
    printf("Active stack is now: %d\n", stacks[active].id);
}

void runStackMenu()
{
    int command, data, k;
    int running = 1;

    while (running)
    {
        printf("\n=== STACKS MENU");

        if (active != -1)
        {
            printf(" | Active stack: %d", stacks[active].id);
        }

        printf(" ===\n");

        printf("[0] Back to main menu\n");
        printf("[1] Create a new stack\n");
        printf("[2] List the created stacks\n");
        printf("[3] Switch the active stack\n");
        printf("[4] Insert elements\n");
        printf("[5] Remove elements\n");
        printf("[6] Remove occurrences\n");
        printf("[7] Search for an element\n");
        printf("[8] Count occurrences of an element\n");
        printf("[9] Reverse the active stack\n");
        printf("[10] Show the current stack\n");
        printf("[11] Copy the active stack (creates a new one)\n");
        printf("[12] Compare two stacks\n");
        printf("[13] Merge the tops of two stacks\n");
        printf("[14] Merge the bottoms of two stacks\n");
        printf("[15] Sort the stack (insertion sort)\n");
        printf("[16] Swap the top (first two elements)\n");
        printf("[17] Move the top K positions down\n");
        printf("[18] Rotate the stack\n");
        printf("[19] Rotate the stack K times\n");
        printf("[20] Sort with Merge Sort\n");
        printf("[21] Remove the middle element\n");
        printf("[22] Check if an expression is balanced\n");

        command = utilsReadInt("Choice: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("You need to create a stack first! (command 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
            createStackInstance();
            break;

        case 2:
            listStackInstances();
            break;

        case 3:
        {
            listStackInstances();

            int number = utilsReadInt("Which stack do you want to activate?\n");

            switchStackInstance(number);
            break;
        }

        case 4:
        {
            int quantity = utilsReadInt("How many elements should be added?\n");

            if (quantity <= 0)
            {
                printf("Invalid quantity!\n");
                break;
            }

            for (int i = 0; i < quantity; i++)
            {
                char prompt[32];
                snprintf(prompt, sizeof(prompt), "Element %d:\n", i + 1);

                data = utilsReadInt(prompt);

                stackPush(&stacks[active].data, data);
            }

            printf("%d element(s) added!\n", quantity);

            break;
        }

        case 5:
            if (stackIsEmpty(&stacks[active].data))
            {
                printf("Stack is empty!\n");
            }
            else
            {
                int quantity = utilsReadInt("How many elements should be removed?\n");

                stackPopMultiple(&stacks[active].data, quantity);
            }

            break;

        case 6:
            data = utilsReadInt("Which value should have its occurrences removed?\n");

            stackRemoveOccurrences(&stacks[active].data, data);

            break;

        case 7:
            data = utilsReadInt("Which element should be searched?\n");

            stackSearch(&stacks[active].data, data);

            break;

        case 8:
        {
            data = utilsReadInt("Which element should be counted?\n");

            int count = stackCountOccurrences(&stacks[active].data, data);

            printf("Element %d appears %d time(s)!\n", data, count);

            break;
        }

        case 9:
            if (stackReverse(&stacks[active].data))
            {
                printf("Stack reversed!\n");
            }
            else
            {
                printf("Stack is empty!\n");
            }

            break;

        case 10:
            stackPrint(&stacks[active].data);
            utilsPause();

            break;

        case 11:
        {
            int sourceIndex = active;
            int sourceId = stacks[sourceIndex].id;

            createStackInstance();
            stackCopy(&stacks[active].data, &stacks[sourceIndex].data);

            printf("Stack %d copied to new stack %d!\n", sourceId, stacks[active].id);

            break;
        }

        case 12:
        {
            listStackInstances();

            int firstNumber = utilsReadInt("First stack:\n");
            int secondNumber = utilsReadInt("Second stack:\n");

            if (firstNumber < 1 || firstNumber > totalStacks || secondNumber < 1 || secondNumber > totalStacks)
            {
                printf("Invalid stack!\n");
                break;
            }

            if (stackCompare(&stacks[firstNumber - 1].data, &stacks[secondNumber - 1].data))
            {
                printf("Stacks are equal!\n");
            }
            else
            {
                printf("Stacks are different!\n");
            }

            break;
        }

        case 13:
        {
            listStackInstances();

            int otherNumber = utilsReadInt("Merge with which stack (its top goes on top of the active one)?\n");

            if (otherNumber < 1 || otherNumber > totalStacks)
            {
                printf("Invalid stack!\n");
                break;
            }

            stackMergeTop(&stacks[active].data, &stacks[otherNumber - 1].data);

            printf("Stacks merged!\n");

            break;
        }

        case 14:
        {
            listStackInstances();

            int otherNumber = utilsReadInt("Merge with which stack (it goes to the bottom of the active one)?\n");

            if (otherNumber < 1 || otherNumber > totalStacks)
            {
                printf("Invalid stack!\n");
                break;
            }

            stackMergeBottom(&stacks[active].data, &stacks[otherNumber - 1].data);

            printf("Stacks merged!\n");

            break;
        }

        case 15:
            stackSort(&stacks[active].data);

            printf("Stack sorted!\n");

            break;

        case 16:
            stackSwapTop(&stacks[active].data);

            printf("Top swapped\n");

            break;

        case 17:
            k = utilsReadInt("How many positions?\n");

            stackMoveTopDown(&stacks[active].data, k);

            printf("Top moved %d positions\n", k);

            break;

        case 18:
            stackRotate(&stacks[active].data);

            printf("Stack rotated\n");

            break;

        case 19:
            k = utilsReadInt("How many times?\n");

            stackRotateK(&stacks[active].data, k);

            printf("Stack rotated %d times\n", k);

            break;

        case 20:
            stackMergeSort(&stacks[active].data);

            printf("Stack sorted!\n");

            break;

        case 21:
            stackRemoveMiddle(&stacks[active].data);

            printf("Middle element removed\n");

            break;

        case 22:
        {
            char expr[100];

            utilsReadLine("Enter the expression: ", expr, sizeof(expr));

            if (stackVerifyBalance(expr))
            {
                printf("Valid expression!\n");
            }
            else
            {
                printf("Invalid expression!\n");
            }

            break;
        }

        default:
            printf("Enter a valid command!\n");
            break;
        }
    }
}