#include "common.h"
#include "utils.h"
#include "hash_table.h"

typedef struct
{
    int id;
    HashTable *table;
} HashTableInstance;

static HashTableInstance *tables = NULL;
static int totalTables = 0;
static int tablesCapacity = 0;
static int active = -1; /* indice em 'tables', -1 = nenhuma hash table ativa */

static void createHashTableInstance(void)
{
    int size = utilsReadInt("Qual o tamanho da tabela?\n");

    if (size <= 0)
    {
        printf("Tamanho invalido!\n");
        return;
    }

    HashTable *table = hashTableCreate(size);

    if (table == NULL)
    {
        printf("Erro ao criar a hash table\n");
        return;
    }

    if (totalTables == tablesCapacity)
    {
        if (tablesCapacity == 0)
        {
            tablesCapacity = 4;
        }
        else
        {
            tablesCapacity = tablesCapacity * 2;
        }

        tables = realloc(tables, tablesCapacity * sizeof(HashTableInstance));

        if (tables == NULL)
        {
            printf("Erro ao alocar memoria\n");
            exit(1);
        }
    }

    tables[totalTables].id = totalTables + 1;
    tables[totalTables].table = table;

    active = totalTables;
    totalTables++;

    printf("Hash Table %d criada e definida como ativa!\n", tables[active].id);
}

static void listHashTableInstances(void)
{
    if (totalTables == 0)
    {
        printf("Nenhuma hash table criada ainda. Use o comando [1] para criar uma.\n");
        return;
    }

    printf("=== Hash Tables criadas ===\n");

    for (int i = 0; i < totalTables; i++)
    {
        if (i == active)
        {
            printf("Hash Table %d (ativa)\n", tables[i].id);
        }
        else
        {
            printf("Hash Table %d\n", tables[i].id);
        }
    }
}

static void switchHashTableInstance(int number)
{
    if (number < 1 || number > totalTables)
    {
        printf("Hash Table invalida!\n");
        return;
    }

    active = number - 1;
    printf("Hash Table ativa agora: %d\n", tables[active].id);
}

void runHashTableMenu(void)
{
    int command, key;
    int running = 1;

    while (running)
    {
        printf("\n=== MENU HASH TABLES");

        if (active != -1)
        {
            printf(" | Hash Table ativa: %d", tables[active].id);
        }

        printf(" ===\n");

        printf("[0] Voltar ao menu principal\n");
        printf("[1] Cria uma nova hash table\n");
        printf("[2] Lista as hash tables criadas\n");
        printf("[3] Troca a hash table ativa\n");
        printf("[4] Mostra a tabela atual\n");
        printf("[5] Insere elemento\n");
        printf("[6] Remove elemento\n");
        printf("[7] Busca um elemento\n");
        printf("[8] ReHash manual (rapido, reaproveita os nodes)\n");
        printf("[9] ReHash manual (recria a tabela do zero)\n");
        printf("[10] Compara duas tabelas\n");
        printf("[11] Limpa os elementos da tabela ativa\n");

        command = utilsReadInt("Escolha: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("Voce precisa criar uma hash table primeiro! (comando 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
            createHashTableInstance();
            break;

        case 2:
            listHashTableInstances();
            break;

        case 3:
        {
            listHashTableInstances();

            int number = utilsReadInt("Qual hash table deseja ativar?\n");

            switchHashTableInstance(number);
            break;
        }

        case 4:
            hashTablePrint(tables[active].table);
            utilsPause();

            break;

        case 5:
            key = utilsReadInt("Qual key?\n");

            hashTableInsert(tables[active].table, key);

            printf("Key inserida\n");

            break;

        case 6:
        {
            key = utilsReadInt("Qual key?\n");

            int result = hashTableRemove(tables[active].table, key);

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

        case 7:
        {
            key = utilsReadInt("Qual key?\n");

            HashNode *result = hashTableSearch(tables[active].table, key);

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

        case 8:
        {
            HashTable *table = tables[active].table;

            float loadFactor = (float)table->count / table->size;

            if (loadFactor < 0.75)
            {
                printf("O load factor eh: %.2f\n\n", loadFactor);

                int option = utilsReadInt("A tabela nao esta muito cheia, deseja continuar mesmo assim?\n[1] Sim\n[2] Nao\n");

                if (option == 2)
                {
                    break;
                }
            }

            int newSize = utilsReadInt("Qual sera o novo tamanho?\n");

            if (newSize <= 0)
            {
                printf("Tamanho invalido!\n");
                break;
            }

            if (!hashTableRehash(table, newSize))
            {
                printf("Erro ao alocar memoria\n");
                break;
            }

            loadFactor = (float)table->count / table->size;

            printf("ReHash feito\n");
            printf("Novo Load Factor: %.2f\n", loadFactor);

            break;
        }

        case 9:
        {
            int newSize = utilsReadInt("Qual sera o novo tamanho?\n");

            if (newSize <= 0)
            {
                printf("Tamanho invalido!\n");
                break;
            }

            HashTable *newTable = hashTableRehashRealloc(tables[active].table, newSize);

            if (newTable == NULL)
            {
                printf("Erro ao alocar memoria\n");
                break;
            }

            tables[active].table = newTable;

            printf("ReHash feito (tabela recriada do zero)\n");

            break;
        }

        case 10:
        {
            listHashTableInstances();

            int firstNumber = utilsReadInt("Primeira hash table:\n");
            int secondNumber = utilsReadInt("Segunda hash table:\n");

            if (firstNumber < 1 || firstNumber > totalTables || secondNumber < 1 || secondNumber > totalTables)
            {
                printf("Hash Table invalida!\n");
                break;
            }

            hashTableCompare(tables[firstNumber - 1].table, tables[secondNumber - 1].table);
            utilsPause();

            break;
        }

        case 11:
            hashTableClearElements(tables[active].table);

            printf("Elementos removidos!\n");

            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }
}