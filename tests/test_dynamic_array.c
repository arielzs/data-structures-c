#include <assert.h>
#include "common.h"
#include "array.h"

int main()
{
    DynamicArray array;
    dynamicArrayCreate(&array);

    assert(dynamicArrayIsEmpty(&array) == 1);
    assert(dynamicArraySize(&array) == 0);

    /* ===== Push e crescimento automatico alem da capacidade inicial ===== */
    for (int i = 0; i < 15; i++)
    {
        assert(dynamicArrayPush(&array, i) == 1);
    }

    assert(dynamicArraySize(&array) == 15);
    assert(dynamicArrayIsEmpty(&array) == 0);
    assert(dynamicArrayGet(&array, 0) == 0);
    assert(dynamicArrayGet(&array, 14) == 14);

    /* ===== Get / Set fora dos limites ===== */
    assert(dynamicArrayGet(&array, -1) == -1);
    assert(dynamicArrayGet(&array, 999) == -1);
    dynamicArraySet(&array, 0, 100);
    assert(dynamicArrayGet(&array, 0) == 100);

    /* ===== Insert em posicao invalida nao insere e retorna 0 ===== */
    int sizeBefore = dynamicArraySize(&array);
    assert(dynamicArrayInsert(&array, -1, 999) == 0);
    assert(dynamicArrayInsert(&array, 999, 999) == 0);
    assert(dynamicArraySize(&array) == sizeBefore);

    /* ===== Insert em posicao valida ===== */
    assert(dynamicArrayInsert(&array, 0, 999) == 1);
    assert(dynamicArrayGet(&array, 0) == 999);
    assert(dynamicArraySize(&array) == sizeBefore + 1);

    /* ===== Contains / CountOccurrences ===== */
    dynamicArrayClear(&array);
    dynamicArrayCreate(&array);

    dynamicArrayPush(&array, 5);
    dynamicArrayPush(&array, 3);
    dynamicArrayPush(&array, 5);
    dynamicArrayPush(&array, 8);
    dynamicArrayPush(&array, 5);

    assert(dynamicArrayContains(&array, 8) == 3);
    assert(dynamicArrayContains(&array, 999) == -1);
    assert(dynamicArrayCountOccurrences(&array, 5) == 3);
    assert(dynamicArrayCountOccurrences(&array, 999) == 0);

    /* ===== RemoveAt ===== */
    int removed = dynamicArrayRemoveAt(&array, 1); /* remove o 3 */
    assert(removed == 3);
    assert(dynamicArrayContains(&array, 3) == -1);
    assert(dynamicArrayRemoveAt(&array, 999) == -1);

    /* ===== RemoveOccurrences ===== */
    dynamicArrayRemoveOccurrences(&array, 5);
    assert(dynamicArrayContains(&array, 5) == -1);
    assert(dynamicArraySize(&array) == 1); /* so sobrou o 8 */

    /* ===== RemoveDuplicates ===== */
    dynamicArrayClear(&array);
    dynamicArrayCreate(&array);
    dynamicArrayPush(&array, 1);
    dynamicArrayPush(&array, 2);
    dynamicArrayPush(&array, 2);
    dynamicArrayPush(&array, 3);
    dynamicArrayPush(&array, 1);

    dynamicArrayRemoveDuplicates(&array);
    assert(dynamicArraySize(&array) == 3);

    /* ===== Reverse ===== */
    dynamicArrayClear(&array);
    dynamicArrayCreate(&array);
    dynamicArrayPush(&array, 1);
    dynamicArrayPush(&array, 2);
    dynamicArrayPush(&array, 3);

    dynamicArrayReverse(&array);
    assert(dynamicArrayGet(&array, 0) == 3);
    assert(dynamicArrayGet(&array, 2) == 1);

    /* ===== Copy / Compare ===== */
    DynamicArray copy;
    dynamicArrayCreate(&copy);
    dynamicArrayCopy(&copy, &array);

    assert(dynamicArrayCompare(&array, &copy) == 1);

    dynamicArrayPush(&copy, 999);
    assert(dynamicArrayCompare(&array, &copy) == 0);

    dynamicArrayClear(&copy);
    dynamicArrayClear(&array);

    return 0;
}