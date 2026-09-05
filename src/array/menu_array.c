#include "common.h"
#include "utils.h"
#include "array.h"
#include "sorting.h"

typedef struct
{
    int id;
    Array data;
} ArrayInstance;

static ArrayInstance *arrays = NULL;
static int totalArrays = 0;
static int arraysCapacity = 0;
static int active = -1; /* indice em 'arrays', -1 = nenhum array ativo */

static void createArrayInstance(void)
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
            printf("Erro ao alocar memoria\n");
            exit(1);
        }
    }

    arrays[totalArrays].id = totalArrays + 1;
    arrayCreate(&arrays[totalArrays].data);

    active = totalArrays;
    totalArrays++;

    printf("Array %d criado e definido como ativo!\n", arrays[active].id);
}

static void listArrayInstances(void)
{
    if (totalArrays == 0)
    {
        printf("Nenhum array criado ainda. Use o comando [1] para criar um.\n");
        return;
    }

    printf("=== Arrays criados ===\n");

    for (int i = 0; i < totalArrays; i++)
    {
        if (i == active)
        {
            printf("Array %d (ativo)\n", arrays[i].id);
        }
        else
        {
            printf("Array %d\n", arrays[i].id);
        }
    }
}

static void switchArrayInstance(int number)
{
    if (number < 1 || number > totalArrays)
    {
        printf("Array invalido!\n");
        return;
    }

    active = number - 1;
    printf("Array ativo agora: %d\n", arrays[active].id);
}

static void printSortMenu(void)
{
    printf("\nQual algoritmo de ordenacao?\n");
    printf("[1] Bubble Sort\n");
    printf("[2] Selection Sort\n");
    printf("[3] Insertion Sort\n");
    printf("[4] Counting Sort (somente valores nao negativos)\n");
    printf("[5] Merge Sort\n");
    printf("[6] Quick Sort (particao de Lomuto)\n");
    printf("[7] Quick Sort (particao de Hoare)\n");
    printf("[8] Heap Sort\n");
}

static void sortActiveArray(void)
{
    printSortMenu();

    int choice = utilsReadInt("Escolha: ");
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
        printf("Algoritmo invalido!\n");
        return;
    }

    Array *activeArray = &arrays[active].data;

    sortArray(activeArray->data, activeArray->size, type);

    printf("Array ordenado!\n");
}

