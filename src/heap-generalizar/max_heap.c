#include <stdio.h>
#include <stdlib.h>
/*

typedef struct
{
    int *array;
    int size;
    int capacity;
    int isHeap;
} Heap;

void pause()
{
    printf("\nPressione ENTER para continuar...");

    while (getchar() != '\n')
        ;

    getchar();
}

Heap *createHeap(int capacity)
{
    Heap *h = malloc(sizeof(*h));

    if (h == NULL)
    {
        return NULL;
    }

    h->array = malloc(capacity * sizeof(int));

    if (h->array == NULL)
    {
        free(h);
        return NULL;
    }

    h->size = 0;
    h->capacity = capacity;
    h->isHeap = 1;

    return h;
}

void destroyHeap(Heap *h)
{
    if (h == NULL)
    {
        return;
    }

    free(h->array);
    free(h);
}

void printHeap(Heap *h)
{
    if (h == NULL)
    {
        return;
    }

    printf("data: [");

    for (int i = 0; i < h->size; i++)
    {
        printf("%d", h->array[i]);

        if (i < h->size - 1)
        {
            printf(", ");
        }
    }

    printf("]\n");

    printf("\nCapacidade: %d\n", h->capacity);
    printf("Tamanho: %d\n", h->size);

    if (h->isHeap)
    {
        printf("Estado: Max-Heap\n");
    }
    else
    {
        printf("Estado: Vetor Ordenado / Nao eh uma Heap\n");
    }
}

void heapifyUp(Heap *h, int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (h->array[index] > h->array[parent])
        {
            int temp = h->array[index];
            h->array[index] = h->array[parent];
            h->array[parent] = temp;

            index = parent;
        }
        else
        {
            break;
        }
    }
}

void heapifyDown(Heap *h, int index)
{
    while (2 * index + 1 < h->size)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int maxChild = leftChild;

        if (rightChild < h->size &&
            h->array[rightChild] > h->array[leftChild])
        {
            maxChild = rightChild;
        }

        if (h->array[index] >= h->array[maxChild])
        {
            break;
        }

        int temp = h->array[maxChild];
        h->array[maxChild] = h->array[index];
        h->array[index] = temp;

        index = maxChild;
    }
}

int reSize(Heap *h, int newCapacity)
{
    if (h == NULL)
    {
        return 0;
    }

    if (newCapacity <= 0)
    {
        return 0;
    }

    if (h->size > newCapacity)
    {
        return 0;
    }

    int *temp = realloc(h->array, newCapacity * sizeof(int));

    if (temp == NULL)
    {
        return 0;
    }

    h->array = temp;
    h->capacity = newCapacity;

    return 1;
}

void insert(Heap *h, int data)
{
    if (h == NULL)
    {
        return;
    }

    if (!h->isHeap)
    {
        printf("Erro: o vetor nao e uma Heap\n");
        printf("Faca BuildHeap antes de inserir\n");
        return;
    }

    if (h->size == h->capacity)
    {
        if (!reSize(h, h->capacity * 2))
        {
            return;
        }
    }

    int index = h->size;

    h->array[index] = data;

    h->size++;

    heapifyUp(h, index);
}

int extractMax(Heap *h, int *max)
{
    if (h == NULL || h->size == 0 || max == NULL)
    {
        return 0;
    }

    if (!h->isHeap)
    {
        return 0;
    }

    *max = h->array[0];

    int last = h->size - 1;

    h->array[0] = h->array[last];

    h->size--;

    heapifyDown(h, 0);

    return 1;
}

int update(Heap *h, int index, int newData)
{
    if (h == NULL ||
        h->size == 0 ||
        index < 0 ||
        index >= h->size)
    {
        return 0;
    }

    if (!h->isHeap)
    {
        return 0;
    }

    int oldData = h->array[index];

    h->array[index] = newData;

    if (newData > oldData)
    {
        heapifyUp(h, index);
    }
    else if (newData < oldData)
    {
        heapifyDown(h, index);
    }

    return 1;
}

void buildHeap(Heap *h)
{
    if (h == NULL || h->size == 0)
    {
        return;
    }

    int lastParent = (h->size / 2) - 1;

    for (int i = lastParent; i >= 0; i--)
    {
        heapifyDown(h, i);
    }

    h->isHeap = 1;
}

void heapSort(Heap *h)
{
    if (h == NULL || h->size <= 1)
    {
        return;
    }

    if (!h->isHeap)
    {
        return;
    }

    int originalSize = h->size;

    while (h->size > 1)
    {
        int temp = h->array[h->size - 1];

        h->array[h->size - 1] = h->array[0];

        h->array[0] = temp;

        h->size--;

        heapifyDown(h, 0);
    }

    h->size = originalSize;

    h->isHeap = 0;
}

int main()
{
    int command, data;

    Heap *h1 = createHeap(10);
    Heap *h2 = createHeap(10);

    if (h1 == NULL || h2 == NULL)
    {
        destroyHeap(h1);
        destroyHeap(h2);

        return 1;
    }

    Heap **activeHeap = &h1;

    int activeNum = 1;

    while (1)
    {
        printf("\n=== MENU | Heap ativa: %d ===\n", activeNum);

        printf("[0] Encerra o Programa\n");
        printf("[1] Mostra a Heap\n");
        printf("[2] Troca a Heap\n");
        printf("[3] Insere elemento\n");
        printf("[4] Extrai maior elemento\n");
        printf("[5] Atualiza um elemento\n");
        printf("[6] Aumenta a capacidade\n");
        printf("[7] Ordena com HeapSort\n");
        printf("[8] Transforma vetor em Heap\n");

        scanf("%d", &command);

        switch (command)
        {
        case 0:
            destroyHeap(h1);
            destroyHeap(h2);

            return 0;

        case 1:
            printHeap(*activeHeap);
            pause();
            break;

        case 2:
            if (activeHeap == &h1)
            {
                activeHeap = &h2;
                activeNum = 2;
            }
            else
            {
                activeHeap = &h1;
                activeNum = 1;
            }

            printf("Heap ativa: %d\n", activeNum);

            break;

        case 3:
            printf("Qual elemento?\n");

            scanf("%d", &data);

            insert(*activeHeap, data);

            break;

        case 4:
        {
            int max;

            if (!extractMax(*activeHeap, &max))
            {
                printf("Erro: nao foi possivel extrair o maior elemento\n");
                break;
            }

            printf("Valor maximo: %d extraido\n", max);

            break;
        }

        case 5:
        {
            int newData, index;

            printf("Em qual index?\n");

            scanf("%d", &index);

            printf("Para qual valor?\n");

            scanf("%d", &newData);

            if (!update(*activeHeap, index, newData))
            {
                printf("Erro: nao foi possivel atualizar\n");
                break;
            }

            printf("Alteracao feita!\n");

            break;
        }

        case 6:
        {
            int newCapacity;

            printf("Qual a nova capacidade?\n");

            scanf("%d", &newCapacity);

            if (!reSize(*activeHeap, newCapacity))
            {
                printf("Erro no ReSize\n");
                break;
            }

            printf("Capacidade alterada!\n");

            break;
        }

        case 7:
        {
            if (!(*activeHeap)->isHeap)
            {
                printf("Erro: o vetor atual nao e uma Heap\n");
                printf("Faca BuildHeap primeiro\n");
                break;
            }

            heapSort(*activeHeap);

            printf("Vetor ordenado!\n");

            break;
        }

        case 8:
        {
            buildHeap(*activeHeap);

            printf("Max-Heap construida!\n");

            break;
        }

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }

    return 0;
}
    
*/