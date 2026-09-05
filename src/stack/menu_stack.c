#include "common.h"
#include "utils.h"
#include "stack.h"

typedef struct
{
    int id;
    Stack data;
} StackInstance;

static StackInstance *stacks = NULL;
static int totalStacks = 0;
static int stacksCapacity = 0;
static int active = -1; /* indice em 'stacks', -1 = nenhuma pilha ativa */

static void createStackInstance(void)
{
    if (totalStacks == stacksCapacity)
    {
        if (stacksCapacity == 0)
        {
            stacksCapacity = 4;
        }
        else
        {
            stacksCapacity = stacksCapacity * 2;
        }

        stacks = realloc(stacks, stacksCapacity * sizeof(StackInstance));

        if (stacks == NULL)
        {
            printf("Erro ao alocar memoria\n");
            exit(1);
        }
    }

    stacks[totalStacks].id = totalStacks + 1;
    stackCreate(&stacks[totalStacks].data);

    active = totalStacks;
    totalStacks++;

    printf("Pilha %d criada e definida como ativa!\n", stacks[active].id);
}

static void listStackInstances(void)
{
    if (totalStacks == 0)
    {
        printf("Nenhuma pilha criada ainda. Use o comando [1] para criar uma.\n");
        return;
    }

    printf("=== Pilhas criadas ===\n");

    for (int i = 0; i < totalStacks; i++)
    {
        if (i == active)
        {
            printf("Pilha %d (ativa)\n", stacks[i].id);
        }
        else
        {
            printf("Pilha %d\n", stacks[i].id);
        }
    }
}

static void switchStackInstance(int number)
{
    if (number < 1 || number > totalStacks)
    {
        printf("Pilha invalida!\n");
        return;
    }

    active = number - 1;
    printf("Pilha ativa agora: %d\n", stacks[active].id);
}

