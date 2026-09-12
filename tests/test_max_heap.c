#include <assert.h>
#include "common.h"
#include "heap.h"

static int isMaxHeapProperty(Heap *h)
{
    for (int i = 0; i < h->size; i++)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < h->size && h->array[i] < h->array[left])
        {
            return 0;
        }

        if (right < h->size && h->array[i] < h->array[right])
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    /* ===== Create ===== */
    Heap *invalid = maxHeapCreate(0);
    assert(invalid == NULL);

    Heap *h = maxHeapCreate(4);
    assert(h != NULL);
    assert(h->size == 0);
    assert(h->capacity == 4);
    assert(h->isHeap == 1);
    assert(h->type == HEAP_MAX);

    /* ===== Insert mantem a propriedade de max-heap, inclusive apos crescer
       sozinho (capacidade inicial e so 4, vamos inserir 10) ===== */
    int values[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};

    for (int i = 0; i < 10; i++)
    {
        maxHeapInsert(h, values[i]);
        assert(isMaxHeapProperty(h) == 1);
    }

    assert(h->size == 10);
    assert(h->capacity >= 10);
    assert(h->array[0] == 9); /* maior valor inserido */

    /* ===== Extract sempre tira o maior, na ordem certa ===== */
    int expectedDescending[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    for (int i = 0; i < 10; i++)
    {
        int extracted;
        int ok = maxHeapExtract(h, &extracted);

        assert(ok == 1);
        assert(extracted == expectedDescending[i]);
        assert(isMaxHeapProperty(h) == 1);
    }

    assert(h->size == 0);

    /* ===== Extract com argumentos invalidos falha sem quebrar ===== */
    int dummy;
    assert(maxHeapExtract(h, &dummy) == 0); /* heap vazia */
    assert(maxHeapExtract(NULL, &dummy) == 0);
    assert(maxHeapExtract(h, NULL) == 0);

    maxHeapDestroy(h);

    /* ===== Update: subir e descer o valor reorganiza o heap ===== */
    h = maxHeapCreate(10);
    maxHeapInsert(h, 5);
    maxHeapInsert(h, 3);
    maxHeapInsert(h, 8);
    maxHeapInsert(h, 1);

    assert(h->array[0] == 8);

    int indexOfOne = -1;

    for (int i = 0; i < h->size; i++)
    {
        if (h->array[i] == 1)
        {
            indexOfOne = i;
            break;
        }
    }

    assert(indexOfOne != -1);

    /* Aumentar um valor pequeno o suficiente para virar o novo maior */
    int updateOk = maxHeapUpdate(h, indexOfOne, 100);
    assert(updateOk == 1);
    assert(h->array[0] == 100);
    assert(isMaxHeapProperty(h) == 1);

    /* Diminuir o valor que acabou de virar raiz deve fazer ele afundar de novo */
    updateOk = maxHeapUpdate(h, 0, -50);
    assert(updateOk == 1);
    assert(isMaxHeapProperty(h) == 1);
    assert(h->array[0] != -50);

    /* Update em index invalido falha */
    assert(maxHeapUpdate(h, -1, 10) == 0);
    assert(maxHeapUpdate(h, 999, 10) == 0);

    maxHeapDestroy(h);

    /* ===== Insert e Update recusam quando o vetor nao esta em estado de heap ===== */
    h = maxHeapCreate(5);
    maxHeapInsert(h, 1);
    maxHeapInsert(h, 2);
    maxHeapInsert(h, 3);

    maxHeapSort(h); /* deixa o vetor ordenado, mas isHeap vira 0 */
    assert(h->isHeap == 0);

    int sizeBefore = h->size;
    maxHeapInsert(h, 999); /* deve recusar, pois isHeap == 0 */
    assert(h->size == sizeBefore);

    assert(maxHeapUpdate(h, 0, 999) == 0);

    maxHeapDestroy(h);

    /* ===== Build: transforma um vetor qualquer em max-heap valido ===== */
    h = maxHeapCreate(5);
    h->array[0] = 1;
    h->array[1] = 5;
    h->array[2] = 3;
    h->array[3] = 2;
    h->array[4] = 4;
    h->size = 5;
    h->isHeap = 0;

    maxHeapBuild(h);

    assert(h->isHeap == 1);
    assert(isMaxHeapProperty(h) == 1);
    assert(h->array[0] == 5); /* o maior valor do vetor original */

    maxHeapDestroy(h);

    /* ===== Sort: deixa em ordem crescente e marca isHeap = 0 ===== */
    h = maxHeapCreate(5);
    maxHeapInsert(h, 40);
    maxHeapInsert(h, 10);
    maxHeapInsert(h, 30);
    maxHeapInsert(h, 20);
    maxHeapInsert(h, 5);

    maxHeapSort(h);

    int expectedSorted[] = {5, 10, 20, 30, 40};

    for (int i = 0; i < 5; i++)
    {
        assert(h->array[i] == expectedSorted[i]);
    }

    assert(h->size == 5); /* tamanho preservado apos o sort */
    assert(h->isHeap == 0);

    maxHeapDestroy(h);

    /* ===== Resize ===== */
    h = maxHeapCreate(4);
    maxHeapInsert(h, 1);
    maxHeapInsert(h, 2);

    assert(maxHeapResize(h, 10) == 1);
    assert(h->capacity == 10);
    assert(h->array[0] >= 1); /* dados preservados */

    assert(maxHeapResize(h, 0) == 0); /* capacidade invalida */
    assert(maxHeapResize(h, 1) == 0); /* menor que o size atual (2) */
    assert(maxHeapResize(NULL, 10) == 0);

    maxHeapDestroy(h);

    return 0;
}