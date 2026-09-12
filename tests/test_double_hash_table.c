#include <assert.h>
#include "common.h"
#include "hash_table.h"

int main()
{
    /* ===== Create ajusta o tamanho para o proximo numero primo ===== */
    DoubleHashTable *dht = doubleHashTableCreate(4);
    assert(dht != NULL);
    assert(dht->size == 5); /* proximo primo >= 4 */
    assert(dht->count == 0);

    doubleHashTableDestroy(dht);

    /* ===== Insert / Search basicos ===== */
    dht = doubleHashTableCreate(5);

    doubleHashTableInsert(dht, 10);
    doubleHashTableInsert(dht, 20);

    assert(dht->count == 2);
    assert(doubleHashTableSearch(dht, 10) != -1);
    assert(doubleHashTableSearch(dht, 20) != -1);
    assert(doubleHashTableSearch(dht, 999) == -1);

    /* ===== Insert de chave duplicada e recusado ===== */
    int countBefore = dht->count;
    doubleHashTableInsert(dht, 10);
    assert(dht->count == countBefore);

    doubleHashTableDestroy(dht);

    /* ===== Colisao real + remocao com tombstone ===== */
    dht = doubleHashTableCreate(5);

    doubleHashTableInsert(dht, 10);
    doubleHashTableInsert(dht, 15); /* 15 % 5 == 10 % 5 == 0: cai na mesma casa */
    doubleHashTableInsert(dht, 3);

    assert(dht->count == 3);
    assert(doubleHashTableSearch(dht, 10) != -1);
    assert(doubleHashTableSearch(dht, 15) != -1);
    assert(doubleHashTableSearch(dht, 3) != -1);

    /* Remove o que colidiu (15): o outro (10) precisa continuar encontravel,
       senao a busca estaria parando cedo demais num slot vazio de verdade
       em vez de num "removido" (tombstone) */
    int removed = doubleHashTableRemove(dht, 15);
    assert(removed == 1);
    assert(doubleHashTableSearch(dht, 15) == -1);
    assert(doubleHashTableSearch(dht, 10) != -1);

    int removedMissing = doubleHashTableRemove(dht, 999);
    assert(removedMissing == 0);

    /* Reinserir 15 deve reaproveitar o slot removido (tombstone) */
    int countBeforeReinsert = dht->count;
    doubleHashTableInsert(dht, 15);
    assert(dht->count == countBeforeReinsert + 1);
    assert(doubleHashTableSearch(dht, 15) != -1);

    doubleHashTableDestroy(dht);

    /* ===== Tabela realmente cheia: InsertNoResize deve recusar ===== */
    dht = doubleHashTableCreate(5); /* 5 ja eh primo, tamanho fica 5 */

    for (int i = 0; i < dht->size; i++)
    {
        assert(doubleHashTableInsertNoResize(dht, i) == 1);
    }

    assert(dht->count == dht->size);

    int insertedWhenFull = doubleHashTableInsertNoResize(dht, 999);
    assert(insertedWhenFull == 0);
    assert(dht->count == dht->size);

    doubleHashTableDestroy(dht);

    /* ===== ClearElements: esvazia mas mantem a tabela viva ===== */
    dht = doubleHashTableCreate(5);
    doubleHashTableInsert(dht, 1);
    doubleHashTableInsert(dht, 2);

    doubleHashTableClearElements(dht);
    assert(dht->count == 0);
    assert(dht->size == 5); /* tamanho preservado */
    assert(doubleHashTableSearch(dht, 1) == -1);

    doubleHashTableInsert(dht, 42);
    assert(doubleHashTableSearch(dht, 42) != -1);

    doubleHashTableDestroy(dht);

    /* ===== Rehash manual rapido (o novo tamanho tambem vira primo) ===== */
    dht = doubleHashTableCreate(5);
    doubleHashTableInsertNoResize(dht, 1);
    doubleHashTableInsertNoResize(dht, 2);
    doubleHashTableInsertNoResize(dht, 3);

    int rehashOk = doubleHashTableRehash(dht, 10);
    assert(rehashOk == 1);
    assert(dht->size == 11); /* proximo primo >= 10 */
    assert(dht->count == 3); /* nao perde elementos no rehash */
    assert(doubleHashTableSearch(dht, 1) != -1);
    assert(doubleHashTableSearch(dht, 2) != -1);
    assert(doubleHashTableSearch(dht, 3) != -1);

    doubleHashTableDestroy(dht);

    /* ===== Rehash "do zero" (recria a struct, retorna ponteiro novo) ===== */
    dht = doubleHashTableCreate(5);
    doubleHashTableInsertNoResize(dht, 1);
    doubleHashTableInsertNoResize(dht, 2);

    DoubleHashTable *newDht = doubleHashTableRehashRealloc(dht, 10);
    assert(newDht != NULL);
    assert(newDht->size == 11);
    assert(newDht->count == 2);
    assert(doubleHashTableSearch(newDht, 1) != -1);
    assert(doubleHashTableSearch(newDht, 2) != -1);

    doubleHashTableDestroy(newDht);

    /* ===== ReHash automatico ao atingir load factor >= 0.75 ===== */
    dht = doubleHashTableCreate(5);
    doubleHashTableInsert(dht, 1);
    doubleHashTableInsert(dht, 2);
    doubleHashTableInsert(dht, 3);

    int sizeBeforeAuto = dht->size;
    doubleHashTableInsert(dht, 4); /* count vira 4, load factor 4/5 = 0.8 -> dispara sozinho */

    assert(dht->size > sizeBeforeAuto);
    assert(dht->count == 4);
    assert(doubleHashTableSearch(dht, 1) != -1);
    assert(doubleHashTableSearch(dht, 2) != -1);
    assert(doubleHashTableSearch(dht, 3) != -1);
    assert(doubleHashTableSearch(dht, 4) != -1);

    doubleHashTableDestroy(dht);

    return 0;
}