#include <assert.h>
#include "common.h"
#include "stack.h"

/* Confere o conteudo da pilha, do topo para o fundo, contra um vetor esperado */
static int stackEqualsArray(Stack *s, int *expected, int count)
{
    if (s->size != count)
    {
        return 0;
    }

    StackNode *node = s->top;

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
    /* ===== Push / Pop / IsEmpty basicos ===== */
    Stack s;
    stackCreate(&s);

    assert(stackIsEmpty(&s) == 1);

    stackPush(&s, 5);
    stackPush(&s, 3);
    stackPush(&s, 8);

    assert(stackIsEmpty(&s) == 0);
    assert(s.size == 3);

    int afterPush[] = {8, 3, 5};
    assert(stackEqualsArray(&s, afterPush, 3) == 1);

    stackPop(&s);

    int afterPop[] = {3, 5};
    assert(stackEqualsArray(&s, afterPop, 2) == 1);

    stackClear(&s);
    assert(stackIsEmpty(&s) == 1);

    /* ===== PopMultiple ===== */
    stackCreate(&s);
    stackPush(&s, 1);
    stackPush(&s, 2);
    stackPush(&s, 3);
    stackPush(&s, 4);
    stackPush(&s, 5);

    stackPopMultiple(&s, 3);
    assert(s.size == 2);

    stackPopMultiple(&s, 999); /* mais que o tamanho: deve parar quando esvaziar */
    assert(stackIsEmpty(&s) == 1);

    stackClear(&s);

    /* ===== CountOccurrences / RemoveOccurrences ===== */
    stackCreate(&s);
    stackPush(&s, 5);
    stackPush(&s, 3);
    stackPush(&s, 5);
    stackPush(&s, 8);
    stackPush(&s, 5);

    assert(stackCountOccurrences(&s, 5) == 3);
    assert(stackCountOccurrences(&s, 999) == 0);

    stackRemoveOccurrences(&s, 5);
    assert(stackCountOccurrences(&s, 5) == 0);
    assert(s.size == 2);

    stackClear(&s);

    /* ===== Reverse ===== */
    stackCreate(&s);
    stackPush(&s, 5);
    stackPush(&s, 3);
    stackPush(&s, 8);

    int reverseOk = stackReverse(&s);
    assert(reverseOk == 1);

    int afterReverse[] = {5, 3, 8};
    assert(stackEqualsArray(&s, afterReverse, 3) == 1);

    Stack emptyStack;
    stackCreate(&emptyStack);
    assert(stackReverse(&emptyStack) == 0);

    stackClear(&s);

    /* ===== Copy / Compare ===== */
    Stack a, b;
    stackCreate(&a);
    stackCreate(&b);

    stackPush(&a, 1);
    stackPush(&a, 2);
    stackPush(&a, 3);

    stackCopy(&b, &a);
    assert(stackCompare(&a, &b) == 1);

    stackPush(&b, 999);
    assert(stackCompare(&a, &b) == 0);

    stackClear(&a);
    stackClear(&b);

    /* ===== MergeTop: o topo do src fica por cima de tudo ===== */
    Stack dest, src;
    stackCreate(&dest);
    stackCreate(&src);

    stackPush(&dest, 1);
    stackPush(&dest, 2);
    stackPush(&dest, 3);

    stackPush(&src, 7);
    stackPush(&src, 8);

    stackMergeTop(&dest, &src);

    int afterMergeTop[] = {8, 7, 3, 2, 1};
    assert(stackEqualsArray(&dest, afterMergeTop, 5) == 1);

    /* src continua intacta */
    int srcIntact[] = {8, 7};
    assert(stackEqualsArray(&src, srcIntact, 2) == 1);

    stackClear(&dest);
    stackClear(&src);

    /* ===== MergeBottom: o dest continua em cima, src vai pro fundo ===== */
    stackCreate(&dest);
    stackCreate(&src);

    stackPush(&dest, 1);
    stackPush(&dest, 2);
    stackPush(&dest, 3);

    stackPush(&src, 7);
    stackPush(&src, 8);

    stackMergeBottom(&dest, &src);

    int afterMergeBottom[] = {3, 2, 1, 8, 7};
    assert(stackEqualsArray(&dest, afterMergeBottom, 5) == 1);
    assert(stackEqualsArray(&src, srcIntact, 2) == 1);

    stackClear(&dest);
    stackClear(&src);

    /* ===== Sort (insertion sort com pilha auxiliar) ===== */
    stackCreate(&s);
    stackPush(&s, 5);
    stackPush(&s, 3);
    stackPush(&s, 8);
    stackPush(&s, 1);

    stackSort(&s);

    int afterSort[] = {8, 5, 3, 1};
    assert(stackEqualsArray(&s, afterSort, 4) == 1);

    stackClear(&s);

    /* ===== Rotate: move o topo pro fundo ===== */
    stackCreate(&s);
    stackPush(&s, 1);
    stackPush(&s, 2);
    stackPush(&s, 3);
    stackPush(&s, 4);

    stackRotate(&s);

    int afterRotate[] = {3, 2, 1, 4};
    assert(stackEqualsArray(&s, afterRotate, 4) == 1);

    stackClear(&s);

    /* ===== RotateK ===== */
    stackCreate(&s);
    stackPush(&s, 1);
    stackPush(&s, 2);
    stackPush(&s, 3);
    stackPush(&s, 4);
    stackPush(&s, 5);

    stackRotateK(&s, 2);

    int afterRotateK[] = {3, 2, 1, 5, 4};
    assert(stackEqualsArray(&s, afterRotateK, 5) == 1);

    stackClear(&s);

    /* ===== SwapTop: troca os dois primeiros elementos ===== */
    stackCreate(&s);
    stackPush(&s, 1);
    stackPush(&s, 2);
    stackPush(&s, 3);

    stackSwapTop(&s);

    int afterSwapTop[] = {2, 3, 1};
    assert(stackEqualsArray(&s, afterSwapTop, 3) == 1);

    stackClear(&s);

    /* ===== MoveTopDown: reposiciona o topo K posicoes abaixo ===== */
    stackCreate(&s);
    stackPush(&s, 1);
    stackPush(&s, 2);
    stackPush(&s, 3);
    stackPush(&s, 4);

    stackMoveTopDown(&s, 2);

    int afterMoveTopDown[] = {3, 2, 4, 1};
    assert(stackEqualsArray(&s, afterMoveTopDown, 4) == 1);

    stackClear(&s);

    /* ===== MergeSort ===== */
    stackCreate(&s);
    stackPush(&s, 5);
    stackPush(&s, 1);
    stackPush(&s, 4);
    stackPush(&s, 2);
    stackPush(&s, 3);

    stackMergeSort(&s);

    int afterMergeSort[] = {1, 2, 3, 4, 5};
    assert(stackEqualsArray(&s, afterMergeSort, 5) == 1);

    stackClear(&s);

    /* ===== RemoveMiddle ===== */
    stackCreate(&s);
    stackPush(&s, 1);
    stackPush(&s, 2);
    stackPush(&s, 3);
    stackPush(&s, 4);
    stackPush(&s, 5);

    stackRemoveMiddle(&s); /* remove o elemento do meio (posicao 3 a partir do topo) */

    int afterRemoveMiddle[] = {5, 4, 2, 1};
    assert(stackEqualsArray(&s, afterRemoveMiddle, 4) == 1);

    stackClear(&s);

    /* RemoveMiddle com 1 elemento remove o unico elemento */
    stackCreate(&s);
    stackPush(&s, 42);
    stackRemoveMiddle(&s);
    assert(stackIsEmpty(&s) == 1);

    /* RemoveMiddle com 2 elementos remove o segundo (o "do meio" entre os dois) */
    stackCreate(&s);
    stackPush(&s, 1);
    stackPush(&s, 2);
    stackRemoveMiddle(&s);
    assert(s.size == 1);

    stackClear(&s);

    /* ===== VerifyBalance ===== */
    assert(stackVerifyBalance("(a[b]{c})") == 1);
    assert(stackVerifyBalance("(a[b)") == 0);
    assert(stackVerifyBalance("") == 1);
    assert(stackVerifyBalance("(") == 0);
    assert(stackVerifyBalance(")") == 0);
    assert(stackVerifyBalance("sem parenteses") == 1);

    return 0;
}