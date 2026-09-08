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
        return "Dinamico";
    }

    return "Estatico";
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
            printf("Erro ao alocar memoria\n");
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

    printf("Array %d (%s) criado e definido como ativo!\n", arrays[active].id, arrayTypeName(type));
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
            printf("Array %d - %s (ativo)\n", arrays[i].id, arrayTypeName(arrays[i].type));
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
        printf("Array invalido!\n");
        return;
    }

    active = number - 1;
    printf("Array ativo agora: %d (%s)\n", arrays[active].id, arrayTypeName(arrays[active].type));
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

    int *dataPtr;
    int sizeVal;

    getActiveRaw(&dataPtr, &sizeVal);

    sortArray(dataPtr, sizeVal, type);

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
            printf(" | Array ativo: %d (%s)", arrays[active].id, arrayTypeName(arrays[active].type));
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
        printf("[13] Compara dois arrays (mesmo tipo)\n");
        printf("[14] Remove ocorrencias de um valor\n");
        printf("[15] Remove duplicatas\n");
        printf("[16] Inverte o array\n");
        printf("[17] Mostra o tamanho do array\n");
        printf("[18] Verifica se o array esta vazio\n");
        printf("[19] Ordena o array\n");
        printf("[20] Verifica se o array esta cheio (somente Estatico)\n");

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
        {
            int typeChoice = utilsReadInt("Qual tipo de array?\n[1] Dinamico (cresce sozinho)\n[2] Estatico (capacidade fixa)\n");

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
                printf("Tipo invalido!\n");
            }

            break;
        }

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
            data = utilsReadInt("Qual elemento deve ser adicionado?\n");

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
                printf("Elemento adicionado!\n");
            }

            break;
        }

        case 6:
        {
            index = utilsReadInt("Qual posicao?\n");
            data = utilsReadInt("Qual elemento?\n");

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
                printf("Elemento inserido!\n");
            }

            break;
        }

        case 7:
        {
            index = utilsReadInt("Qual posicao deve ser removida?\n");

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

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                dynamicArraySet(&arrays[active].data.dynamic, index, data);
            }
            else
            {
                staticArraySet(&arrays[active].data.staticArr, index, data);
            }

            printf("Elemento alterado!\n");

            break;

        case 10:
        {
            data = utilsReadInt("Qual elemento deve ser procurado?\n");

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

            int count;

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                count = dynamicArrayCountOccurrences(&arrays[active].data.dynamic, data);
            }
            else
            {
                count = staticArrayCountOccurrences(&arrays[active].data.staticArr, data);
            }

            printf("O elemento %d aparece %d vez(es) no array\n", data, count);

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

            ArrayInstance *first = &arrays[firstNumber - 1];
            ArrayInstance *second = &arrays[secondNumber - 1];

            if (first->type != second->type)
            {
                printf("So da para comparar arrays do mesmo tipo!\n");
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

            if (arrays[active].type == ARRAY_DYNAMIC)
            {
                dynamicArrayRemoveOccurrences(&arrays[active].data.dynamic, data);
            }
            else
            {
                staticArrayRemoveOccurrences(&arrays[active].data.staticArr, data);
            }

            printf("Ocorrencias removidas!\n");

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

            printf("Duplicatas removidas!\n");

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

            printf("Array invertido!\n");

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

            printf("Tamanho do array: %d\n", total);

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
                printf("Array vazio!\n");
            }
            else
            {
                printf("Array nao esta vazio!\n");
            }

            break;
        }

        case 19:
            sortActiveArray();
            break;

        case 20:
            if (arrays[active].type != ARRAY_STATIC)
            {
                printf("Esse comando so faz sentido para arrays Estaticos!\n");
            }
            else if (staticArrayIsFull(&arrays[active].data.staticArr))
            {
                printf("Array cheio!\n");
            }
            else
            {
                printf("Array nao esta cheio!\n");
            }

            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }
}