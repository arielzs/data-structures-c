#include "common.h"
#include "hash_table.h"

static int isPrime(int n)
{
    if (n < 2)
    {
        return 0;
    }

    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

static int nextPrime(int n)
{
    if (n < 2)
    {
        n = 2;
    }

    while (!isPrime(n))
    {
        n++;
    }

    return n;
}

static int hash1(int size, int key)
{
    return ((key % size) + size) % size;
}

static int hash2(int size, int key)
{
    int size2 = size - 1;

    if (size2 <= 0)
    {
        return 1;
    }

    return 1 + (((key % size2) + size2) % size2);
}

DoubleHashTable *doubleHashTableCreate(int size)
{
    if (size <= 0)
    {
        return NULL;
    }

    int actualSize = nextPrime(size);

    DoubleHashTable *dht = malloc(sizeof(DoubleHashTable));

    if (dht == NULL)
    {
        return NULL;
    }

    dht->table = malloc(actualSize * sizeof(DoubleHashSlot));

    if (dht->table == NULL)
    {
        free(dht);
        return NULL;
    }

    for (int i = 0; i < actualSize; i++)
    {
        dht->table[i].status = SLOT_EMPTY;
    }

    dht->size = actualSize;
    dht->count = 0;

    return dht;
}

void doubleHashTableClearElements(DoubleHashTable *dht)
{
    for (int i = 0; i < dht->size; i++)
    {
        dht->table[i].status = SLOT_EMPTY;
    }

    dht->count = 0;
}

void doubleHashTableDestroy(DoubleHashTable *dht)
{
    if (dht == NULL)
    {
        return;
    }

    free(dht->table);
    free(dht);
}

void doubleHashTablePrint(DoubleHashTable *dht)
{
    for (int i = 0; i < dht->size; i++)
    {
        printf("[%d] -> ", i);

        if (dht->table[i].status == SLOT_OCCUPIED)
        {
            printf("%d\n", dht->table[i].key);
        }
        else if (dht->table[i].status == SLOT_DELETED)
        {
            printf("removido\n");
        }
        else
        {
            printf("vazio\n");
        }
    }
}

int doubleHashTableInsertNoResize(DoubleHashTable *dht, int key)
{
    if (dht->count >= dht->size)
    {
        return 0;
    }

    int index = hash1(dht->size, key);
    int step = hash2(dht->size, key);
    int firstDeleted = -1;

    for (int i = 0; i < dht->size; i++)
    {
        int probe = (index + i * step) % dht->size;

        if (dht->table[probe].status == SLOT_OCCUPIED && dht->table[probe].key == key)
        {
            printf("Valor Repetido\n");
            return 0;
        }

        if (dht->table[probe].status == SLOT_DELETED && firstDeleted == -1)
        {
            firstDeleted = probe;
        }

        if (dht->table[probe].status == SLOT_EMPTY)
        {
            int insertAt = probe;

            if (firstDeleted != -1)
            {
                insertAt = firstDeleted;
            }

            dht->table[insertAt].key = key;
            dht->table[insertAt].status = SLOT_OCCUPIED;
            dht->count++;

            return 1;
        }
    }

    return 0;
}

void doubleHashTableInsert(DoubleHashTable *dht, int key)
{
    if (dht == NULL)
    {
        return;
    }

    if (!doubleHashTableInsertNoResize(dht, key))
    {
        return;
    }

    float loadFactor = (float)dht->count / dht->size;

    if (loadFactor >= 0.75)
    {
        if (doubleHashTableRehash(dht, dht->size * 2))
        {
            printf("ReHash automatico realizado!\n");
        }
    }
}

int doubleHashTableRemove(DoubleHashTable *dht, int key)
{
    if (dht == NULL)
    {
        return 0;
    }

    int index = hash1(dht->size, key);
    int step = hash2(dht->size, key);

    for (int i = 0; i < dht->size; i++)
    {
        int probe = (index + i * step) % dht->size;

        if (dht->table[probe].status == SLOT_EMPTY)
        {
            return 0;
        }

        if (dht->table[probe].status == SLOT_OCCUPIED && dht->table[probe].key == key)
        {
            dht->table[probe].status = SLOT_DELETED;
            dht->count--;

            return 1;
        }
    }

    return 0;
}

int doubleHashTableSearch(DoubleHashTable *dht, int key)
{
    if (dht == NULL)
    {
        return -1;
    }

    int index = hash1(dht->size, key);
    int step = hash2(dht->size, key);

    for (int i = 0; i < dht->size; i++)
    {
        int probe = (index + i * step) % dht->size;

        if (dht->table[probe].status == SLOT_EMPTY)
        {
            return -1;
        }

        if (dht->table[probe].status == SLOT_OCCUPIED && dht->table[probe].key == key)
        {
            return probe;
        }
    }

    return -1;
}

int doubleHashTableRehash(DoubleHashTable *dht, int newSize)
{
    if (dht == NULL)
    {
        return 0;
    }

    int actualSize = nextPrime(newSize);

    DoubleHashSlot *newTable = malloc(actualSize * sizeof(DoubleHashSlot));

    if (newTable == NULL)
    {
        return 0;
    }

    for (int i = 0; i < actualSize; i++)
    {
        newTable[i].status = SLOT_EMPTY;
    }

    DoubleHashSlot *oldTable = dht->table;
    int oldSize = dht->size;

    dht->table = newTable;
    dht->size = actualSize;
    dht->count = 0;

    for (int i = 0; i < oldSize; i++)
    {
        if (oldTable[i].status == SLOT_OCCUPIED)
        {
            doubleHashTableInsertNoResize(dht, oldTable[i].key);
        }
    }

    free(oldTable);

    return 1;
}

DoubleHashTable *doubleHashTableRehashRealloc(DoubleHashTable *dht, int newSize)
{
    if (dht == NULL)
    {
        return NULL;
    }

    DoubleHashTable *newDht = doubleHashTableCreate(newSize);

    if (newDht == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < dht->size; i++)
    {
        if (dht->table[i].status == SLOT_OCCUPIED)
        {
            doubleHashTableInsertNoResize(newDht, dht->table[i].key);
        }
    }

    doubleHashTableDestroy(dht);

    return newDht;
}

/* Simula uma busca do zero e conta quantos slots foram testados ate achar
   a key (1 = achou de primeira, sem colisao; N = N-1 colisoes no caminho) */
static int doubleHashTableProbeCount(DoubleHashTable *dht, int key)
{
    int index = hash1(dht->size, key);
    int step = hash2(dht->size, key);

    for (int i = 0; i < dht->size; i++)
    {
        int probe = (index + i * step) % dht->size;

        if (dht->table[probe].status == SLOT_OCCUPIED && dht->table[probe].key == key)
        {
            return i + 1;
        }
    }

    return 0;
}

void doubleHashTableCompare(DoubleHashTable *dht1, DoubleHashTable *dht2)
{
    float loadFactor1 = (float)dht1->count / dht1->size;
    float loadFactor2 = (float)dht2->count / dht2->size;

    int collisions1 = 0;
    int collisions2 = 0;

    int maxProbe1 = 0;
    int maxProbe2 = 0;

    for (int i = 0; i < dht1->size; i++)
    {
        if (dht1->table[i].status == SLOT_OCCUPIED)
        {
            int probes = doubleHashTableProbeCount(dht1, dht1->table[i].key);

            if (probes > 1)
            {
                collisions1 += probes - 1;
            }

            if (probes > maxProbe1)
            {
                maxProbe1 = probes;
            }
        }
    }

    for (int i = 0; i < dht2->size; i++)
    {
        if (dht2->table[i].status == SLOT_OCCUPIED)
        {
            int probes = doubleHashTableProbeCount(dht2, dht2->table[i].key);

            if (probes > 1)
            {
                collisions2 += probes - 1;
            }

            if (probes > maxProbe2)
            {
                maxProbe2 = probes;
            }
        }
    }

    printf("\n====== Comparacao ======\n");

    printf("\nTabela 1:");
    printf("\nElementos: %d", dht1->count);
    printf("\nTamanho: %d", dht1->size);
    printf("\nLoad Factor: %.2f", loadFactor1);
    printf("\nColisoes: %d", collisions1);
    printf("\nMaior sequencia de probing: %d", maxProbe1);

    printf("\n\nTabela 2:");
    printf("\nElementos: %d", dht2->count);
    printf("\nTamanho: %d", dht2->size);
    printf("\nLoad Factor: %.2f", loadFactor2);
    printf("\nColisoes: %d", collisions2);
    printf("\nMaior sequencia de probing: %d", maxProbe2);

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