void runStackMenu(void)
{
    int command, data, k;
    int running = 1;

    while (running)
    {
        printf("\n=== MENU PILHAS");

        if (active != -1)
        {
            printf(" | Pilha ativa: %d", stacks[active].id);
        }

        printf(" ===\n");

        printf("[0] Voltar ao menu principal\n");
        printf("[1] Cria uma nova pilha\n");
        printf("[2] Lista as pilhas criadas\n");
        printf("[3] Troca a pilha ativa\n");
        printf("[4] Insere elementos\n");
        printf("[5] Remove elementos\n");
        printf("[6] Remove ocorrencias\n");
        printf("[7] Procura elemento\n");
        printf("[8] Conta ocorrencias de um elemento\n");
        printf("[9] Inverte a pilha ativa\n");
        printf("[10] Mostra a pilha atual\n");
        printf("[11] Copia a pilha ativa (cria uma nova)\n");
        printf("[12] Compara duas pilhas\n");
        printf("[13] Junta o topo de duas pilhas\n");
        printf("[14] Junta o fim de duas pilhas\n");
        printf("[15] Ordena a pilha (insertion sort)\n");
        printf("[16] Troca o topo (dois primeiros elementos)\n");
        printf("[17] Move o topo K posicoes para baixo\n");
        printf("[18] Rotaciona a pilha\n");
        printf("[19] Rotaciona a pilha K vezes\n");
        printf("[20] Ordena com Merge Sort\n");
        printf("[21] Remove o elemento do meio\n");
        printf("[22] Verifica se uma expressao esta balanceada\n");

        command = utilsReadInt("Escolha: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("Voce precisa criar uma pilha primeiro! (comando 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
            createStackInstance();
            break;

        case 2:
            listStackInstances();
            break;

        case 3:
        {
            listStackInstances();

            int number = utilsReadInt("Qual pilha deseja ativar?\n");

            switchStackInstance(number);
            break;
        }

        case 4:
        {
            int quantity = utilsReadInt("Quantos elementos devem ser adicionados?\n");

            if (quantity <= 0)
            {
                printf("Quantidade invalida!\n");
                break;
            }

            for (int i = 0; i < quantity; i++)
            {
                char prompt[32];
                snprintf(prompt, sizeof(prompt), "Elemento %d:\n", i + 1);

                data = utilsReadInt(prompt);

                stackPush(&stacks[active].data, data);
            }

            printf("%d Elemento(s) adicionado(s)!\n", quantity);

            break;
        }

        case 5:
            if (stackIsEmpty(&stacks[active].data))
            {
                printf("Pilha vazia!\n");
            }
            else
            {
                int quantity = utilsReadInt("Quantos elementos devem ser removidos?\n");

                stackPopMultiple(&stacks[active].data, quantity);
            }

            break;

        case 6:
            data = utilsReadInt("Qual ocorrencia deve ser removida?\n");

            stackRemoveOccurrences(&stacks[active].data, data);

            break;

        case 7:
            data = utilsReadInt("Qual elemento deve ser buscado?\n");

            stackSearch(&stacks[active].data, data);

            break;

        case 8:
        {
            data = utilsReadInt("Qual elemento deve ser contado?\n");

            int count = stackCountOccurrences(&stacks[active].data, data);

            printf("O elemento %d aparece %d vez(es)!\n", data, count);

            break;
        }

        case 9:
            if (stackReverse(&stacks[active].data))
            {
                printf("Pilha invertida!\n");
            }
            else
            {
                printf("Pilha vazia!\n");
            }

            break;

        case 10:
            stackPrint(&stacks[active].data);
            utilsPause();

            break;

        case 11:
        {
            int sourceIndex = active;
            int sourceId = stacks[sourceIndex].id;

            createStackInstance();
            stackCopy(&stacks[active].data, &stacks[sourceIndex].data);

            printf("Pilha %d copiada para a nova pilha %d!\n", sourceId, stacks[active].id);

            break;
        }

        case 12:
        {
            listStackInstances();

            int firstNumber = utilsReadInt("Primeira pilha:\n");
            int secondNumber = utilsReadInt("Segunda pilha:\n");

            if (firstNumber < 1 || firstNumber > totalStacks || secondNumber < 1 || secondNumber > totalStacks)
            {
                printf("Pilha invalida!\n");
                break;
            }

            if (stackCompare(&stacks[firstNumber - 1].data, &stacks[secondNumber - 1].data))
            {
                printf("Pilhas iguais!\n");
            }
            else
            {
                printf("Pilhas diferentes!\n");
            }

            break;
        }

        case 13:
        {
            listStackInstances();

            int otherNumber = utilsReadInt("Juntar com qual pilha (o topo dela entra no topo da ativa)?\n");

            if (otherNumber < 1 || otherNumber > totalStacks)
            {
                printf("Pilha invalida!\n");
                break;
            }

            stackMergeTop(&stacks[active].data, &stacks[otherNumber - 1].data);

            printf("Pilhas unidas!\n");

            break;
        }

        case 14:
        {
            listStackInstances();

            int otherNumber = utilsReadInt("Juntar com qual pilha (ela vai para o fundo da ativa)?\n");

            if (otherNumber < 1 || otherNumber > totalStacks)
            {
                printf("Pilha invalida!\n");
                break;
            }

            stackMergeBottom(&stacks[active].data, &stacks[otherNumber - 1].data);

            printf("Pilhas unidas!\n");

            break;
        }

        case 15:
            stackSort(&stacks[active].data);

            printf("Pilha ordenada!\n");

            break;

        case 16:
            stackSwapTop(&stacks[active].data);

            printf("Topo trocado\n");

            break;

        case 17:
            k = utilsReadInt("Quantas posicoes?\n");

            stackMoveTopDown(&stacks[active].data, k);

            printf("Topo movido %d posicoes\n", k);

            break;

        case 18:
            stackRotate(&stacks[active].data);

            printf("Pilha rotacionada\n");

            break;

        case 19:
            k = utilsReadInt("Quantas vezes?\n");

            stackRotateK(&stacks[active].data, k);

            printf("Pilha rotacionada %d vezes\n", k);

            break;

        case 20:
            stackMergeSort(&stacks[active].data);

            printf("Pilha ordenada!\n");

            break;

        case 21:
            stackRemoveMiddle(&stacks[active].data);

            printf("Elemento do meio removido\n");

            break;

        case 22:
        {
            char expr[100];

            utilsReadLine("Digite a expressao: ", expr, sizeof(expr));

            if (stackVerifyBalance(expr))
            {
                printf("Expressao valida!\n");
            }
            else
            {
                printf("Expressao invalida!\n");
            }

            break;
        }

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }
}