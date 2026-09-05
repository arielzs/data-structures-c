#include "common.h"
#include "stack.h"

void stackCreate(Stack *s)
{
    s->top = NULL;
    s->size = 0;
}

void stackClear(Stack *s)
{
    StackNode *temp = s->top;

    while (temp != NULL)
    {
        StackNode *next = temp->next;

        free(temp);

        temp = next;
    }

    s->top = NULL;
    s->size = 0;
}

int stackIsEmpty(Stack *s)
{
    return s->top == NULL;
}

void stackPush(Stack *s, int data)
{
    StackNode *newNode = malloc(sizeof(StackNode));

    if (newNode == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    newNode->data = data;
    newNode->next = s->top;

    s->top = newNode;
    s->size++;
}

void stackPop(Stack *s)
{
    if (stackIsEmpty(s))
    {
        printf("Pilha vazia!\n");
        return;
    }

    StackNode *temp = s->top;

    s->top = temp->next;

    free(temp);

    s->size--;
}

void stackPopMultiple(Stack *s, int quantity)
{
    if (quantity <= 0)
    {
        printf("Quantidade invalida!\n");
        return;
    }

    int removed = 0;

    for (int i = 0; i < quantity && !stackIsEmpty(s); i++)
    {
        stackPop(s);
        removed++;
    }

    printf("%d Elemento(s) removido(s)!\n", removed);
}

void stackPrint(Stack *s)
{
    if (stackIsEmpty(s))
    {
        printf("Pilha vazia!\n");
        return;
    }

    StackNode *temp = s->top;

    printf("\n TOPO\n");
    printf("  |\n");

    while (temp != NULL)
    {
        printf(" [%d]\n", temp->data);
        printf("  |\n");
        temp = temp->next;
    }

    printf(" NULL\n");
    printf("\n--------\n");
    printf("\nTamanho da pilha: %d\n", s->size);
}

void stackSearch(Stack *s, int data)
{
    if (stackIsEmpty(s))
    {
        printf("Pilha Vazia!\n");
        return;
    }

    StackNode *temp = s->top;
    int pos = 0;

    while (temp != NULL && temp->data != data)
    {
        temp = temp->next;
        pos++;
    }

    if (temp == NULL)
    {
        printf("Elemento %d nao encontrado!\n", data);
    }
    else
    {
        printf("Elemento %d encontrado na posicao %d!\n", data, pos + 1);
    }
}

int stackCountOccurrences(Stack *s, int data)
{
    StackNode *temp = s->top;

    int count = 0;

    while (temp != NULL)
    {
        if (temp->data == data)
        {
            count++;
        }

        temp = temp->next;
    }

    return count;
}

void stackRemoveOccurrences(Stack *s, int data)
{
    if (stackIsEmpty(s))
    {
        printf("Pilha Vazia!\n");
        return;
    }

    StackNode *curr = s->top;
    StackNode *prev = NULL;

    int quantity = 0;

    while (curr != NULL)
    {
        if (curr->data == data)
        {
            StackNode *removed = curr;
            curr = curr->next;

            if (prev == NULL)
            {
                s->top = curr;
            }
            else
            {
                prev->next = curr;
            }

            free(removed);
            s->size--;
            quantity++;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }

    if (quantity == 0)
    {
        printf("Elemento %d nao encontrado!\n", data);
    }
    else
    {
        printf("%d Elementos removidos!\n", quantity);
    }
}

int stackReverse(Stack *s)
{
    if (stackIsEmpty(s))
    {
        return 0;
    }

    Stack aux;
    stackCreate(&aux);
    StackNode *temp = s->top;

    while (temp != NULL)
    {
        stackPush(&aux, temp->data);
        temp = temp->next;
    }

    stackClear(s);
    s->top = aux.top;
    s->size = aux.size;

    return 1;
}

void stackCopy(Stack *dest, Stack *src)
{
    if (dest == src)
    {
        return;
    }

    stackClear(dest);

    StackNode *temp = src->top;

    while (temp != NULL)
    {
        stackPush(dest, temp->data);

        temp = temp->next;
    }

    stackReverse(dest);
}

int stackCompare(Stack *s1, Stack *s2)
{
    if (s1->size != s2->size)
    {
        return 0;
    }

    StackNode *temp1 = s1->top;
    StackNode *temp2 = s2->top;

    while (temp1 != NULL)
    {
        if (temp1->data != temp2->data)
        {
            return 0;
        }

        temp1 = temp1->next;
        temp2 = temp2->next;
    }

    return 1;
}

void stackMergeTop(Stack *dest, Stack *src)
{
    if (dest == src)
    {
        return;
    }

    Stack aux;

    stackCreate(&aux);

    StackNode *temp = src->top;

    while (temp != NULL)
    {
        stackPush(&aux, temp->data);
        temp = temp->next;
    }

    temp = aux.top;

    while (temp != NULL)
    {
        stackPush(dest, temp->data);
        temp = temp->next;
    }

    stackClear(&aux);
}

void stackMergeBottom(Stack *dest, Stack *src)
{
    if (dest == src)
    {
        return;
    }

    Stack aux;

    stackCreate(&aux);
    stackCopy(&aux, dest);
    stackReverse(&aux);

    StackNode *temp = src->top;

    while (temp != NULL)
    {
        stackPush(&aux, temp->data);
        temp = temp->next;
    }

    stackReverse(&aux);
    stackCopy(dest, &aux);
    stackClear(&aux);
}

void stackSort(Stack *s)
{
    Stack aux;
    stackCreate(&aux);

    while (!stackIsEmpty(s))
    {
        int temp = s->top->data;
        stackPop(s);

        while (!(stackIsEmpty(&aux)) && aux.top->data > temp)
        {
            stackPush(s, aux.top->data);
            stackPop(&aux);
        }

        stackPush(&aux, temp);
    }

    stackCopy(s, &aux);
}

void stackRotate(Stack *s)
{
    if (s->size < 2)
    {
        return;
    }

    StackNode *oldTop = s->top;
    s->top = oldTop->next;

    StackNode *temp = s->top;

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = oldTop;
    oldTop->next = NULL;
}

void stackRotateK(Stack *s, int k)
{
    if (s->size < 2)
    {
        return;
    }

    k = k % s->size;

    if (k == 0)
    {
        return;
    }

    StackNode *last = s->top;

    while (last->next != NULL)
    {
        last = last->next;
    }

    last->next = s->top;

    StackNode *newTail = s->top;

    for (int i = 0; i < k - 1; i++)
    {
        newTail = newTail->next;
    }

    StackNode *newTop = newTail->next;

    newTail->next = NULL;

    s->top = newTop;
}

void stackSwapTop(Stack *s)
{
    if (s->size < 2)
    {
        return;
    }

    StackNode *oldTop = s->top;
    StackNode *newTop = oldTop->next;

    s->top = newTop;
    oldTop->next = newTop->next;
    newTop->next = oldTop;
}

/* Move o antigo topo k posicoes para baixo na pilha
   (nao insere elemento novo, so reposiciona o topo atual). */
void stackMoveTopDown(Stack *s, int k)
{
    if (s->size < 2 || k <= 0)
    {
        return;
    }

    if (k > s->size - 1)
    {
        k = s->size - 1;
    }

    StackNode *temp = s->top;

    s->top = temp->next;

    StackNode *curr = s->top;

    for (int i = 0; i < k - 1; i++)
    {
        curr = curr->next;
    }

    temp->next = curr->next;
    curr->next = temp;
}

static StackNode *findMiddle(StackNode *top)
{
    if (top == NULL)
    {
        return NULL;
    }

    StackNode *slow = top;
    StackNode *fast = top->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

static StackNode *mergeNodes(StackNode *left, StackNode *right)
{
    if (left == NULL)
    {
        return right;
    }

    if (right == NULL)
    {
        return left;
    }

    StackNode *result = NULL;

    if (left->data <= right->data)
    {
        result = left;
        result->next = mergeNodes(left->next, right);
    }
    else
    {
        result = right;
        result->next = mergeNodes(left, right->next);
    }

    return result;
}

static StackNode *mergeSortNodes(StackNode *top)
{
    if (top == NULL || top->next == NULL)
    {
        return top;
    }

    StackNode *mid = findMiddle(top);
    StackNode *right = mid->next;
    mid->next = NULL;

    StackNode *leftSorted = mergeSortNodes(top);
    StackNode *rightSorted = mergeSortNodes(right);

    return mergeNodes(leftSorted, rightSorted);
}

void stackMergeSort(Stack *s)
{
    if (s->top == NULL || s->top->next == NULL)
    {
        return;
    }

    s->top = mergeSortNodes(s->top);

    StackNode *temp = s->top;
    s->size = 0;

    while (temp != NULL)
    {
        temp = temp->next;
        s->size++;
    }
}

void stackRemoveMiddle(Stack *s)
{
    if (stackIsEmpty(s))
    {
        return;
    }

    if (s->size == 1)
    {
        stackPop(s);
        return;
    }

    if (s->size == 2)
    {
        StackNode *mid = s->top->next;

        s->top->next = NULL;

        free(mid);

        s->size--;

        return;
    }

    StackNode *slow = s->top;
    StackNode *fast = s->top->next;
    StackNode *prev = NULL;

    while (fast != NULL && fast->next != NULL)
    {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    StackNode *mid = slow;

    prev->next = mid->next;
    mid->next = NULL;

    free(mid);
    s->size--;
}

static int match(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

static int isOpening(char c)
{
    return c == '(' || c == '[' || c == '{';
}

static int isClosing(char c)
{
    return c == ')' || c == ']' || c == '}';
}

int stackVerifyBalance(char expr[])
{
    Stack aux;
    stackCreate(&aux);

    for (int i = 0; expr[i] != '\0'; i++)
    {
        char c = expr[i];

        if (isOpening(c))
        {
            stackPush(&aux, c);
        }
        else if (isClosing(c))
        {
            if (stackIsEmpty(&aux))
            {
                stackClear(&aux);
                return 0;
            }

            char top = aux.top->data;

            if (!match(top, c))
            {
                stackClear(&aux);
                return 0;
            }

            stackPop(&aux);
        }
    }

    int ok = stackIsEmpty(&aux);
    stackClear(&aux);

    return ok;
}