#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/* Ponto de entrada do submenu de hash tables, chamado pelo menu.c */
void runHashTableMenu(void);

/* ===================== Hash Table com encadeamento ===================== */

typedef struct ChainedHashNode
{
    int key;
    struct ChainedHashNode *next;
} ChainedHashNode;

typedef struct
{
    int size;
    int count;
    ChainedHashNode **table;
} ChainedHashTable;

ChainedHashTable *chainedHashTableCreate(int size);

/* Esvazia os elementos mas mantem a tabela viva */
void chainedHashTableClearElements(ChainedHashTable *ht);

/* Libera tudo, incluindo a struct */
void chainedHashTableDestroy(ChainedHashTable *ht);

void chainedHashTablePrint(ChainedHashTable *ht);
void chainedHashTableInsert(ChainedHashTable *ht, int key);
void chainedHashTableInsertNoResize(ChainedHashTable *ht, int key);
int chainedHashTableRemove(ChainedHashTable *ht, int key);
ChainedHashNode *chainedHashTableSearch(ChainedHashTable *ht, int key);

/* Reaproveita os nodes existentes, so recria o vetor de baldes */
int chainedHashTableRehash(ChainedHashTable *ht, int newSize);

/* Cria uma tabela nova, copia as keys, libera a antiga */
ChainedHashTable *chainedHashTableRehashRealloc(ChainedHashTable *ht, int newSize);

void chainedHashTableCompare(ChainedHashTable *ht1, ChainedHashTable *ht2);

/* ===================== Hash Table com enderecamento aberto (hashing duplo) ===================== */

typedef enum
{
    SLOT_EMPTY,
    SLOT_OCCUPIED,
    SLOT_DELETED
} SlotStatus;

typedef struct
{
    int key;
    SlotStatus status;
} DoubleHashSlot;

typedef struct
{
    int size;
    int count;
    DoubleHashSlot *table;
} DoubleHashTable;

DoubleHashTable *doubleHashTableCreate(int size);
void doubleHashTableClearElements(DoubleHashTable *dht);
void doubleHashTableDestroy(DoubleHashTable *dht);
void doubleHashTablePrint(DoubleHashTable *dht);
void doubleHashTableInsert(DoubleHashTable *dht, int key);


int doubleHashTableInsertNoResize(DoubleHashTable *dht, int key);
int doubleHashTableRemove(DoubleHashTable *dht, int key);
int doubleHashTableSearch(DoubleHashTable *dht, int key);
int doubleHashTableRehash(DoubleHashTable *dht, int newSize);
DoubleHashTable *doubleHashTableRehashRealloc(DoubleHashTable *dht, int newSize);
void doubleHashTableCompare(DoubleHashTable *dht1, DoubleHashTable *dht2);

#endif