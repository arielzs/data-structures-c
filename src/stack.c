#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct Stack
{
    Node *top;
    int size;
} Stack;

void pause()
{
    printf("\nPressione ENTER para continuar...");

    while (getchar() != '\n')
        ;

    getchar();
}

int isEmpty(Stack *s)
{
    return s->top == NULL;
}

void createStack(Stack *s)
{
    s->top = NULL;
    s->size = 0;
}

void push(Stack *s, int data)
{
    Node *newNode = malloc(sizeof(Node));

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

void pushMultiple(Stack *s, int quantity)
{
    if (quantity <= 0)
    {
        printf("Quantidade invalida!\n");
        return;
    }

    int data;

    for (int i = 0; i < quantity; i++)
    {
        printf("Elemento %d:\n", i + 1);
        scanf("%d", &data);
        push(s, data);
    }

    printf("%d Elemento(s) adicionado(s)!\n", quantity);
}

void pop(Stack *s)
{
    if (isEmpty(s))
    {
        printf("Pilha vazia!\n");
        return;
    }

    Node *temp = s->top;

    s->top = temp->next;

    free(temp);

    s->size--;
}

void popMultiple(Stack *s, int quantity)
{
    if (quantity <= 0)
    {
        printf("Quantidade invalida!\n");
        return;
    }

    int removed = 0;

    for (int i = 0; i < quantity && !isEmpty(s); i++)
    {
        pop(s);
        removed++;
    }

    printf("%d Elemento(s) removido(s)!\n", removed);
}

void printStack(Stack *s)
{
    if (isEmpty(s))
    {
        printf("Pilha vazia!\n");
        return;
    }

    Node *temp = s->top;

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

void searchStack(Stack *s, int data)
{
    if (isEmpty(s))
    {
        printf("Pilha Vazia!\n");
        return;
    }

    Node *temp = s->top;
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

int countOccurrences(Stack *s, int data)
{
    Node *temp = s->top;

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

void removeOccurrences(Stack *s, int data)
{
    if (isEmpty(s))
    {
        printf("Pilha Vazia!\n");
        return;
    }

    Node *curr = s->top;
    Node *prev = NULL;

    int quantity = 0;

    while (curr != NULL)
    {
        if (curr->data == data)
        {
            Node *removed = curr;
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

void clearStack(Stack *s)
{
    Node *temp = s->top;

    while (temp != NULL)
    {
        Node *next = temp->next;

        free(temp);

        temp = next;
    }

    s->top = NULL;
    s->size = 0;
}

int reverseStack(Stack *s)
{
    if (isEmpty(s))
    {
        return 0;
    }

    Stack aux;
    createStack(&aux);
    Node *temp = s->top;

    while (temp != NULL)
    {
        push(&aux, temp->data);
        temp = temp->next;
    }

    clearStack(s);
    s->top = aux.top;
    s->size = aux.size;

    return 1;
}

void copyStack(Stack *dest, Stack *src)
{
    if (dest == src)
    {
        return;
    }

    clearStack(dest);

    Node *temp = src->top;

    while (temp != NULL)
    {
        push(dest, temp->data);

        temp = temp->next;
    }

    reverseStack(dest);
}

int compareStack(Stack *s1, Stack *s2)
{
    if (s1->size != s2->size)
    {
        return 0;
    }

    Node *temp1 = s1->top;
    Node *temp2 = s2->top;

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

void mergeStackTop(Stack *dest, Stack *src)
{
    if (dest == src)
    {
        return;
    }

    Stack aux;

    createStack(&aux);

    Node *temp = src->top;

    while (temp != NULL)
    {
        push(&aux, temp->data);
        temp = temp->next;
    }

    temp = aux.top;

    while (temp != NULL)
    {
        push(dest, temp->data);
        temp = temp->next;
    }

    clearStack(&aux);
}

void mergeStackBottom(Stack *dest, Stack *src)
{
    if (dest == src)
    {
        return;
    }

    Stack aux;

    createStack(&aux);
    copyStack(&aux, dest);
    reverseStack(&aux);

    Node *temp = src->top;

    while (temp != NULL)
    {
        push(&aux, temp->data);
        temp = temp->next;
    }

    reverseStack(&aux);
    copyStack(dest, &aux);
    clearStack(&aux);
}

void sortStack(Stack *s)
{
    Stack aux;
    createStack(&aux);

    while (!isEmpty(s))
    {
        int temp = s->top->data;
        pop(s);

        while (!(isEmpty(&aux)) && aux.top->data > temp)
        {
            push(s, aux.top->data);
            pop(&aux);
        }

        push(&aux, temp);
    }

    copyStack(s, &aux);
}

void rotateStack(Stack *s)
{
    if (s->size < 2)
    {
        return;
    }

    Node *oldTop = s->top;
    s->top = oldTop->next;

    Node *temp = s->top;

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = oldTop;
    oldTop->next = NULL;
}

void rotateK(Stack *s, int k)
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

    Node *last = s->top;

    while (last->next != NULL)
    {
        last = last->next;
    }

    last->next = s->top;

    Node *newTail = s->top;

    for (int i = 0; i < k - 1; i++)
    {
        newTail = newTail->next;
    }

    Node *newTop = newTail->next;

    newTail->next = NULL;

    s->top = newTop;
}

void swapTop(Stack *s)
{
    if (s->size < 2)
    {
        return;
    }

    Node *oldTop = s->top;
    Node *newTop = oldTop->next;

    s->top = newTop;
    oldTop->next = newTop->next;
    newTop->next = oldTop;
}

void pushTopK(Stack *s, int k)
{
    if (s->size < 2 || k <= 0)
    {
        return;
    }

    if (k > s->size - 1)
    {
        k = s->size - 1;
    }

    Node *temp = s->top;

    s->top = temp->next;

    Node *curr = s->top;

    for (int i = 0; i < k - 1; i++)
    {
        curr = curr->next;
    }

    temp->next = curr->next;
    curr->next = temp;
}

Node *findMiddle(Node *top)
{
    if (top == NULL)
    {
        return NULL;
    }

    Node *slow = top;
    Node *fast = top->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

Node *merge(Node *left, Node *right)
{
    if (left == NULL)
    {
        return right;
    }

    if (right == NULL)
    {
        return left;
    }

    Node *result = NULL;

    if (left->data <= right->data)
    {
        result = left;
        result->next = merge(left->next, right);
    }
    else
    {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

Node *mergeSort(Node *top)
{
    if (top == NULL || top->next == NULL)
    {
        return top;
    }

    Node *mid = findMiddle(top);
    Node *right = mid->next;
    mid->next = NULL;

    Node *leftSorted = mergeSort(top);
    Node *rightSorted = mergeSort(right);

    return merge(leftSorted, rightSorted);
}

void mergeSortStack(Stack *s)
{
    if (s->top == NULL || s->top->next == NULL)
        return;

    s->top = mergeSort(s->top);

    Node *temp = s->top;
    s->size = 0;

    while (temp != NULL)
    {
        temp = temp->next;
        s->size++;
    }
}

void removeMiddle(Stack *s)
{
    if (isEmpty(s))
    {
        return;
    }

    if (s->size == 1)
    {
        pop(s);
        return;
    }

    if (s->size == 2)
    {
        Node *mid = s->top->next;

        s->top->next = NULL;

        free(mid);

        s->size--;

        return;
    }

    Node *slow = s->top;
    Node *fast = s->top->next;
    Node *prev = NULL;

    while (fast != NULL && fast->next != NULL)
    {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    Node *mid = slow;

    prev->next = mid->next;
    mid->next = NULL;

    free(mid);
    s->size--;
}

int match(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

int isOpening(char c)
{
    return c == '(' || c == '[' || c == '{';
}

int isClosing(char c)
{
    return c == ')' || c == ']' || c == '}';
}

int verifyBalance(char expr[])
{
    Stack aux;
    createStack(&aux);

    for (int i = 0; expr[i] != '\0'; i++)
    {
        char c = expr[i];

        if (isOpening(c))
        {
            push(&aux, c);
        }
        else if (isClosing(c))
        {
            if (isEmpty(&aux))
            {
                clearStack(&aux);
                return 0;
            }

            char top = aux.top->data;

            if (!match(top, c))
            {
                clearStack(&aux);
                return 0;
            }

            pop(&aux);
        }
    }

    int ok = isEmpty(&aux);
    clearStack(&aux);

    return ok;
}

int main()
{
    int command, data, quantity, count, k;

    Stack s1;
    Stack s2;

    createStack(&s1);
    createStack(&s2);

    Stack *active = &s1;
    int activeNum = 1;

    while (1)
    {
        printf("\n=== MENU | Pilha ativa: %d ===\n", activeNum);
        printf("\n[1] Inserir elementos\n");
        printf("[2] Remover elementos\n");
        printf("[3] Remover ocorrencias\n");
        printf("[4] Procurar elemento\n");
        printf("[5] Ver a quantia de um elemento\n");
        printf("[6] Inverter Pilha\n");
        printf("[7] Mostrar Pilha atual\n");
        printf("[8] Copiar a Pilha ativa na outra\n");
        printf("[9] Comparar Pilhas\n");
        printf("[10] Juntar Topo da Pilha\n");
        printf("[11] Juntar Fim da Pilha\n");
        printf("[12] Ordenar Pilha\n");
        printf("[13] Trocar de Pilha\n");
        printf("[14] Troca Topo\n");
        printf("[15] Empurra o Topo K posicoes\n");
        printf("[16] Rotaciona Pilha\n");
        printf("[17] Rotaciona Pilha K vezes\n");
        printf("[18] Ordenar com MergeSort\n");
        printf("[19] Remove o elemento do meio\n");
        printf("[20] Verifica Expressao\n");
        printf("\n[0] Encerrar o programa\n");

        scanf("%d", &command);

        switch (command)
        {
        case 0:
            clearStack(&s1);
            clearStack(&s2);
            return 0;

        case 1:
            printf("Quantos elementos devem ser adicionados?\n");
            scanf("%d", &quantity);

            pushMultiple(active, quantity);

            break;

        case 2:
            if (!isEmpty(active))
            {
                printf("Quantos elementos devem ser removidos?\n");
                scanf("%d", &quantity);
                popMultiple(active, quantity);
            }
            else
            {
                printf("Pilha vazia!\n");
            }

            break;

        case 3:
            printf("Qual ocorrencia deve ser removida?\n");
            scanf("%d", &data);

            removeOccurrences(active, data);

            break;

        case 4:
            printf("Qual elemento deve ser buscado?\n");
            scanf("%d", &data);

            searchStack(active, data);

            break;

        case 5:
            printf("Qual elemento deve ser contado?\n");
            scanf("%d", &data);
            count = countOccurrences(active, data);

            printf("O elemento %d aparece %d vez(es)!\n", data, count);

            break;

        case 6:
            if (reverseStack(active))
            {
                printf("Pilha invertida!\n");
            }
            else
            {
                printf("Pilha vazia!\n");
            }

            break;

        case 7:
            printStack(active);
            pause();

            break;

        case 8:
            if (active == &s1)
            {
                copyStack(&s2, active);
            }
            else
            {
                copyStack(&s1, active);
            }

            printf("Pilha copiada!\n");

            break;

        case 9:
            if (compareStack(&s1, &s2))
            {
                printf("Pilhas iguais!\n");
            }
            else
            {
                printf("Pilhas diferentes!\n");
            }

            break;

        case 10:
        {
            Stack *other;

            if (active == &s1)
            {
                other = &s2;
            }
            else
            {
                other = &s1;
            }

            mergeStackTop(active, other);

            printf("Pilhas unidas!\n");

            break;
        }

        case 11:
        {
            Stack *other;

            if (active == &s1)
            {
                other = &s2;
            }
            else
            {
                other = &s1;
            }

            mergeStackBottom(active, other);

            printf("Pilhas unidas!\n");

            break;
        }

        case 12:
            sortStack(active);
            printf("Pilha Ordenada!");

            break;

        case 13:
            if (activeNum == 1)
            {
                active = &s2;
                activeNum = 2;
            }
            else
            {
                active = &s1;
                activeNum = 1;
            }

            printf("Pilha ativa: %d\n", activeNum);

            break;

        case 14:
            swapTop(active);

            printf("Topo trocado\n");

            break;

        case 15:
            printf("Quantas posicoes?\n");
            scanf("%d", &k);

            pushTopK(active, k);

            printf("Topo movido %d vezes\n", k);

            break;

        case 16:
            rotateStack(active);

            printf("Pilha rotacionada\n");

            break;

        case 17:
            printf("Quantas vezes?\n");
            scanf("%d", &k);

            rotateK(active, k);

            printf("Pilha rotacionada %d vezes\n", k);

            break;

        case 18:
            mergeSortStack(active);

            printf("Pilha Ordenada!\n");

            break;

        case 19:
            removeMiddle(active);

            printf("Elemento do meio removido\n");

            break;

        case 20:
        {
            char expr[100];

            printf("Digite a expressao: ");
            scanf("%s", expr);

            if (verifyBalance(expr))
                printf("Expressao valida!\n");
            else
                printf("Expressao invalida!\n");

            break;
        }

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }

    return 0;
}