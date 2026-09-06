#include "common.h"
#include "menu.h"
#include "utils.h"
#include "array.h"      /* declara runArrayMenu() */
#include "hash_table.h" /* declara runHashTableMenu() */
#include "heap.h"       /* declara runHeapMenu() */
#include "queue.h"      /* declara runQueueMenu() */
#include "stack.h"      /* declara runStackMenu() */
#include "tree.h"       /* declara runTreeMenu() */

/* Descomente conforme cada estrutura for sendo implementada.
   Cada modulo deve expor uma funcao runXxxMenu(void), igual os outros headers fazem. */
/*
#include "deque.h"
#include "graph.h"
*/

static void printMainMenu(void)
{
    printf("\n=== DATA STRUCTURES C ===\n");
    printf("[1] Array\n");
    printf("[2] Deque\n");
    printf("[3] Graph\n");
    printf("[4] Hash Table\n");
    printf("[5] Heap\n");
    printf("[6] Queue\n");
    printf("[7] Stack\n");
    printf("[8] Tree (BST / AVL)\n");
    printf("[0] Sair\n");
}

void runMainMenu(void)
{
    int choice;
    int running = 1;

    while (running)
    {
        printMainMenu();
        choice = utilsReadInt("Escolha: ");

        switch (choice)
        {
        case 1:
            runArrayMenu();
            break;

        case 2:
            printf("Deque ainda esta em desenvolvimento!\n");
            /* runDequeMenu(); */
            break;

        case 3:
            printf("Graph ainda esta em desenvolvimento!\n");
            /* runGraphMenu(); */
            break;

        case 4:
            runHashTableMenu();
            break;

        case 5:
            runHeapMenu();
            break;

        case 6:
            runQueueMenu();
            break;

        case 7:
            runStackMenu();
            break;

        case 8:
            runTreeMenu();
            break;

        case 0:
            printf("Encerrando...\n");
            running = 0;
            break;

        default:
            printf("Comando invalido!\n");
        }
    }
}