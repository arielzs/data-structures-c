#include <assert.h>
#include "common.h"
#include "hash_table.h"

int main()
{
    /* ===== Create / Insert / Search basicos ===== */
    ChainedHashTable *ht = chainedHashTableCreate(8);
    assert(ht != NULL);
    assert(ht->size == 8);
    assert(ht->count == 0);

    chainedHashTableInsert(ht, 10);
    chainedHashTableInsert(ht, 20);
    chainedHashTableInsert(ht, 30);

    assert(ht->count == 3);
    assert(chainedHashTableSearch(ht, 10) != NULL);
    assert(chainedHashTableSearch(ht, 20) != NULL);
    assert(chainedHashTableSearch(ht, 999) == NULL);

    /* ===== Encadeamento permite chaves duplicadas ===== */
    int countBefore = ht->count;
    chainedHashTableInsert(ht, 10);
    assert(ht->count == countBefore + 1);

    /* ===== Remove ===== */
    int removed = chainedHashTableRemove(ht, 20);
    assert(removed == 1);
    assert(chainedHashTableSearch(ht, 20) == NULL);

    int removedMissing = chainedHashTableRemove(ht, 999);
    assert(removedMissing == 0);

    /* Remove uma das duas keys "10": a outra copia ainda deve ser encontrada */
    chainedHashTableRemove(ht, 10);
    assert(chainedHashTableSearch(ht, 10) != NULL);

    /* ===== ClearElements: esvazia mas mantem a tabela viva ===== */
    chainedHashTableClearElements(ht);
    assert(ht->count == 0);
    assert(ht->size == 8); /* tamanho preservado */
    assert(chainedHashTableSearch(ht, 10) == NULL);

    chainedHashTableInsert(ht, 42);
    assert(chainedHashTableSearch(ht, 42) != NULL);

    chainedHashTableDestroy(ht);

    /* ===== Colisao real: forcar duas keys no mesmo balde ===== */
    ht = chainedHashTableCreate(5);
    chainedHashTableInsert(ht, 3);
    chainedHashTableInsert(ht, 8); /* 8 % 5 == 3 % 5 == 3: cai na mesma casa */

    assert(chainedHashTableSearch(ht, 3) != NULL);
    assert(chainedHashTableSearch(ht, 8) != NULL);
    assert(ht->count == 2);

    chainedHashTableDestroy(ht);

    /* ===== Rehash manual rapido (reaproveita os nodes existentes) ===== */
    ht = chainedHashTableCreate(4);
    chainedHashTableInsert(ht, 1);
    chainedHashTableInsert(ht, 2);
    chainedHashTableInsert(ht, 3);

    int rehashOk = chainedHashTableRehash(ht, 16);
    assert(rehashOk == 1);
    assert(ht->size == 16);
    assert(ht->count == 3); /* nao perde elementos no rehash */
    assert(chainedHashTableSearch(ht, 1) != NULL);
    assert(chainedHashTableSearch(ht, 2) != NULL);
    assert(chainedHashTableSearch(ht, 3) != NULL);

    chainedHashTableDestroy(ht);

    /* ===== Rehash "do zero" (recria a struct, retorna ponteiro novo) ===== */
    ht = chainedHashTableCreate(4);
    chainedHashTableInsert(ht, 1);
    chainedHashTableInsert(ht, 2);

    ChainedHashTable *newHt = chainedHashTableRehashRealloc(ht, 16);
    assert(newHt != NULL);
    assert(newHt->size == 16);
    assert(newHt->count == 2);
    assert(chainedHashTableSearch(newHt, 1) != NULL);
    assert(chainedHashTableSearch(newHt, 2) != NULL);

    chainedHashTableDestroy(newHt);

    /* ===== ReHash automatico ao atingir load factor >= 0.75 ===== */
    ht = chainedHashTableCreate(4);
    chainedHashTableInsert(ht, 1);
    chainedHashTableInsert(ht, 2);

    int sizeBeforeAuto = ht->size;
    chainedHashTableInsert(ht, 3); /* count vira 3, load factor 3/4 = 0.75 -> dispara sozinho */

    assert(ht->size > sizeBeforeAuto);
    assert(ht->count == 3);
    assert(chainedHashTableSearch(ht, 1) != NULL);
    assert(chainedHashTableSearch(ht, 2) != NULL);
    assert(chainedHashTableSearch(ht, 3) != NULL);

    chainedHashTableDestroy(ht);

    return 0;
}