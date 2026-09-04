#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10

typedef struct
{
    int *data;
    int size;
    int capacity;
} Array;

void pause()
{
    printf("\nPressione ENTER para continuar...");
    while (getchar() != '\n')
        ;
    getchar();
}

void createArray(Array *array)
{
    array->data = malloc(INITIAL_CAPACITY * sizeof(int));
    array->size = 0;
    array->capacity = INITIAL_CAPACITY;
}

void clearArray(Array *array)
{
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}

int isEmpty(Array *array)
{
    return array->size == 0;
}

int size(Array *array)
{
    return array->size;
}

void resize(Array *array)
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

void push(Array *array, int data)
{
    if (array->size == array->capacity)
    {
        resize(array);
    }

    array->data[array->size] = data;
    array->size++;
}

void insert(Array *array, int index, int data)
{
    if (index < 0 || index > array->size)
    {
        return;
    }

    if (array->size == array->capacity)
    {
        resize(array);
    }

    for (int i = array->size; i > index; i--)
    {
        array->data[i] = array->data[i - 1];
    }

    array->data[index] = data;
    array->size++;
}

int removeAt(Array *array, int index)
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

int get(Array *array, int index)
{
    if (index < 0 || index >= array->size)
    {
        return -1;
    }

    return array->data[index];
}

void set(Array *array, int index, int data)
{
    if (index < 0 || index >= array->size)
    {
        return;
    }

    array->data[index] = data;
}

int contains(Array *array, int data)
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

int countOccurrences(Array *array, int data)
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

void printArray(Array *array)
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

void copyArray(Array *destination, Array *source)
{
    clearArray(destination);
    createArray(destination);

    while (destination->capacity < source->size)
    {
        resize(destination);
    }

    for (int i = 0; i < source->size; i++)
    {
        destination->data[i] = source->data[i];
    }

    destination->size = source->size;
}

int compareArrays(Array *array1, Array *array2)
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

void removeOccurrences(Array *array, int data)
{
    int i = 0;

    while (i < array->size)
    {
        if (array->data[i] == data)
        {
            removeAt(array, i);
        }
        else
        {
            i++;
        }
    }
}

void removeDuplicates(Array *array)
{
    for (int i = 0; i < array->size; i++)
    {
        int j = i + 1;

        while (j < array->size)
        {
            if (array->data[i] == array->data[j])
            {
                removeAt(array, j);
            }
            else
            {
                j++;
            }
        }
    }
}

void reverseArray(Array *array)
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

int main()
{
    int command, data, index;

    Array array1;
    Array array2;

    createArray(&array1);
    createArray(&array2);

    Array *active = &array1;
    int activeNum = 1;

    while (1)
    {
        printf("\n=== MENU | Array ativo: %d ===\n", activeNum);

        printf("\n[1] Mostrar Array Atual\n");
        printf("[2] Trocar de Array\n");
        printf("[3] Inserir no fim do Array\n");
        printf("[4] Inserir em uma posicao\n");
        printf("[5] Remover de uma posicao\n");
        printf("[6] Mostrar elemento de uma posicao\n");
        printf("[7] Alterar elemento de uma posicao\n");
        printf("[8] Buscar um valor no Array\n");
        printf("[9] Contar ocorrencias de um valor\n");
        printf("[10] Copiar Array ativo no outro\n");
        printf("[11] Comparar os dois Arrays\n");
        printf("[12] Remover ocorrencias de um valor\n");
        printf("[13] Remover duplicatas\n");
        printf("[14] Inverter Array\n");
        printf("[15] Mostrar tamanho do Array\n");
        printf("[16] Verificar se o Array esta vazio\n");

        printf("\n[0] Encerrar o Programa\n");

        scanf("%d", &command);

        switch (command)
        {
        case 0:
            clearArray(&array1);
            clearArray(&array2);
            return 0;

        case 1:
            printArray(active);
            pause();
            break;

        case 2:
            if (activeNum == 1)
            {
                active = &array2;
                activeNum = 2;
            }
            else
            {
                active = &array1;
                activeNum = 1;
            }
            break;

        case 3:
            printf("Qual elemento deve ser adicionado?\n");
            scanf("%d", &data);

            push(active, data);

            printf("Elemento adicionado!\n");
            break;

        case 4:
            printf("Qual posicao?\n");
            scanf("%d", &index);

            printf("Qual elemento?\n");
            scanf("%d", &data);

            insert(active, index, data);

            printf("Elemento inserido!\n");
            break;

        case 5:
        {
            printf("Qual posicao deve ser removida?\n");
            scanf("%d", &index);

            int removed = removeAt(active, index);

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

        case 6:
        {
            printf("Qual posicao?\n");
            scanf("%d", &index);

            int value = get(active, index);

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

        case 7:
            printf("Qual posicao?\n");
            scanf("%d", &index);

            printf("Qual sera o novo valor?\n");
            scanf("%d", &data);

            set(active, index, data);

            printf("Elemento alterado!\n");
            break;

        case 8:
        {
            printf("Qual elemento deve ser procurado?\n");
            scanf("%d", &data);

            int pos = contains(active, data);

            if (pos == -1)
            {
                printf("Elemento nao encontrado!\n");
            }
            else
            {
                printf(
                    "Elemento %d encontrado na posicao %d\n",
                    data,
                    pos);
            }

            break;
        }

        case 9:
        {
            printf("Qual elemento deve ser contado?\n");
            scanf("%d", &data);

            int count = countOccurrences(active, data);

            printf(
                "O elemento %d aparece %d vez(es) no Array\n",
                data,
                count);

            break;
        }

        case 10:
            if (activeNum == 1)
            {
                copyArray(&array2, active);
            }
            else
            {
                copyArray(&array1, active);
            }

            printf("Array copiado!\n");
            break;

        case 11:
            if (compareArrays(&array1, &array2))
            {
                printf("Arrays iguais!\n");
            }
            else
            {
                printf("Arrays diferentes!\n");
            }

            break;

        case 12:
            printf("Qual elemento deve ser removido?\n");
            scanf("%d", &data);

            removeOccurrences(active, data);

            printf("Ocorrencias removidas!\n");
            break;

        case 13:
            removeDuplicates(active);

            printf("Duplicatas removidas!\n");
            break;

        case 14:
            reverseArray(active);

            printf("Array invertido!\n");
            break;

        case 15:
            printf("Tamanho do Array: %d\n", size(active));
            break;

        case 16:
            if (isEmpty(active))
            {
                printf("Array vazio!\n");
            }
            else
            {
                printf("Array nao esta vazio!\n");
            }

            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }

    return 0;
}