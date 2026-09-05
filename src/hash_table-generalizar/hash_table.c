#include <stdio.h>
#include <stdlib.h>
/*

typedef struct Node
{
    int key;
    struct Node *next;
} Node;

typedef struct
{
    int size;
    int count;
    Node **table;
} HashTable;

void pause()
{
    printf("\nPressione ENTER para continuar...");
    while (getchar() != '\n')
        ;
    getchar();
}

int hash(int size, int key)
{
    return ((key % size) + size) % size;
}

int reHashPointer(HashTable **ht, int newSize);

HashTable *createHashTable(int size)
{
    HashTable *ht = malloc(sizeof(HashTable));
    if (ht == NULL)
    {
        return NULL;
    }

    ht->count = 0;
    ht->size = size;
    ht->table = calloc(size, sizeof(Node *));

    if (ht->table == NULL)
    {
        free(ht);
        return NULL;
    }

    return ht;
}

void clearElements(HashTable *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        Node *current = ht->table[i];

        while (current != NULL)
        {
            Node *temp = current;
            current = current->next;
            free(temp);
        }

        ht->table[i] = NULL;
    }

    ht->count = 0;
}

void clearHashTable(HashTable *ht)
{
    if (ht == NULL)
    {
        return;
    }

    clearElements(ht);
    free(ht->table);
    free(ht);
}

void printTable(HashTable *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        printf("[%d] -> ", i);

        Node *current = ht->table[i];

        while (current != NULL)
        {
            printf("%d -> ", current->key);
            current = current->next;
        }

        printf("NULL\n");
    }
}

Node *createNode(int key)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->key = key;
    newNode->next = NULL;

    return newNode;
}

void insertKey(HashTable *ht, int key)
{
    int index = hash(ht->size, key);

    Node *newNode = createNode(key);

    if (newNode == NULL)
    {
        return;
    }

    newNode->next = ht->table[index];
    ht->table[index] = newNode;
    ht->count++;

    float loadFactor = (float)ht->count / ht->size;

    if (loadFactor >= 0.75)
    {
        if (reHashPointer(&ht, ht->size * 2))
        {
            printf("ReHash automatico realizado!\n");
        }
    }
}

void insertKeyWithoutResize(HashTable *ht, int key)
{
    int index = hash(ht->size, key);

    Node *newNode = createNode(key);

    if (newNode == NULL)
        return;

    newNode->next = ht->table[index];
    ht->table[index] = newNode;
    ht->count++;
}

int removeKey(HashTable *ht, int key)
{
    int index = hash(ht->size, key);

    Node *current = ht->table[index];
    Node *prev = NULL;

    while (current != NULL)
    {
        if (current->key == key)
        {
            if (prev == NULL)
            {
                ht->table[index] = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            free(current);
            ht->count--;

            return 1;
        }

        prev = current;
        current = current->next;
    }

    return 0;
}

Node *searchKey(HashTable *ht, int key)
{
    int index = hash(ht->size, key);

    Node *current = ht->table[index];

    while (current != NULL)
    {
        if (current->key == key)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

HashTable *reHash(HashTable *ht, int newSize)
{
    HashTable *newHt = createHashTable(newSize);

    if (newHt == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < ht->size; i++)
    {
        Node *current = ht->table[i];

        while (current != NULL)
        {
            insertKeyWithoutResize(newHt, current->key);
            current = current->next;
        }
    }

    clearHashTable(ht);

    return newHt;
}

int reHashPointer(HashTable **ht, int newSize)
{
    Node **newTable = calloc(newSize, sizeof(Node *));

    if (newTable == NULL)
    {
        return 0;
    }

    for (int i = 0; i < (*ht)->size; i++)
    {
        Node *current = (*ht)->table[i];

        while (current != NULL)
        {
            Node *next = current->next;

            int newIndex = hash(newSize, current->key);

            current->next = newTable[newIndex];
            newTable[newIndex] = current;

            current = next;
        }
    }

    free((*ht)->table);

    (*ht)->table = newTable;
    (*ht)->size = newSize;

    return 1;
}

void compareHashTables(HashTable *ht1, HashTable *ht2)
{
    float loadFactor1 = (float)ht1->count / ht1->size;
    float loadFactor2 = (float)ht2->count / ht2->size;

    int collisions1 = 0;
    int collisions2 = 0;

    int maxChain1 = 0;
    int maxChain2 = 0;

    for (int i = 0; i < ht1->size; i++)
    {
        int chainSize = 0;

        Node *current = ht1->table[i];

        while (current != NULL)
        {
            chainSize++;
            current = current->next;
        }

        if (chainSize > 1)
        {
            collisions1 += chainSize - 1;
        }

        if (chainSize > maxChain1)
        {
            maxChain1 = chainSize;
        }
    }

    for (int i = 0; i < ht2->size; i++)
    {
        int chainSize = 0;

        Node *current = ht2->table[i];

        while (current != NULL)
        {
            chainSize++;
            current = current->next;
        }

        if (chainSize > 1)
        {
            collisions2 += chainSize - 1;
        }

        if (chainSize > maxChain2)
        {
            maxChain2 = chainSize;
        }
    }

    printf("\n====== Comparacao ======\n");

    printf("\nTabela 1:");
    printf("\nElementos: %d", ht1->count);
    printf("\nTamanho: %d", ht1->size);
    printf("\nLoad Factor: %.2f", loadFactor1);
    printf("\nColisoes: %d", collisions1);
    printf("\nMaior lista: %d", maxChain1);

    printf("\n\nTabela 2:");
    printf("\nElementos: %d", ht2->count);
    printf("\nTamanho: %d", ht2->size);
    printf("\nLoad Factor: %.2f", loadFactor2);
    printf("\nColisoes: %d", collisions2);
    printf("\nMaior lista: %d", maxChain2);

    printf("\n\nMelhor tabela: ");

    if (collisions1 < collisions2)
    {
        printf("Tabela 1 (menos colisoes)\n");
    }
    else if (collisions2 < collisions1)
    {
        printf("Tabela 2 (menos colisoes)\n");
    }
    else
    {
        printf("Empate\n");
    }
}

int main()
{
    int command, key;

    HashTable *ht1 = NULL;
    HashTable *ht2 = NULL;
    HashTable **activeHt = NULL;

    int activeNum = 1;

    while (1)
    {
        printf("\n=== MENU | HashTable ativa: %d ===\n", activeNum);
        printf("[0] Inicia as Tabelas\n");
        printf("[1] Mostra a Tabela\n");
        printf("[2] Insere elemento\n");
        printf("[3] Remove elemento\n");
        printf("[4] Busca um elemento\n");
        printf("[5] Troca de Tabela\n");
        printf("[6] ReHash Manual\n");
        printf("[7] Compara Tabelas\n");
        printf("[8] Limpa a Tabela\n");
        printf("[9] Encerra o programa\n");

        scanf("%d", &command);

        switch (command)
        {
        case 0:
        {
            if (ht1 != NULL || ht2 != NULL)
            {
                clearHashTable(ht1);
                clearHashTable(ht2);
            }

            int size1, size2;

            printf("Tamanho da Tabela 1:\n");
            scanf("%d", &size1);

            printf("Tamanho da Tabela 2:\n");
            scanf("%d", &size2);

            ht1 = createHashTable(size1);
            ht2 = createHashTable(size2);

            if (ht1 == NULL || ht2 == NULL)
            {
                printf("Erro ao criar Hash Table\n");
                return 1;
            }

            activeHt = &ht1;

            printf("Tabelas criadas!\n");

            break;
        }

        case 1:
            if (activeHt == NULL)
            {
                printf("Crie as tabelas primeiro!\n");
                break;
            }

            printTable(*activeHt);
            pause();

            break;

        case 2:
            if (activeHt == NULL)
            {
                printf("Crie as tabelas primeiro!\n");
                break;
            }

            printf("Qual key?\n");
            scanf("%d", &key);

            insertKey(*activeHt, key);

            printf("Key inserida\n");

            break;

        case 3:
        {
            if (activeHt == NULL)
            {
                printf("Crie as tabelas primeiro!\n");
                break;
            }

            printf("Qual key?\n");
            scanf("%d", &key);

            int result = removeKey(*activeHt, key);

            if (result == 1)
            {
                printf("Key removida\n");
            }
            else
            {
                printf("Key nao encontrada\n");
            }

            break;
        }

        case 4:
        {
            if (activeHt == NULL)
            {
                printf("Crie as tabelas primeiro!\n");
                break;
            }

            printf("Qual key?\n");
            scanf("%d", &key);

            Node *result = searchKey(*activeHt, key);

            if (result != NULL)
            {
                printf("Key encontrada\n");
            }
            else
            {
                printf("Key nao encontrada\n");
            }

            break;
        }

        case 5:
            if (activeHt == NULL)
            {
                printf("Crie as tabelas primeiro!\n");
                break;
            }

            if (activeNum == 1)
            {
                activeHt = &ht2;
                activeNum = 2;
            }
            else
            {
                activeHt = &ht1;
                activeNum = 1;
            }

            printf("Tabela ativa: %d\n", activeNum);

            break;

        case 6:
        {
            if (activeHt == NULL)
            {
                printf("Crie as tabelas primeiro!\n");
                break;
            }

            int option, newSize;

            float loadFactor = (float)(*activeHt)->count / (*activeHt)->size;

            if (loadFactor < 0.75)
            {
                printf("O loadfactor eh: %.2f\n\n", loadFactor);
                printf("A tabela nao esta muito cheia, deseja continuar mesmo assim?\n");
                printf("[1] Sim\n");
                printf("[2] Nao\n");

                scanf("%d", &option);

                if (option == 2)
                {
                    break;
                }
            }

            printf("Qual sera o novo tamanho?\n");
            scanf("%d", &newSize);

            if (newSize <= 0)
            {
                printf("Tamanho invalido!\n");
                break;
            }

            int verify = reHashPointer(activeHt, newSize);

            if (verify == 0)
            {
                printf("Erro ao alocar memoria\n");
                break;
            }

            loadFactor = (float)(*activeHt)->count / (*activeHt)->size;

            printf("ReHash feito\n");
            printf("Novo LoadFactor: %.2f\n", loadFactor);

            break;
        }

        case 7:
            if (ht1 == NULL || ht2 == NULL)
            {
                printf("Crie as tabelas primeiro!\n");
                break;
            }

            compareHashTables(ht1, ht2);
            pause();

            break;

        case 8:
            if (activeHt == NULL)
            {
                printf("Crie as tabelas primeiro!\n");
                break;
            }

            clearElements(*activeHt);

            break;

        case 9:
            if (ht1 != NULL)
            {
                clearHashTable(ht1);
            }

            if (ht2 != NULL)
            {
                clearHashTable(ht2);
            }

            return 0;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }

    return 0;
}
      
*/