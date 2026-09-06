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
    int capacity = utilsReadInt("Qual a capacidade inicial?\n");

    if (capacity <= 0)
    {
        printf("Capacidade invalida!\n");
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
        printf("Erro ao criar a heap\n");
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
            printf("Erro ao alocar memoria\n");
            exit(1);
        }
    }

    heaps[totalHeaps].id = totalHeaps + 1;
    heaps[totalHeaps].heap = heap;

    active = totalHeaps;
    totalHeaps++;

    printf("Heap %d (%s) criada e definida como ativa!\n", heaps[active].id, heapTypeName(type));
}

static void listHeapInstances(void)
{
    if (totalHeaps == 0)
    {
        printf("Nenhuma heap criada ainda. Use o comando [1] para criar uma.\n");
        return;
    }

    printf("=== Heaps criadas ===\n");

    for (int i = 0; i < totalHeaps; i++)
    {
        if (i == active)
        {
            printf("Heap %d - %s (ativa)\n", heaps[i].id, heapTypeName(heaps[i].heap->type));
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
        printf("Heap invalida!\n");
        return;
    }

    active = number - 1;
    printf("Heap ativa agora: %d (%s)\n", heaps[active].id, heapTypeName(heaps[active].heap->type));
}

void runHeapMenu(void)
{
    int command, data;
    int running = 1;

    while (running)
    {
        printf("\n=== MENU HEAPS");

        if (active != -1)
        {
            printf(" | Heap ativa: %d (%s)", heaps[active].id, heapTypeName(heaps[active].heap->type));
        }

        printf(" ===\n");

        printf("[0] Voltar ao menu principal\n");
        printf("[1] Cria uma nova heap\n");
        printf("[2] Lista as heaps criadas\n");
        printf("[3] Troca a heap ativa\n");
        printf("[4] Mostra a heap atual\n");
        printf("[5] Insere elemento\n");
        printf("[6] Extrai o topo (maior na Max-Heap, menor na Min-Heap)\n");
        printf("[7] Atualiza um elemento\n");
        printf("[8] Aumenta a capacidade\n");
        printf("[9] Ordena com HeapSort\n");
        printf("[10] Transforma o vetor em Heap (BuildHeap)\n");

        command = utilsReadInt("Escolha: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("Voce precisa criar uma heap primeiro! (comando 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
        {
            int typeChoice = utilsReadInt("Qual tipo de heap?\n[1] Max-Heap\n[2] Min-Heap\n");

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
                printf("Tipo invalido!\n");
            }

            break;
        }

        case 2:
            listHeapInstances();
            break;

        case 3:
        {
            listHeapInstances();

            int number = utilsReadInt("Qual heap deseja ativar?\n");

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
            data = utilsReadInt("Qual elemento?\n");

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
                printf("Erro: nao foi possivel extrair o topo\n");
                break;
            }

            printf("Valor extraido: %d\n", top);

            break;
        }

        case 7:
        {
            int index = utilsReadInt("Em qual index?\n");
            int newData = utilsReadInt("Para qual valor?\n");

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
                printf("Erro: nao foi possivel atualizar\n");
                break;
            }

            printf("Alteracao feita!\n");

            break;
        }

        case 8:
        {
            int newCapacity = utilsReadInt("Qual a nova capacidade?\n");
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
                printf("Erro no ReSize\n");
                break;
            }

            printf("Capacidade alterada!\n");

            break;
        }

        case 9:
            if (!heaps[active].heap->isHeap)
            {
                printf("Erro: o vetor atual nao e uma Heap\n");
                printf("Faca BuildHeap primeiro\n");
                break;
            }

            if (heaps[active].heap->type == HEAP_MAX)
            {
                maxHeapSort(heaps[active].heap);
                printf("Vetor ordenado (crescente)!\n");
            }
            else
            {
                minHeapSort(heaps[active].heap);
                printf("Vetor ordenado (decrescente)!\n");
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

            printf("Heap construida!\n");

            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }
}