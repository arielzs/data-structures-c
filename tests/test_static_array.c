#include <assert.h>
#include "common.h"
#include "array.h"

int main()
{
    StaticArray array;
    staticArrayCreate(&array);

    assert(staticArrayIsEmpty(&array) == 1);
    assert(staticArrayIsFull(&array) == 0);

    /* ===== Enche ate a capacidade maxima ===== */
    for (int i = 0; i < STATIC_ARRAY_CAPACITY; i++)
    {
        assert(staticArrayPush(&array, i) == 1);
    }

    assert(staticArraySize(&array) == STATIC_ARRAY_CAPACITY);
    assert(staticArrayIsFull(&array) == 1);

    /* ===== Cheio: push e insert devem recusar e retornar 0 ===== */
    assert(staticArrayPush(&array, 999) == 0);
    assert(staticArraySize(&array) == STATIC_ARRAY_CAPACITY);

    assert(staticArrayInsert(&array, 0, 999) == 0);
    assert(staticArraySize(&array) == STATIC_ARRAY_CAPACITY);

    /* ===== Insert em posicao invalida tambem recusa ===== */
    assert(staticArrayInsert(&array, -1, 999) == 0);
    assert(staticArrayInsert(&array, 99999, 999) == 0);

    /* ===== Libera espaco removendo, agora deve caber de novo ===== */
    int removed = staticArrayRemoveAt(&array, 0);
    assert(removed == 0);
    assert(staticArrayIsFull(&array) == 0);
    assert(staticArrayPush(&array, 999) == 1);
    assert(staticArrayContains(&array, 999) != -1);

    /* ===== Contains / CountOccurrences / RemoveOccurrences / RemoveDuplicates ===== */
    StaticArray small;
    staticArrayCreate(&small);
    staticArrayPush(&small, 5);
    staticArrayPush(&small, 3);
    staticArrayPush(&small, 5);
    staticArrayPush(&small, 8);

    assert(staticArrayCountOccurrences(&small, 5) == 2);
    staticArrayRemoveOccurrences(&small, 5);
    assert(staticArrayCountOccurrences(&small, 5) == 0);
    assert(staticArraySize(&small) == 2);

    staticArrayClear(&small);
    staticArrayPush(&small, 1);
    staticArrayPush(&small, 1);
    staticArrayPush(&small, 2);
    staticArrayRemoveDuplicates(&small);
    assert(staticArraySize(&small) == 2);

    /* ===== Reverse ===== */
    staticArrayClear(&small);
    staticArrayPush(&small, 1);
    staticArrayPush(&small, 2);
    staticArrayPush(&small, 3);
    staticArrayReverse(&small);
    assert(staticArrayGet(&small, 0) == 3);
    assert(staticArrayGet(&small, 2) == 1);

    /* ===== Copy / Compare ===== */
    StaticArray copy;
    staticArrayCreate(&copy);
    staticArrayCopy(&copy, &small);
    assert(staticArrayCompare(&small, &copy) == 1);

    staticArraySet(&copy, 0, 999);
    assert(staticArrayCompare(&small, &copy) == 0);

    return 0;
}