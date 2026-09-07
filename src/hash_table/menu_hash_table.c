#include "common.h"
#include "utils.h"
#include "hash_table.h"

typedef enum
{
    HASH_CHAINED,
    HASH_DOUBLE
} HashType;

typedef struct
{
    int id;
    HashType type;
    void *table;
} HashTableInstance;

static HashTableInstance *tables = NULL;
static int totalTables = 0;
static int tablesCapacity = 0;
static int active = -1; /* indice em 'tables', -1 = nenhuma hash table ativa */

static const char *hashTypeName(HashType type)
{
    if (type == HASH_CHAINED)
    {
        return "Encadeamento";
    }

    return "Hashing Duplo";
}

static float activeLoadFactor()
{
    if (tables[active].type == HASH_CHAINED)
    {
        ChainedHashTable *table = (ChainedHashTable *)tables[active].table;

        return (float)table->count / table->size;
    }

    DoubleHashTable *table = (DoubleHashTable *)tables[active].table;

    return (float)table->count / table->size;
}

static void createHashTableInstance(HashType type)
{
    int size = utilsReadInt("Qual o tamanho da tabela?\n");

    if (size <= 0)
    {
        printf("Tamanho invalido!\n");
        return;
    }

    void *table = NULL;

    if (type == HASH_CHAINED)
    {
        table = chainedHashTableCreate(size);
    }
    else
    {
        table = doubleHashTableCreate(size);
    }

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
    tables[totalTables].type = type;
    tables[totalTables].table = table;

    active = totalTables;
    totalTables++;

    printf("Hash Table %d (%s) criada e definida como ativa!\n", tables[active].id, hashTypeName(type));

    if (type == HASH_DOUBLE)
    {
        DoubleHashTable *dht = (DoubleHashTable *)tables[active].table;

        printf("(tamanho ajustado para o proximo primo: %d)\n", dht->size);
    }
}

static void listHashTableInstances()
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
            printf("Hash Table %d - %s (ativa)\n", tables[i].id, hashTypeName(tables[i].type));
        }
        else
        {
            printf("Hash Table %d - %s\n", tables[i].id, hashTypeName(tables[i].type));
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
    printf("Hash Table ativa agora: %d (%s)\n", tables[active].id, hashTypeName(tables[active].type));
}

void runHashTableMenu()
{
    int command, key;
    int running = 1;

    while (running)
    {
        printf("\n=== MENU HASH TABLES");

        if (active != -1)
        {
            printf(" | Hash Table ativa: %d (%s)", tables[active].id, hashTypeName(tables[active].type));
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
        printf("[8] ReHash manual (reaproveita a tabela)\n");
        printf("[9] ReHash manual (recria a tabela do zero)\n");
        printf("[10] Compara duas tabelas (mesmo tipo)\n");
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
        {
            int typeChoice = utilsReadInt("Qual tipo de hash table?\n[1] Encadeamento (chaining)\n[2] Enderecamento aberto (hashing duplo)\n");

            if (typeChoice == 1)
            {
                createHashTableInstance(HASH_CHAINED);
            }
            else if (typeChoice == 2)
            {
                createHashTableInstance(HASH_DOUBLE);
            }
            else
            {
                printf("Tipo invalido!\n");
            }

            break;
        }

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
            if (tables[active].type == HASH_CHAINED)
            {
                chainedHashTablePrint((ChainedHashTable *)tables[active].table);
            }
            else
            {
                doubleHashTablePrint((DoubleHashTable *)tables[active].table);
            }

            utilsPause();

            break;

        case 5:
            key = utilsReadInt("Qual key?\n");

            if (tables[active].type == HASH_CHAINED)
            {
                chainedHashTableInsert((ChainedHashTable *)tables[active].table, key);
            }
            else
            {
                doubleHashTableInsert((DoubleHashTable *)tables[active].table, key);
            }

            printf("Key inserida\n");

            break;

        case 6:
        {
            key = utilsReadInt("Qual key?\n");

            int result;

            if (tables[active].type == HASH_CHAINED)
            {
                result = chainedHashTableRemove((ChainedHashTable *)tables[active].table, key);
            }
            else
            {
                result = doubleHashTableRemove((DoubleHashTable *)tables[active].table, key);
            }

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

            int found;

            if (tables[active].type == HASH_CHAINED)
            {
                ChainedHashNode *result = chainedHashTableSearch((ChainedHashTable *)tables[active].table, key);

                found = (result != NULL);
            }
            else
            {
                int index = doubleHashTableSearch((DoubleHashTable *)tables[active].table, key);

                found = (index != -1);
            }

            if (found)
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
            float loadFactor = activeLoadFactor();

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

            int ok;

            if (tables[active].type == HASH_CHAINED)
            {
                ok = chainedHashTableRehash((ChainedHashTable *)tables[active].table, newSize);
            }
            else
            {
                ok = doubleHashTableRehash((DoubleHashTable *)tables[active].table, newSize);
            }

            if (!ok)
            {
                printf("Erro ao alocar memoria\n");
                break;
            }

            printf("ReHash feito\n");
            printf("Novo Load Factor: %.2f\n", activeLoadFactor());

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

            if (tables[active].type == HASH_CHAINED)
            {
                ChainedHashTable *newTable = chainedHashTableRehashRealloc((ChainedHashTable *)tables[active].table, newSize);

                if (newTable == NULL)
                {
                    printf("Erro ao alocar memoria\n");
                    break;
                }

                tables[active].table = newTable;
            }
            else
            {
                DoubleHashTable *newTable = doubleHashTableRehashRealloc((DoubleHashTable *)tables[active].table, newSize);

                if (newTable == NULL)
                {
                    printf("Erro ao alocar memoria\n");
                    break;
                }

                tables[active].table = newTable;
            }

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

            HashTableInstance *first = &tables[firstNumber - 1];
            HashTableInstance *second = &tables[secondNumber - 1];

            if (first->type != second->type)
            {
                printf("So da para comparar hash tables do mesmo tipo!\n");
                break;
            }

            if (first->type == HASH_CHAINED)
            {
                chainedHashTableCompare((ChainedHashTable *)first->table, (ChainedHashTable *)second->table);
            }
            else
            {
                doubleHashTableCompare((DoubleHashTable *)first->table, (DoubleHashTable *)second->table);
            }

            utilsPause();

            break;
        }

        case 11:
            if (tables[active].type == HASH_CHAINED)
            {
                chainedHashTableClearElements((ChainedHashTable *)tables[active].table);
            }
            else
            {
                doubleHashTableClearElements((DoubleHashTable *)tables[active].table);
            }

            printf("Elementos removidos!\n");

            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }
}