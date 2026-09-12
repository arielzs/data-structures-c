#include <assert.h>
#include "common.h"
#include "queue.h"

/* Confere o conteudo da fila, do inicio para o fim, contra um vetor esperado */
static int queueEqualsArray(Queue *q, int *expected, int count)
{
    if (q->size != count)
    {
        return 0;
    }

    QueueNode *node = q->start;

    for (int i = 0; i < count; i++)
    {
        if (node == NULL || node->data != expected[i])
        {
            return 0;
        }

        node = node->next;
    }

    return node == NULL;
}

int main()
{
    /* ===== Enqueue / Dequeue / Peek (FIFO) ===== */
    Queue q;
    queueCreate(&q);

    assert(queueIsEmpty(&q) == 1);
    assert(queuePeek(&q) == -1);
    assert(queueDequeue(&q) == -1);

    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);

    assert(queueIsEmpty(&q) == 0);
    assert(q.size == 3);
    assert(queuePeek(&q) == 1); /* o primeiro a entrar */

    int dequeued[] = {1, 2, 3};
    for (int i = 0; i < 3; i++)
    {
        assert(queueDequeue(&q) == dequeued[i]);
    }

    assert(queueIsEmpty(&q) == 1);

    /* ===== Contains / CountOccurrences ===== */
    queueEnqueue(&q, 5);
    queueEnqueue(&q, 3);
    queueEnqueue(&q, 5);
    queueEnqueue(&q, 8);
    queueEnqueue(&q, 5);

    assert(queueContains(&q, 8) == 4); /* posicao 1-indexada */
    assert(queueContains(&q, 999) == -1);
    assert(queueCountOccurrences(&q, 5) == 3);
    assert(queueCountOccurrences(&q, 999) == 0);

    /* ===== RemoveOccurrences ===== */
    int removedCount = queueRemoveOccurrences(&q, 5);
    assert(removedCount == 3);

    int afterRemoveOccurrences[] = {3, 8};
    assert(queueEqualsArray(&q, afterRemoveOccurrences, 2) == 1);

    queueClear(&q);

    /* ===== RemoveDuplicates (mantem a primeira ocorrencia de cada valor) ===== */
    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);
    queueEnqueue(&q, 1);

    queueRemoveDuplicates(&q);

    int afterRemoveDuplicates[] = {1, 2, 3};
    assert(queueEqualsArray(&q, afterRemoveDuplicates, 3) == 1);

    queueClear(&q);

    /* ===== Copy / Compare ===== */
    Queue a, b;
    queueCreate(&a);
    queueCreate(&b);

    queueEnqueue(&a, 1);
    queueEnqueue(&a, 2);
    queueEnqueue(&a, 3);

    queueCopy(&b, &a);
    assert(queueCompare(&a, &b) == 1);

    queueEnqueue(&b, 999);
    assert(queueCompare(&a, &b) == 0);

    /* Copiar uma fila nela mesma nao faz nada (guarda dest == src) */
    queueCopy(&a, &a);
    assert(a.size == 3);

    queueClear(&a);
    queueClear(&b);

    /* ===== Reverse ===== */
    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);

    queueReverse(&q);

    int afterReverse[] = {3, 2, 1};
    assert(queueEqualsArray(&q, afterReverse, 3) == 1);

    queueClear(&q);

    /* ===== FindMiddle ===== */
    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);
    queueEnqueue(&q, 4);
    queueEnqueue(&q, 5);

    assert(queueFindMiddle(&q) == 3); /* 5 elementos: indice 2 (0-indexado) */

    queueEnqueue(&q, 6); /* agora 6 elementos: indice 3 (0-indexado) */
    assert(queueFindMiddle(&q) == 4);

    queueClear(&q);

    Queue emptyQueue;
    queueCreate(&emptyQueue);
    assert(queueFindMiddle(&emptyQueue) == -1);

    /* ===== InsertSorted mantem a ordem a cada insercao ===== */
    queueCreate(&q);
    queueInsertSorted(&q, 5);
    queueInsertSorted(&q, 1);
    queueInsertSorted(&q, 3);
    queueInsertSorted(&q, 4);
    queueInsertSorted(&q, 2);

    int afterInsertSorted[] = {1, 2, 3, 4, 5};
    assert(queueEqualsArray(&q, afterInsertSorted, 5) == 1);

    queueClear(&q);

    /* ===== Sort ===== */
    queueCreate(&q);
    queueEnqueue(&q, 5);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 4);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);

    queueSort(&q);
    assert(queueEqualsArray(&q, afterInsertSorted, 5) == 1);

    queueClear(&q);

    /* ===== MergeCopy: src continua intacta ===== */
    Queue dest, src;
    queueCreate(&dest);
    queueCreate(&src);

    queueEnqueue(&dest, 1);
    queueEnqueue(&dest, 2);
    queueEnqueue(&src, 10);
    queueEnqueue(&src, 20);

    queueMergeCopy(&dest, &src);

    int afterMergeCopy[] = {1, 2, 10, 20};
    assert(queueEqualsArray(&dest, afterMergeCopy, 4) == 1);

    int srcIntact[] = {10, 20};
    assert(queueEqualsArray(&src, srcIntact, 2) == 1);

    queueClear(&dest);
    queueClear(&src);

    /* ===== MergeMove: src fica vazia depois ===== */
    queueCreate(&dest);
    queueCreate(&src);

    queueEnqueue(&dest, 1);
    queueEnqueue(&dest, 2);
    queueEnqueue(&src, 10);
    queueEnqueue(&src, 20);

    queueMergeMove(&dest, &src);

    assert(queueEqualsArray(&dest, afterMergeCopy, 4) == 1); /* mesmo resultado que o copy */
    assert(queueIsEmpty(&src) == 1);

    queueClear(&dest);

    /* Merge com dest == src nao faz nada (evita self-merge) */
    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueMergeCopy(&q, &q);
    assert(q.size == 1);
    queueClear(&q);

    /* ===== Interleave ===== */
    queueCreate(&dest);
    queueCreate(&src);

    queueEnqueue(&dest, 1);
    queueEnqueue(&dest, 2);
    queueEnqueue(&dest, 3);
    queueEnqueue(&src, 10);
    queueEnqueue(&src, 20);

    queueInterleave(&dest, &src);

    int afterInterleave[] = {1, 10, 2, 20, 3};
    assert(queueEqualsArray(&dest, afterInterleave, 5) == 1);

    queueClear(&dest);
    queueClear(&src);

    /* ===== Rotate: move o primeiro elemento para o fim ===== */
    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);

    queueRotate(&q);

    int afterRotate[] = {2, 3, 1};
    assert(queueEqualsArray(&q, afterRotate, 3) == 1);

    queueClear(&q);

    /* ===== RotateK ===== */
    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);
    queueEnqueue(&q, 4);

    queueRotateK(&q, 2);

    int afterRotateK[] = {3, 4, 1, 2};
    assert(queueEqualsArray(&q, afterRotateK, 4) == 1);

    /* k maior que o tamanho: usa modulo, k=6 em fila de 4 equivale a k=2 */
    queueClear(&q);
    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);
    queueEnqueue(&q, 4);

    queueRotateK(&q, 6);
    assert(queueEqualsArray(&q, afterRotateK, 4) == 1);

    queueClear(&q);

    /* ===== MergeSort ===== */
    queueCreate(&q);
    queueEnqueue(&q, 5);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 4);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);

    queueMergeSort(&q);
    assert(queueEqualsArray(&q, afterInsertSorted, 5) == 1);

    queueClear(&q);

    /* ===== IsSorted ===== */
    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 3);
    assert(queueIsSorted(&q) == 1); /* crescente */
    queueClear(&q);

    queueCreate(&q);
    queueEnqueue(&q, 3);
    queueEnqueue(&q, 2);
    queueEnqueue(&q, 1);
    assert(queueIsSorted(&q) == 2); /* decrescente */
    queueClear(&q);

    queueCreate(&q);
    queueEnqueue(&q, 1);
    queueEnqueue(&q, 3);
    queueEnqueue(&q, 2);
    assert(queueIsSorted(&q) == 0); /* nao ordenada */
    queueClear(&q);

    queueCreate(&q);
    assert(queueIsSorted(&q) == -1); /* vazia */

    queueCreate(&q);
    queueEnqueue(&q, 42);
    assert(queueIsSorted(&q) == 1); /* 1 elemento conta como ordenada */
    queueClear(&q);

    return 0;
}