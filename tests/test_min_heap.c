#include <assert.h>
#include "common.h"
#include "heap.h"

static int isMinHeapProperty(Heap *h)
{
    for (int i = 0; i < h->size; i++)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < h->size && h->array[i] > h->array[left])
        {
            return 0;
        }

        if (right < h->size && h->array[i] > h->array[right])
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    /* ===== Create ===== */
    Heap *invalid = minHeapCreate(0);
    assert(invalid == NULL);

    Heap *h = minHeapCreate(4);
    assert(h != NULL);
    assert(h->size == 0);
    assert(h->capacity == 4);
    assert(h->isHeap == 1);
    assert(h->type == HEAP_MIN);

    /* ===== Insert mantem a propriedade de min-heap, inclusive apos crescer
       sozinho (capacidade inicial e so 4, vamos inserir 10) ===== */
    int values[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};

    for (int i = 0; i < 10; i++)
    {
        minHeapInsert(h, values[i]);
        assert(isMinHeapProperty(h) == 1);
    }

    assert(h->size == 10);
    assert(h->capacity >= 10);
    assert(h->array[0] == 0); /* menor valor inserido */

    /* ===== Extract sempre tira o menor, na ordem certa ===== */
    int expectedAscending[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < 10; i++)
    {
        int extracted;
        int ok = minHeapExtract(h, &extracted);

        assert(ok == 1);
        assert(extracted == expectedAscending[i]);
        assert(isMinHeapProperty(h) == 1);
    }

    assert(h->size == 0);

    /* ===== Extract com argumentos invalidos falha sem quebrar ===== */
    int dummy;
    assert(minHeapExtract(h, &dummy) == 0); /* heap vazia */
    assert(minHeapExtract(NULL, &dummy) == 0);
    assert(minHeapExtract(h, NULL) == 0);

    minHeapDestroy(h);

    /* ===== Update: descer e subir o valor reorganiza o heap ===== */
    h = minHeapCreate(10);
    minHeapInsert(h, 5);
    minHeapInsert(h, 8);
    minHeapInsert(h, 3);
    minHeapInsert(h, 9);

    assert(h->array[0] == 3);

    int indexOfNine = -1;

    for (int i = 0; i < h->size; i++)
    {
        if (h->array[i] == 9)
        {
            indexOfNine = i;
            break;
        }
    }

    assert(indexOfNine != -1);

    /* Diminuir um valor grande o suficiente para virar o novo menor */
    int updateOk = minHeapUpdate(h, indexOfNine, -100);
    assert(updateOk == 1);
    assert(h->array[0] == -100);
    assert(isMinHeapProperty(h) == 1);

    /* Aumentar o valor que acabou de virar raiz deve fazer ele afundar de novo */
    updateOk = minHeapUpdate(h, 0, 500);
    assert(updateOk == 1);
    assert(isMinHeapProperty(h) == 1);
    assert(h->array[0] != 500);

    /* Update em index invalido falha */
    assert(minHeapUpdate(h, -1, 10) == 0);
    assert(minHeapUpdate(h, 999, 10) == 0);

    minHeapDestroy(h);

    /* ===== Insert e Update recusam quando o vetor nao esta em estado de heap ===== */
    h = minHeapCreate(5);
    minHeapInsert(h, 1);
    minHeapInsert(h, 2);
    minHeapInsert(h, 3);

    minHeapSort(h); /* deixa o vetor ordenado, mas isHeap vira 0 */
    assert(h->isHeap == 0);

    int sizeBefore = h->size;
    minHeapInsert(h, 999); /* deve recusar, pois isHeap == 0 */
    assert(h->size == sizeBefore);

    assert(minHeapUpdate(h, 0, 999) == 0);

    minHeapDestroy(h);

    /* ===== Build: transforma um vetor qualquer em min-heap valido ===== */
    h = minHeapCreate(5);
    h->array[0] = 5;
    h->array[1] = 1;
    h->array[2] = 3;
    h->array[3] = 4;
    h->array[4] = 2;
    h->size = 5;
    h->isHeap = 0;

    minHeapBuild(h);

    assert(h->isHeap == 1);
    assert(isMinHeapProperty(h) == 1);
    assert(h->array[0] == 1); /* o menor valor do vetor original */

    minHeapDestroy(h);

    /* ===== Sort: deixa em ordem DECRESCENTE (espelho do max-heap sort) e marca isHeap = 0 ===== */
    h = minHeapCreate(5);
    minHeapInsert(h, 40);
    minHeapInsert(h, 10);
    minHeapInsert(h, 30);
    minHeapInsert(h, 20);
    minHeapInsert(h, 5);

    minHeapSort(h);

    int expectedSorted[] = {40, 30, 20, 10, 5};

    for (int i = 0; i < 5; i++)
    {
        assert(h->array[i] == expectedSorted[i]);
    }

    assert(h->size == 5); /* tamanho preservado apos o sort */
    assert(h->isHeap == 0);

    minHeapDestroy(h);

    /* ===== Resize ===== */
    h = minHeapCreate(4);
    minHeapInsert(h, 1);
    minHeapInsert(h, 2);

    assert(minHeapResize(h, 10) == 1);
    assert(h->capacity == 10);
    assert(h->array[0] <= 2); /* dados preservados */

    assert(minHeapResize(h, 0) == 0); /* capacidade invalida */
    assert(minHeapResize(h, 1) == 0); /* menor que o size atual (2) */
    assert(minHeapResize(NULL, 10) == 0);

    minHeapDestroy(h);

    return 0;
}