void runArrayMenu(void)
{
    int command, data, index;
    int running = 1;

    while (running)
    {
        printf("\n=== MENU ARRAYS");

        if (active != -1)
        {
            printf(" | Array ativo: %d", arrays[active].id);
        }

        printf(" ===\n");

        printf("[0] Voltar ao menu principal\n");
        printf("[1] Cria um novo array\n");
        printf("[2] Lista os arrays criados\n");
        printf("[3] Troca o array ativo\n");
        printf("[4] Mostra o array atual\n");
        printf("[5] Insere no fim do array\n");
        printf("[6] Insere em uma posicao\n");
        printf("[7] Remove de uma posicao\n");
        printf("[8] Mostra elemento de uma posicao\n");
        printf("[9] Altera elemento de uma posicao\n");
        printf("[10] Busca um valor no array\n");
        printf("[11] Conta ocorrencias de um valor\n");
        printf("[12] Copia o array ativo (cria um novo)\n");
        printf("[13] Compara dois arrays\n");
        printf("[14] Remove ocorrencias de um valor\n");
        printf("[15] Remove duplicatas\n");
        printf("[16] Inverte o array\n");
        printf("[17] Mostra o tamanho do array\n");
        printf("[18] Verifica se o array esta vazio\n");
        printf("[19] Ordena o array\n");

        command = utilsReadInt("Escolha: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("Voce precisa criar um array primeiro! (comando 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
            createArrayInstance();
            break;

        case 2:
            listArrayInstances();
            break;

        case 3:
        {
            listArrayInstances();

            int number = utilsReadInt("Qual array deseja ativar?\n");

            switchArrayInstance(number);
            break;
        }

        case 4:
            arrayPrint(&arrays[active].data);
            utilsPause();
            break;

        case 5:
            data = utilsReadInt("Qual elemento deve ser adicionado?\n");

            arrayPush(&arrays[active].data, data);

            printf("Elemento adicionado!\n");
            break;

        case 6:
            index = utilsReadInt("Qual posicao?\n");
            data = utilsReadInt("Qual elemento?\n");

            arrayInsert(&arrays[active].data, index, data);

            printf("Elemento inserido!\n");
            break;

        case 7:
        {
            index = utilsReadInt("Qual posicao deve ser removida?\n");

            int removed = arrayRemoveAt(&arrays[active].data, index);

            if (removed == -1)
            {
                printf("Posicao invalida!\n");
            }
            else
            {
                printf("Elemento %d removido!\n", removed);
            }

            break;
        }

        case 8:
        {
            index = utilsReadInt("Qual posicao?\n");

            int value = arrayGet(&arrays[active].data, index);

            if (value == -1)
            {
                printf("Posicao invalida!\n");
            }
            else
            {
                printf("O elemento da posicao %d eh: %d\n", index, value);
            }

            break;
        }

        case 9:
            index = utilsReadInt("Qual posicao?\n");
            data = utilsReadInt("Qual sera o novo valor?\n");

            arraySet(&arrays[active].data, index, data);

            printf("Elemento alterado!\n");
            break;

        case 10:
        {
            data = utilsReadInt("Qual elemento deve ser procurado?\n");

            int pos = arrayContains(&arrays[active].data, data);

            if (pos == -1)
            {
                printf("Elemento nao encontrado!\n");
            }
            else
            {
                printf("Elemento %d encontrado na posicao %d\n", data, pos);
            }

            break;
        }

        case 11:
        {
            data = utilsReadInt("Qual elemento deve ser contado?\n");

            int count = arrayCountOccurrences(&arrays[active].data, data);

            printf("O elemento %d aparece %d vez(es) no array\n", data, count);

            break;
        }

        case 12:
        {
            int sourceIndex = active;
            int sourceId = arrays[sourceIndex].id;

            createArrayInstance();
            arrayCopy(&arrays[active].data, &arrays[sourceIndex].data);

            printf("Array %d copiado para o novo array %d!\n", sourceId, arrays[active].id);

            break;
        }

        case 13:
        {
            listArrayInstances();

            int firstNumber = utilsReadInt("Primeiro array:\n");
            int secondNumber = utilsReadInt("Segundo array:\n");

            if (firstNumber < 1 || firstNumber > totalArrays || secondNumber < 1 || secondNumber > totalArrays)
            {
                printf("Array invalido!\n");
                break;
            }

            if (arrayCompare(&arrays[firstNumber - 1].data, &arrays[secondNumber - 1].data))
            {
                printf("Arrays iguais!\n");
            }
            else
            {
                printf("Arrays diferentes!\n");
            }

            break;
        }

        case 14:
            data = utilsReadInt("Qual elemento deve ser removido?\n");

            arrayRemoveOccurrences(&arrays[active].data, data);

            printf("Ocorrencias removidas!\n");
            break;

        case 15:
            arrayRemoveDuplicates(&arrays[active].data);

            printf("Duplicatas removidas!\n");
            break;

        case 16:
            arrayReverse(&arrays[active].data);

            printf("Array invertido!\n");
            break;

        case 17:
            printf("Tamanho do array: %d\n", arraySize(&arrays[active].data));
            break;

        case 18:
            if (arrayIsEmpty(&arrays[active].data))
            {
                printf("Array vazio!\n");
            }
            else
            {
                printf("Array nao esta vazio!\n");
            }

            break;

        case 19:
            sortActiveArray();
            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }
}