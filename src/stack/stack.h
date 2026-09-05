#ifndef STACK_H
#define STACK_H

/* Ponto de entrada do submenu de pilhas, chamado pelo menu.c */
void runStackMenu(void);

typedef struct StackNode
{
    int data;
    struct StackNode *next;
} StackNode;

typedef struct
{
    StackNode *top;
    int size;
} Stack;

void stackCreate(Stack *s);
void stackClear(Stack *s);
int stackIsEmpty(Stack *s);
void stackPush(Stack *s, int data);
void stackPop(Stack *s);
void stackPopMultiple(Stack *s, int quantity);
void stackPrint(Stack *s);
void stackSearch(Stack *s, int data);
int stackCountOccurrences(Stack *s, int data);
void stackRemoveOccurrences(Stack *s, int data);
int stackReverse(Stack *s);
void stackCopy(Stack *dest, Stack *src);
int stackCompare(Stack *s1, Stack *s2);
void stackMergeTop(Stack *dest, Stack *src);
void stackMergeBottom(Stack *dest, Stack *src);
void stackSort(Stack *s);
void stackRotate(Stack *s);
void stackRotateK(Stack *s, int k);
void stackSwapTop(Stack *s);
void stackMoveTopDown(Stack *s, int k);
void stackMergeSort(Stack *s);
void stackRemoveMiddle(Stack *s);
int stackVerifyBalance(char expr[]);

#endif