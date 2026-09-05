#ifndef QUEUE_H
#define QUEUE_H

/* Ponto de entrada do submenu de filas, chamado pelo menu.c */
void runQueueMenu(void);

typedef struct QueueNode
{
    int data;
    struct QueueNode *next;
} QueueNode;

typedef struct
{
    QueueNode *start;
    QueueNode *end;
    int size;
} Queue;

void queueCreate(Queue *q);
void queueClear(Queue *q);
int queueIsEmpty(Queue *q);
void queuePrint(Queue *q);
void queueEnqueue(Queue *q, int data);
int queueDequeue(Queue *q);
int queuePeek(Queue *q);
int queueContains(Queue *q, int data);
int queueCountOccurrences(Queue *q, int data);
int queueRemoveOccurrences(Queue *q, int data);
void queueRemoveDuplicates(Queue *q);
void queueCopy(Queue *dest, Queue *src);
int queueCompare(Queue *q1, Queue *q2);
void queueReverse(Queue *q);
int queueFindMiddle(Queue *q);
void queueInsertSorted(Queue *q, int data);
void queueSort(Queue *q);
void queueMergeCopy(Queue *dest, Queue *src);
void queueMergeMove(Queue *dest, Queue *src);
void queueInterleave(Queue *dest, Queue *src);
void queueRotate(Queue *q);
void queueRotateK(Queue *q, int k);
void queueMergeSort(Queue *q);
int queueIsSorted(Queue *q);

#endif