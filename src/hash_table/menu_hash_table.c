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
        return "Chaining";
    }

    return "Double Hashing";
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
    int size = utilsReadInt("Table size?\n");

    if (size <= 0)
    {
        printf("Invalid size!\n");
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
        printf("Error creating the hash table\n");
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
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    tables[totalTables].id = totalTables + 1;
    tables[totalTables].type = type;
    tables[totalTables].table = table;

    active = totalTables;
    totalTables++;

    printf("Hash table %d (%s) created and set as active!\n", tables[active].id, hashTypeName(type));

    if (type == HASH_DOUBLE)
    {
        DoubleHashTable *dht = (DoubleHashTable *)tables[active].table;

        printf("(size adjusted to the next prime: %d)\n", dht->size);
    }
}

static void listHashTableInstances()
{
    if (totalTables == 0)
    {
        printf("No hash tables created yet. Use command [1] to create one.\n");
        return;
    }

    printf("=== Created hash tables ===\n");

    for (int i = 0; i < totalTables; i++)
    {
        if (i == active)
        {
            printf("Hash table %d - %s (active)\n", tables[i].id, hashTypeName(tables[i].type));
        }
        else
        {
            printf("Hash table %d - %s\n", tables[i].id, hashTypeName(tables[i].type));
        }
    }
}

static void switchHashTableInstance(int number)
{
    if (number < 1 || number > totalTables)
    {
        printf("Invalid hash table!\n");
        return;
    }

    active = number - 1;
    printf("Active hash table is now: %d (%s)\n", tables[active].id, hashTypeName(tables[active].type));
}

void runHashTableMenu()
{
    int command, key;
    int running = 1;

    while (running)
    {
        printf("\n=== HASH TABLES MENU");

        if (active != -1)
        {
            printf(" | Active hash table: %d (%s)", tables[active].id, hashTypeName(tables[active].type));
        }

        printf(" ===\n");

        printf("[0] Back to main menu\n");
        printf("[1] Create a new hash table\n");
        printf("[2] List the created hash tables\n");
        printf("[3] Switch the active hash table\n");
        printf("[4] Show the current table\n");
        printf("[5] Insert element\n");
        printf("[6] Remove element\n");
        printf("[7] Search for an element\n");
        printf("[8] Manual rehash (reuses the table)\n");
        printf("[9] Manual rehash (rebuilds the table from scratch)\n");
        printf("[10] Compare two tables (same type)\n");
        printf("[11] Clear the elements of the active table\n");

        command = utilsReadInt("Choice: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("You need to create a hash table first! (command 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
        {
            int typeChoice = utilsReadInt("Which type of hash table?\n[1] Chaining\n[2] Open addressing (double hashing)\n");

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
                printf("Invalid type!\n");
            }

            break;
        }

        case 2:
            listHashTableInstances();
            break;

        case 3:
        {
            listHashTableInstances();

            int number = utilsReadInt("Which hash table do you want to activate?\n");

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
            key = utilsReadInt("Which key?\n");

            if (tables[active].type == HASH_CHAINED)
            {
                chainedHashTableInsert((ChainedHashTable *)tables[active].table, key);
            }
            else
            {
                doubleHashTableInsert((DoubleHashTable *)tables[active].table, key);
            }

            printf("Key inserted\n");

            break;

        case 6:
        {
            key = utilsReadInt("Which key?\n");

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
                printf("Key removed\n");
            }
            else
            {
                printf("Key not found\n");
            }

            break;
        }

        case 7:
        {
            key = utilsReadInt("Which key?\n");

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
                printf("Key found\n");
            }
            else
            {
                printf("Key not found\n");
            }

            break;
        }

        case 8:
        {
            float loadFactor = activeLoadFactor();

            if (loadFactor < 0.75)
            {
                printf("The load factor is: %.2f\n\n", loadFactor);

                int option = utilsReadInt("The table is not very full, continue anyway?\n[1] Yes\n[2] No\n");

                if (option == 2)
                {
                    break;
                }
            }

            int newSize = utilsReadInt("New size?\n");

            if (newSize <= 0)
            {
                printf("Invalid size!\n");
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
                printf("Memory allocation failed\n");
                break;
            }

            printf("Rehash done\n");
            printf("New load factor: %.2f\n", activeLoadFactor());

            break;
        }

        case 9:
        {
            int newSize = utilsReadInt("New size?\n");

            if (newSize <= 0)
            {
                printf("Invalid size!\n");
                break;
            }

            if (tables[active].type == HASH_CHAINED)
            {
                ChainedHashTable *newTable = chainedHashTableRehashRealloc((ChainedHashTable *)tables[active].table, newSize);

                if (newTable == NULL)
                {
                    printf("Memory allocation failed\n");
                    break;
                }

                tables[active].table = newTable;
            }
            else
            {
                DoubleHashTable *newTable = doubleHashTableRehashRealloc((DoubleHashTable *)tables[active].table, newSize);

                if (newTable == NULL)
                {
                    printf("Memory allocation failed\n");
                    break;
                }

                tables[active].table = newTable;
            }

            printf("Rehash done (table rebuilt from scratch)\n");

            break;
        }

        case 10:
        {
            listHashTableInstances();

            int firstNumber = utilsReadInt("First hash table:\n");
            int secondNumber = utilsReadInt("Second hash table:\n");

            if (firstNumber < 1 || firstNumber > totalTables || secondNumber < 1 || secondNumber > totalTables)
            {
                printf("Invalid hash table!\n");
                break;
            }

            HashTableInstance *first = &tables[firstNumber - 1];
            HashTableInstance *second = &tables[secondNumber - 1];

            if (first->type != second->type)
            {
                printf("Only hash tables of the same type can be compared!\n");
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

            printf("Elements removed!\n");

            break;

        default:
            printf("Enter a valid command!\n");
            break;
        }
    }
}