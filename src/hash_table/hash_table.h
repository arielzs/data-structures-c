#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/* Ponto de entrada do submenu de hash tables, chamado pelo menu.c */
void runHashTableMenu(void);

typedef struct HashNode
{
    int key;
    struct HashNode *next;
} HashNode;

typedef struct
{
    int size;
    int count;
    HashNode **table;
} HashTable;

HashTable *hashTableCreate(int size);

/* Esvazia os elementos mas mantem a tabela viva (pode inserir de novo depois) */
void hashTableClearElements(HashTable *ht);

/* Libera tudo, incluindo a struct HashTable. O ponteiro fica invalido depois */
void hashTableDestroy(HashTable *ht);

void hashTablePrint(HashTable *ht);
void hashTableInsert(HashTable *ht, int key);
void hashTableInsertNoResize(HashTable *ht, int key);
int hashTableRemove(HashTable *ht, int key);
HashNode *hashTableSearch(HashTable *ht, int key);

/* Rehash rapido: reaproveita os mesmos nodes, so recria o vetor de baldes */
int hashTableRehash(HashTable *ht, int newSize);

/* Rehash "do zero": cria uma tabela nova (com nodes novos) */
HashTable *hashTableRehashRealloc(HashTable *ht, int newSize);

void hashTableCompare(HashTable *ht1, HashTable *ht2);

#endif