#include "common.h"
#include "hash_table.h"

static int hashFunction(int size, int key)
{
    return ((key % size) + size) % size;
}

HashTable *hashTableCreate(int size)
{
    HashTable *ht = malloc(sizeof(HashTable));

    if (ht == NULL)
    {
        return NULL;
    }

    ht->count = 0;
    ht->size = size;
    ht->table = calloc(size, sizeof(HashNode *));

    if (ht->table == NULL)
    {
        free(ht);
        return NULL;
    }

    return ht;
}

void hashTableClearElements(HashTable *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        HashNode *current = ht->table[i];

        while (current != NULL)
        {
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }

        ht->table[i] = NULL;
    }

    ht->count = 0;
}

void hashTableDestroy(HashTable *ht)
{
    if (ht == NULL)
    {
        return;
    }

    hashTableClearElements(ht);
    free(ht->table);
    free(ht);
}

void hashTablePrint(HashTable *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        printf("[%d] -> ", i);

        HashNode *current = ht->table[i];

        while (current != NULL)
        {
            printf("%d -> ", current->key);
            current = current->next;
        }

        printf("NULL\n");
    }
}

static HashNode *createHashNode(int key)
{
    HashNode *newNode = malloc(sizeof(HashNode));

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->key = key;
    newNode->next = NULL;

    return newNode;
}

void hashTableInsert(HashTable *ht, int key)
{
    int index = hashFunction(ht->size, key);

    HashNode *newNode = createHashNode(key);

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
        if (hashTableRehash(ht, ht->size * 2))
        {
            printf("ReHash automatico realizado!\n");
        }
    }
}

void hashTableInsertNoResize(HashTable *ht, int key)
{
    int index = hashFunction(ht->size, key);

    HashNode *newNode = createHashNode(key);

    if (newNode == NULL)
    {
        return;
    }

    newNode->next = ht->table[index];
    ht->table[index] = newNode;
    ht->count++;
}

int hashTableRemove(HashTable *ht, int key)
{
    int index = hashFunction(ht->size, key);

    HashNode *current = ht->table[index];
    HashNode *prev = NULL;

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

HashNode *hashTableSearch(HashTable *ht, int key)
{
    int index = hashFunction(ht->size, key);

    HashNode *current = ht->table[index];

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

int hashTableRehash(HashTable *ht, int newSize)
{
    HashNode **newTable = calloc(newSize, sizeof(HashNode *));

    if (newTable == NULL)
    {
        return 0;
    }

    for (int i = 0; i < ht->size; i++)
    {
        HashNode *current = ht->table[i];

        while (current != NULL)
        {
            HashNode *next = current->next;

            int newIndex = hashFunction(newSize, current->key);

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

HashTable *hashTableRehashRealloc(HashTable *ht, int newSize)
{
    HashTable *newHt = hashTableCreate(newSize);

    if (newHt == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < ht->size; i++)
    {
        HashNode *current = ht->table[i];

        while (current != NULL)
        {
            hashTableInsertNoResize(newHt, current->key);
            current = current->next;
        }
    }

    hashTableDestroy(ht);

    return newHt;
}

void hashTableCompare(HashTable *ht1, HashTable *ht2)
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

        HashNode *current = ht1->table[i];

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

        HashNode *current = ht2->table[i];

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