#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct Queue
{
    Node *start;
    Node *end;
    int size;
} Queue;

void pause()
{
    printf("\nPressione ENTER para continuar...");

    while (getchar() != '\n')
        ;

    getchar();
}

void createQueue(Queue *q)
{
    q->start = NULL;
    q->end = NULL;
    q->size = 0;
}

void clearQueue(Queue *q)
{
    Node *temp = q->start;

    while (temp != NULL)
    {
        Node *removed = temp;

        temp = temp->next;

        free(removed);
    }

    q->start = NULL;
    q->end = NULL;
    q->size = 0;
}

int isEmpty(Queue *q)
{
    return q->size == 0;
}

void printQueue(Queue *q)
{
    if (isEmpty(q))
    {
        printf("INICIO -> NULL <- FIM");
        return;
    }

    Node *temp = q->start;

    printf("\nINICIO -> ");

    while (temp != NULL)
    {
        printf("[%d] -> ", temp->data);
        temp = temp->next;
    }

    printf("NULL\n");
    printf("\nTamanho da fila: %d\n", q->size);
}

void enqueue(Queue *q, int data)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(q))
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

int dequeue(Queue *q)
{
    if (isEmpty(q))
    {
        printf("Fila Vazia!");
        return -1;
    }

    int data = q->start->data;

    Node *temp = q->start;

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

int peek(Queue *q)
{
    if (isEmpty(q))
    {
        return -1;
    }

    return q->start->data;
}

int contains(Queue *q, int data)
{
    Node *temp = q->start;

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

int countOccurrences(Queue *q, int data)
{
    Node *temp = q->start;

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

int removeOccurrences(Queue *q, int data)
{
    Node *current = q->start;
    Node *prev = NULL;
    int removeds = 0;

    while (current != NULL)
    {
        if (current->data == data)
        {
            Node *temp = current;

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

void removeDuplicates(Queue *q)
{
    if (isEmpty(q))
    {
        printf("Fila Vazia!\n");
        return;
    }

    Node *current = q->start;

    while (current != NULL)
    {
        Node *runner = current;

        while (runner->next != NULL)
        {
            if (runner->next->data == current->data)
            {
                Node *removed = runner->next;

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

void copyQueue(Queue *dest, Queue *src)
{
    if (dest == src)
    {
        return;
    }

    clearQueue(dest);

    Node *temp = src->start;

    while (temp != NULL)
    {
        enqueue(dest, temp->data);

        temp = temp->next;
    }
}

int compareQueue(Queue *q1, Queue *q2)
{
    if (q1->size != q2->size)
    {
        return 0;
    }

    Node *temp1 = q1->start;
    Node *temp2 = q2->start;

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

void reverseQueue(Queue *q)
{
    Node *current = q->start;
    Node *prev = NULL;
    Node *next;

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

int findValueMiddle(Queue *q)
{
    if (isEmpty(q))
    {
        return -1;
    }

    int middle = q->size / 2;

    Node *temp = q->start;

    for (int i = 0; i < middle; i++)
    {
        temp = temp->next;
    }

    return temp->data;
}

Node *findMiddle(Node *start)
{
    if (start == NULL)
    {
        return NULL;
    }

    Node *slow = start;
    Node *fast = start->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

void insertSorted(Queue *q, int data)
{
    Node *newNode = malloc(sizeof(Node));

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

    Node *temp = q->start;

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

void sortQueue(Queue *q)
{
    if (isEmpty(q))
    {
        return;
    }

    Queue aux;
    createQueue(&aux);

    Node *temp = q->start;

    while (temp != NULL)
    {
        insertSorted(&aux, temp->data);
        temp = temp->next;
    }

    clearQueue(q);

    *q = aux;
}

void mergeQueues(Queue *dest, Queue *src)
{
    Node *temp = src->start;

    while (temp != NULL)
    {
        enqueue(dest, temp->data);
        temp = temp->next;
    }
}

void mergeQueues2(Queue *dest, Queue *src)
{
    if (isEmpty(src))
    {
        return;
    }

    if (isEmpty(dest))
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

void interleaveQueues(Queue *dest, Queue *src)
{
    Node *temp1 = dest->start;
    Node *temp2 = src->start;

    Queue aux;
    createQueue(&aux);

    while (temp1 != NULL || temp2 != NULL)
    {
        if (temp1 != NULL)
        {
            enqueue(&aux, temp1->data);
            temp1 = temp1->next;
        }

        if (temp2 != NULL)
        {
            enqueue(&aux, temp2->data);
            temp2 = temp2->next;
        }
    }

    clearQueue(dest);

    *dest = aux;
}

void rotateQueue(Queue *q)
{
    if (q->size <= 1)
    {
        return;
    }

    int data = dequeue(q);
    enqueue(q, data);
}

void rotateQueueK(Queue *q, int k)
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

    Node *oldEnd = q->end;

    oldEnd->next = q->start;

    Node *newEnd = q->start;

    for (int i = 0; i < k - 1; i++)
    {
        newEnd = newEnd->next;
    }

    q->start = newEnd->next;
    q->end = newEnd;
    q->end->next = NULL;
}

Node *merge(Node *leftQueue, Node *rightQueue)
{
    if (leftQueue == NULL)
    {
        return rightQueue;
    }

    if (rightQueue == NULL)
    {
        return leftQueue;
    }

    Node *result = NULL;

    if (leftQueue->data <= rightQueue->data)
    {
        result = leftQueue;
        result->next = merge(leftQueue->next, rightQueue);
    }
    else
    {
        result = rightQueue;
        result->next = merge(leftQueue, rightQueue->next);
    }

    return result;
}

Node *mergeSort(Node *start)
{
    if (start == NULL || start->next == NULL)
    {
        return start;
    }

    Node *mid = findMiddle(start);
    Node *right = mid->next;
    mid->next = NULL;

    Node *leftSorted = mergeSort(start);
    Node *rightSorted = mergeSort(right);

    return merge(leftSorted, rightSorted);
}

void mergeSortQueue(Queue *q)
{
    if (q->start == NULL || q->start->next == NULL)
        return;

    q->start = mergeSort(q->start);

    Node *temp = q->start;
    q->size = 0;

    while (temp != NULL)
    {
        q->end = temp;
        temp = temp->next;
        q->size++;
    }
}

int isSortedQueue(Queue *q)
{
    if (isEmpty(q))
    {
        return -1;
    }

    if (q->start->next == NULL)
    {
        return 1;
    }

    int ascending = 1;
    int descending = 1;

    Node *temp = q->start;

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

int main()
{
    int command, data;

    Queue q1;
    Queue q2;

    createQueue(&q1);
    createQueue(&q2);

    Queue *active = &q1;
    int activeNum = 1;

    while (1)
    {
        printf("\n=== MENU | Fila ativa: %d ===\n", activeNum);
        printf("\n[1] Mostrar Fila Atual\n");
        printf("[2] Trocar de fila\n");
        printf("[3] Inserir no fim da fila\n");
        printf("[4] Remover do inicio da fila\n");
        printf("[5] Mostra o Inicio da Fila\n");
        printf("[6] Busca um valor na fila\n");
        printf("[7] Conta ocorrencias de um valor\n");
        printf("[8] Copia a fila ativa na outra\n");
        printf("[9] Compara duas filas\n");
        printf("[10] Mostra o elemento do meio\n");
        printf("[11] Remove ocorrencias\n");
        printf("[12] Remover duplicatas\n");
        printf("[13] Inserir Ordenado\n");
        printf("[14] Ordenar Fila\n");
        printf("[15] Junta Filas\n");
        printf("[16] Intercala Filas\n");
        printf("[17] Rotaciona a fila\n");
        printf("[18] Rotaciona a fila K vezes\n");
        printf("[19] Ordenar com MergeSort\n");
        printf("[20] Verifica a Ordenacao\n");
        printf("\n[0] Encerrar o Programa\n");

        scanf("%d", &command);

        switch (command)
        {
        case 0:
            clearQueue(&q1);
            clearQueue(&q2);

            return 0;

        case 1:
            printQueue(active);
            pause();

            break;

        case 2:
            if (activeNum == 1)
            {
                active = &q2;
                activeNum = 2;
            }
            else
            {
                active = &q1;
                activeNum = 1;
            }

            break;

        case 3:
            printf("Qual elemento deve ser adicionado?\n");
            scanf("%d", &data);

            enqueue(active, data);
            printf("Elemento adicionado!\n");

            break;

        case 4:
        {
            int removed = dequeue(active);

            if (removed != -1)
            {
                printf("Elemento %d removido!\n", removed);
            }

            break;
        }

        case 5:
        {
            if (isEmpty(active))
            {
                printf("Fila Vazia!\n");
            }
            else
            {
                int inicio = peek(active);

                printf("O inicio eh: %d\n", inicio);
            }

            break;
        }

        case 6:
        {
            printf("Qual elemento deve ser procurado?\n");
            scanf("%d", &data);

            int pos = contains(active, data);

            if (pos == -1)
            {
                printf("Elemento nao encontrado!\n");
            }
            else
            {
                printf("Elemento %d encontrado na posicao %d\n", data, pos);
            }

            break;
        }

        case 7:
        {

            if (isEmpty(active))
            {
                printf("Fila Vazia!\n");
                break;
            }

            printf("Qual elemento deve ser contado?\n");
            scanf("%d", &data);

            int count = countOccurrences(active, data);

            printf("O elemento %d aparece %d vez(es) na fila\n", data, count);

            break;
        }

        case 8:
            if (activeNum == 1)
            {
                copyQueue(&q2, active);
            }
            else
            {
                copyQueue(&q1, active);
            }

            printf("Fila copiada!\n");

            break;

        case 9:
            if (compareQueue(&q1, &q2))
            {
                printf("FIlas iguais!\n");
            }
            else
            {
                printf("Filas diferentes!\n");
            }

            break;

        case 10:
        {
            int middle = findValueMiddle(active);

            if (middle == -1)
            {
                printf("Fila Vazia!\n");
            }
            else
            {
                printf("O elemento do meio eh: %d\n", middle);
            }

            break;
        }

        case 11:
        {
            printf("Qual ocorrencia?\n");
            scanf("%d", &data);

            int removeds = removeOccurrences(active, data);

            if (removeds == 0)
            {
                printf("Nenhuma ocorrencia encontrada\n");
            }
            else
            {
                printf("%d ocorrencia(s) removidas\n", removeds);
            }

            break;
        }

        case 12:
            removeDuplicates(active);

            printf("Duplicatas removidas!\n");

            break;

        case 13:
            printf("Qual elemento?\n");
            scanf("%d", &data);
            insertSorted(active, data);

            printf("Elemento %d inserido ordenado!\n", data);

            break;

        case 14:
            sortQueue(active);

            printf("Fila Ordenada!\n");

            break;

        case 15:
            if (activeNum == 1)
            {
                mergeQueues2(active, &q2);
            }
            else
            {
                mergeQueues2(active, &q1);
            }

            printf("Filas Unidas!\n");

            break;

        case 16:
            if (activeNum == 1)
            {
                interleaveQueues(active, &q2);
            }
            else
            {
                interleaveQueues(active, &q1);
            }

            printf("Filas intercaladas!\n");

            break;

        case 17:
            rotateQueue(active);

            printf("Fila Rotacionada!\n");

            break;

        case 18:
        {
            int k;

            printf("Quantas Vezes?\n");
            scanf("%d", &k);

            rotateQueueK(active, k);

            printf("Fila Rotacionada %d vezes!\n", k);

            break;
        }

        case 19:
            mergeSortQueue(active);

            printf("Fila Ordenada!\n");

            break;

        case 20:
        {
            int sorted = isSortedQueue(active);

            if (sorted == 1)
            {
                printf("Fila em Ordem Crescente\n");
            }
            else if (sorted == 2)
            {
                printf("Fila em Ordem Decrescente\n");
            }
            else if (sorted == -1)
            {
                printf("Fila Vazia\n");
            }
            else
            {
                printf("Fila Nao Ordenada\n");
            }

            break;
        }

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }

    return 0;
}
