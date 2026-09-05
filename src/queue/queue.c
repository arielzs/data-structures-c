#include "common.h"
#include "queue.h"

void queueCreate(Queue *q)
{
    q->start = NULL;
    q->end = NULL;
    q->size = 0;
}

void queueClear(Queue *q)
{
    QueueNode *temp = q->start;

    while (temp != NULL)
    {
        QueueNode *removed = temp;

        temp = temp->next;

        free(removed);
    }

    q->start = NULL;
    q->end = NULL;
    q->size = 0;
}

int queueIsEmpty(Queue *q)
{
    return q->size == 0;
}

void queuePrint(Queue *q)
{
    if (queueIsEmpty(q))
    {
        printf("INICIO -> NULL <- FIM");
        return;
    }

    QueueNode *temp = q->start;

    printf("\nINICIO -> ");

    while (temp != NULL)
    {
        printf("[%d] -> ", temp->data);
        temp = temp->next;
    }

    printf("NULL\n");
    printf("\nTamanho da fila: %d\n", q->size);
}

void queueEnqueue(Queue *q, int data)
{
    QueueNode *newNode = malloc(sizeof(QueueNode));

    if (newNode == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    newNode->data = data;
    newNode->next = NULL;

    if (queueIsEmpty(q))
    {
        q->start = newNode;
        q->end = newNode;
    }
    else
    {
        q->end->next = newNode;
        q->end = newNode;
    }

    q->size++;
}

int queueDequeue(Queue *q)
{
    if (queueIsEmpty(q))
    {
        printf("Fila Vazia!");
        return -1;
    }

    int data = q->start->data;

    QueueNode *temp = q->start;

    if (q->size == 1)
    {
        q->start = NULL;
        q->end = NULL;
    }
    else
    {
        q->start = q->start->next;
    }

    free(temp);

    q->size--;

    return data;
}

int queuePeek(Queue *q)
{
    if (queueIsEmpty(q))
    {
        return -1;
    }

    return q->start->data;
}

int queueContains(Queue *q, int data)
{
    QueueNode *temp = q->start;

    int pos = 1;

    while (temp != NULL)
    {
        if (temp->data == data)
        {
            return pos;
        }

        temp = temp->next;
        pos++;
    }

    return -1;
}

int queueCountOccurrences(Queue *q, int data)
{
    QueueNode *temp = q->start;

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

int queueRemoveOccurrences(Queue *q, int data)
{
    QueueNode *current = q->start;
    QueueNode *prev = NULL;
    int removeds = 0;

    while (current != NULL)
    {
        if (current->data == data)
        {
            QueueNode *temp = current;

            if (prev == NULL)
            {
                q->start = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            if (current == q->end)
            {
                q->end = prev;
            }

            current = current->next;

            free(temp);

            q->size--;
            removeds++;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }

    return removeds;
}

void queueRemoveDuplicates(Queue *q)
{
    if (queueIsEmpty(q))
    {
        printf("Fila Vazia!\n");
        return;
    }

    QueueNode *current = q->start;

    while (current != NULL)
    {
        QueueNode *runner = current;

        while (runner->next != NULL)
        {
            if (runner->next->data == current->data)
            {
                QueueNode *removed = runner->next;

                if (runner->next == q->end)
                {
                    q->end = runner;
                }

                runner->next = removed->next;

                free(removed);

                q->size--;
            }
            else
            {
                runner = runner->next;
            }
        }

        current = current->next;
    }
}

void queueCopy(Queue *dest, Queue *src)
{
    if (dest == src)
    {
        return;
    }

    queueClear(dest);

    QueueNode *temp = src->start;

    while (temp != NULL)
    {
        queueEnqueue(dest, temp->data);

        temp = temp->next;
    }
}

int queueCompare(Queue *q1, Queue *q2)
{
    if (q1->size != q2->size)
    {
        return 0;
    }

    QueueNode *temp1 = q1->start;
    QueueNode *temp2 = q2->start;

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

void queueReverse(Queue *q)
{
    QueueNode *current = q->start;
    QueueNode *prev = NULL;
    QueueNode *next;

    q->end = q->start;

    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    q->start = prev;
}

int queueFindMiddle(Queue *q)
{
    if (queueIsEmpty(q))
    {
        return -1;
    }

    int middle = q->size / 2;

    QueueNode *temp = q->start;

    for (int i = 0; i < middle; i++)
    {
        temp = temp->next;
    }

    return temp->data;
}

static QueueNode *findMiddleNode(QueueNode *start)
{
    if (start == NULL)
    {
        return NULL;
    }

    QueueNode *slow = start;
    QueueNode *fast = start->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

void queueInsertSorted(Queue *q, int data)
{
    QueueNode *newNode = malloc(sizeof(QueueNode));

    if (newNode == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    newNode->data = data;
    newNode->next = NULL;

    if (q->start == NULL)
    {
        q->start = newNode;
        q->end = newNode;
        q->size++;

        return;
    }

    if (data < q->start->data)
    {
        newNode->next = q->start;

        q->start = newNode;
        q->size++;

        return;
    }

    QueueNode *temp = q->start;

    while (temp->next != NULL && temp->next->data < data)
    {
        temp = temp->next;
    }

    newNode->next = temp->next;
    temp->next = newNode;

    if (newNode->next == NULL)
    {
        q->end = newNode;
    }

    q->size++;
}

void queueSort(Queue *q)
{
    if (queueIsEmpty(q))
    {
        return;
    }

    Queue aux;
    queueCreate(&aux);

    QueueNode *temp = q->start;

    while (temp != NULL)
    {
        queueInsertSorted(&aux, temp->data);
        temp = temp->next;
    }

    queueClear(q);

    *q = aux;
}

void queueMergeCopy(Queue *dest, Queue *src)
{
    if (dest == src)
    {
        return;
    }

    QueueNode *temp = src->start;

    while (temp != NULL)
    {
        queueEnqueue(dest, temp->data);
        temp = temp->next;
    }
}

void queueMergeMove(Queue *dest, Queue *src)
{
    if (dest == src)
    {
        return;
    }

    if (queueIsEmpty(src))
    {
        return;
    }

    if (queueIsEmpty(dest))
    {
        dest->start = src->start;
        dest->end = src->end;
        dest->size = src->size;
    }
    else
    {
        dest->end->next = src->start;
        dest->end = src->end;
        dest->size += src->size;
    }

    src->start = NULL;
    src->end = NULL;
    src->size = 0;
}

void queueInterleave(Queue *dest, Queue *src)
{
    if (dest == src)
    {
        return;
    }

    QueueNode *temp1 = dest->start;
    QueueNode *temp2 = src->start;

    Queue aux;
    queueCreate(&aux);

    while (temp1 != NULL || temp2 != NULL)
    {
        if (temp1 != NULL)
        {
            queueEnqueue(&aux, temp1->data);
            temp1 = temp1->next;
        }

        if (temp2 != NULL)
        {
            queueEnqueue(&aux, temp2->data);
            temp2 = temp2->next;
        }
    }

    queueClear(dest);

    *dest = aux;
}

void queueRotate(Queue *q)
{
    if (q->size <= 1)
    {
        return;
    }

    int data = queueDequeue(q);
    queueEnqueue(q, data);
}

void queueRotateK(Queue *q, int k)
{
    if (q->size <= 1 || k == 0)
    {
        return;
    }

    k = k % q->size;

    if (k == 0)
    {
        return;
    }

    QueueNode *oldEnd = q->end;

    oldEnd->next = q->start;

    QueueNode *newEnd = q->start;

    for (int i = 0; i < k - 1; i++)
    {
        newEnd = newEnd->next;
    }

    q->start = newEnd->next;
    q->end = newEnd;
    q->end->next = NULL;
}

static QueueNode *mergeNodes(QueueNode *left, QueueNode *right)
{
    if (left == NULL)
    {
        return right;
    }

    if (right == NULL)
    {
        return left;
    }

    QueueNode *result = NULL;

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

static QueueNode *mergeSortNodes(QueueNode *start)
{
    if (start == NULL || start->next == NULL)
    {
        return start;
    }

    QueueNode *mid = findMiddleNode(start);
    QueueNode *right = mid->next;
    mid->next = NULL;

    QueueNode *leftSorted = mergeSortNodes(start);
    QueueNode *rightSorted = mergeSortNodes(right);

    return mergeNodes(leftSorted, rightSorted);
}

void queueMergeSort(Queue *q)
{
    if (q->start == NULL || q->start->next == NULL)
    {
        return;
    }

    q->start = mergeSortNodes(q->start);

    QueueNode *temp = q->start;
    q->size = 0;

    while (temp != NULL)
    {
        q->end = temp;
        temp = temp->next;
        q->size++;
    }
}

int queueIsSorted(Queue *q)
{
    if (queueIsEmpty(q))
    {
        return -1;
    }

    if (q->start->next == NULL)
    {
        return 1;
    }

    int ascending = 1;
    int descending = 1;

    QueueNode *temp = q->start;

    while (temp->next != NULL)
    {
        if (temp->data < temp->next->data)
        {
            descending = 0;
        }
        else if (temp->data > temp->next->data)
        {
            ascending = 0;
        }

        temp = temp->next;
    }

    if (ascending)
    {
        return 1;
    }

    if (descending)
    {
        return 2;
    }

    return 0;
}