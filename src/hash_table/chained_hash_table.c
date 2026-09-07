#include "common.h"
#include "hash_table.h"

static int chainedHashFunction(int size, int key)
{
    return ((key % size) + size) % size;
}

ChainedHashTable *chainedHashTableCreate(int size)
{
    ChainedHashTable *ht = malloc(sizeof(ChainedHashTable));

    if (ht == NULL)
    {
        return NULL;
    }

    ht->count = 0;
    ht->size = size;
    ht->table = calloc(size, sizeof(ChainedHashNode *));

    if (ht->table == NULL)
    {
        free(ht);
        return NULL;
    }

    return ht;
}

void chainedHashTableClearElements(ChainedHashTable *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        ChainedHashNode *current = ht->table[i];

        while (current != NULL)
        {
            ChainedHashNode *temp = current;
            current = current->next;
            free(temp);
        }

        ht->table[i] = NULL;
    }

    ht->count = 0;
}

void chainedHashTableDestroy(ChainedHashTable *ht)
{
    if (ht == NULL)
    {
        return;
    }

    chainedHashTableClearElements(ht);
    free(ht->table);
    free(ht);
}

void chainedHashTablePrint(ChainedHashTable *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        printf("[%d] -> ", i);

        ChainedHashNode *current = ht->table[i];

        while (current != NULL)
        {
            printf("%d -> ", current->key);
            current = current->next;
        }

        printf("NULL\n");
    }
}

static ChainedHashNode *createChainedHashNode(int key)
{
    ChainedHashNode *newNode = malloc(sizeof(ChainedHashNode));

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->key = key;
    newNode->next = NULL;

    return newNode;
}

void chainedHashTableInsert(ChainedHashTable *ht, int key)
{
    int index = chainedHashFunction(ht->size, key);

    ChainedHashNode *newNode = createChainedHashNode(key);

    if (newNode == NULL)
    {
        return;
    }

    newNode->next = ht->table[index];
    ht->table[index] = newNode;
    ht->count++;

    float loadFactor = (float)ht->count / ht->size;

    if (loadFactor >= 0.75)
    {
        if (chainedHashTableRehash(ht, ht->size * 2))
        {
            printf("ReHash automatico realizado!\n");
        }
    }
}

void chainedHashTableInsertNoResize(ChainedHashTable *ht, int key)
{
    int index = chainedHashFunction(ht->size, key);

    ChainedHashNode *newNode = createChainedHashNode(key);

    if (newNode == NULL)
    {
        return;
    }

    newNode->next = ht->table[index];
    ht->table[index] = newNode;
    ht->count++;
}

int chainedHashTableRemove(ChainedHashTable *ht, int key)
{
    int index = chainedHashFunction(ht->size, key);

    ChainedHashNode *current = ht->table[index];
    ChainedHashNode *prev = NULL;

    while (current != NULL)
    {
        if (current->key == key)
        {
            if (prev == NULL)
            {
                ht->table[index] = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            free(current);
            ht->count--;

            return 1;
        }

        prev = current;
        current = current->next;
    }

    return 0;
}

ChainedHashNode *chainedHashTableSearch(ChainedHashTable *ht, int key)
{
    int index = chainedHashFunction(ht->size, key);

    ChainedHashNode *current = ht->table[index];

    while (current != NULL)
    {
        if (current->key == key)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

int chainedHashTableRehash(ChainedHashTable *ht, int newSize)
{
    ChainedHashNode **newTable = calloc(newSize, sizeof(ChainedHashNode *));

    if (newTable == NULL)
    {
        return 0;
    }

    for (int i = 0; i < ht->size; i++)
    {
        ChainedHashNode *current = ht->table[i];

        while (current != NULL)
        {
            ChainedHashNode *next = current->next;

            int newIndex = chainedHashFunction(newSize, current->key);

            current->next = newTable[newIndex];
            newTable[newIndex] = current;

            current = next;
        }
    }

    free(ht->table);

    ht->table = newTable;
    ht->size = newSize;

    return 1;
}

ChainedHashTable *chainedHashTableRehashRealloc(ChainedHashTable *ht, int newSize)
{
    ChainedHashTable *newHt = chainedHashTableCreate(newSize);

    if (newHt == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < ht->size; i++)
    {
        ChainedHashNode *current = ht->table[i];

        while (current != NULL)
        {
            chainedHashTableInsertNoResize(newHt, current->key);
            current = current->next;
        }
    }

    chainedHashTableDestroy(ht);

    return newHt;
}

void chainedHashTableCompare(ChainedHashTable *ht1, ChainedHashTable *ht2)
{
    float loadFactor1 = (float)ht1->count / ht1->size;
    float loadFactor2 = (float)ht2->count / ht2->size;

    int collisions1 = 0;
    int collisions2 = 0;

    int maxChain1 = 0;
    int maxChain2 = 0;

    for (int i = 0; i < ht1->size; i++)
    {
        int chainSize = 0;

        ChainedHashNode *current = ht1->table[i];

        while (current != NULL)
        {
            chainSize++;
            current = current->next;
        }

        if (chainSize > 1)
        {
            collisions1 += chainSize - 1;
        }

        if (chainSize > maxChain1)
        {
            maxChain1 = chainSize;
        }
    }

    for (int i = 0; i < ht2->size; i++)
    {
        int chainSize = 0;

        ChainedHashNode *current = ht2->table[i];

        while (current != NULL)
        {
            chainSize++;
            current = current->next;
        }

        if (chainSize > 1)
        {
            collisions2 += chainSize - 1;
        }

        if (chainSize > maxChain2)
        {
            maxChain2 = chainSize;
        }
    }

    printf("\n====== Comparacao ======\n");

    printf("\nTabela 1:");
    printf("\nElementos: %d", ht1->count);
    printf("\nTamanho: %d", ht1->size);
    printf("\nLoad Factor: %.2f", loadFactor1);
    printf("\nColisoes: %d", collisions1);
    printf("\nMaior lista: %d", maxChain1);

    printf("\n\nTabela 2:");
    printf("\nElementos: %d", ht2->count);
    printf("\nTamanho: %d", ht2->size);
    printf("\nLoad Factor: %.2f", loadFactor2);
    printf("\nColisoes: %d", collisions2);
    printf("\nMaior lista: %d", maxChain2);

    printf("\n\nMelhor tabela: ");

    if (collisions1 < collisions2)
    {
        printf("Tabela 1 (menos colisoes)\n");
    }
    else if (collisions2 < collisions1)
    {
        printf("Tabela 2 (menos colisoes)\n");
    }
    else
    {
        printf("Empate\n");
    }
}