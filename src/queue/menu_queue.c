#include "common.h"
#include "utils.h"
#include "queue.h"

typedef struct
{
    int id;
    Queue data;
} QueueInstance;

static QueueInstance *queues = NULL;
static int totalQueues = 0;
static int queuesCapacity = 0;
static int active = -1; /* indice em 'queues', -1 = nenhuma fila ativa */

static void createQueueInstance(void)
{
    if (totalQueues == queuesCapacity)
    {
        if (queuesCapacity == 0)
        {
            queuesCapacity = 4;
        }
        else
        {
            queuesCapacity = queuesCapacity * 2;
        }

        queues = realloc(queues, queuesCapacity * sizeof(QueueInstance));

        if (queues == NULL)
        {
            printf("Erro ao alocar memoria\n");
            exit(1);
        }
    }

    queues[totalQueues].id = totalQueues + 1;
    queueCreate(&queues[totalQueues].data);

    active = totalQueues;
    totalQueues++;

    printf("Fila %d criada e definida como ativa!\n", queues[active].id);
}

static void listQueueInstances(void)
{
    if (totalQueues == 0)
    {
        printf("Nenhuma fila criada ainda. Use o comando [1] para criar uma.\n");
        return;
    }

    printf("=== Filas criadas ===\n");

    for (int i = 0; i < totalQueues; i++)
    {
        if (i == active)
        {
            printf("Fila %d (ativa)\n", queues[i].id);
        }
        else
        {
            printf("Fila %d\n", queues[i].id);
        }
    }
}

static void switchQueueInstance(int number)
{
    if (number < 1 || number > totalQueues)
    {
        printf("Fila invalida!\n");
        return;
    }

    active = number - 1;
    printf("Fila ativa agora: %d\n", queues[active].id);
}

/* Pede o numero de outra fila (diferente da ativa) e devolve o indice dela,
   ou -1 se invalida. Usado pelos comandos de copiar/comparar/juntar/intercalar */
static int chooseOtherQueue(const char *prompt)
{
    listQueueInstances();

    int number = utilsReadInt(prompt);

    if (number < 1 || number > totalQueues)
    {
        printf("Fila invalida!\n");
        return -1;
    }

    if (number - 1 == active)
    {
        printf("Escolha uma fila diferente da ativa!\n");
        return -1;
    }

    return number - 1;
}

void runQueueMenu(void)
{
    int command, data, k;
    int running = 1;

    while (running)
    {
        printf("\n=== MENU FILAS");

        if (active != -1)
        {
            printf(" | Fila ativa: %d", queues[active].id);
        }

        printf(" ===\n");

        printf("[0] Voltar ao menu principal\n");
        printf("[1] Cria uma nova fila\n");
        printf("[2] Lista as filas criadas\n");
        printf("[3] Troca a fila ativa\n");
        printf("[4] Mostra a fila atual\n");
        printf("[5] Insere no fim da fila\n");
        printf("[6] Remove do inicio da fila\n");
        printf("[7] Mostra o inicio da fila\n");
        printf("[8] Busca um valor na fila\n");
        printf("[9] Conta ocorrencias de um valor\n");
        printf("[10] Copia a fila ativa (cria uma nova)\n");
        printf("[11] Compara duas filas\n");
        printf("[12] Mostra o elemento do meio\n");
        printf("[13] Remove ocorrencias\n");
        printf("[14] Remove duplicatas\n");
        printf("[15] Insere ordenado\n");
        printf("[16] Ordena a fila (insertion sort)\n");
        printf("[17] Junta filas (copia; a outra continua intacta)\n");
        printf("[18] Junta filas (move; a outra fica vazia)\n");
        printf("[19] Intercala filas\n");
        printf("[20] Rotaciona a fila\n");
        printf("[21] Rotaciona a fila K vezes\n");
        printf("[22] Ordena com Merge Sort\n");
        printf("[23] Verifica a ordenacao\n");

        command = utilsReadInt("Escolha: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("Voce precisa criar uma fila primeiro! (comando 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
            createQueueInstance();
            break;

        case 2:
            listQueueInstances();
            break;

        case 3:
        {
            listQueueInstances();

            int number = utilsReadInt("Qual fila deseja ativar?\n");

            switchQueueInstance(number);
            break;
        }

        case 4:
            queuePrint(&queues[active].data);
            utilsPause();

            break;

        case 5:
            data = utilsReadInt("Qual elemento deve ser adicionado?\n");

            queueEnqueue(&queues[active].data, data);

            printf("Elemento adicionado!\n");

            break;

        case 6:
        {
            int removed = queueDequeue(&queues[active].data);

            if (removed != -1)
            {
                printf("Elemento %d removido!\n", removed);
            }

            break;
        }

        case 7:
            if (queueIsEmpty(&queues[active].data))
            {
                printf("Fila Vazia!\n");
            }
            else
            {
                printf("O inicio eh: %d\n", queuePeek(&queues[active].data));
            }

            break;

        case 8:
        {
            data = utilsReadInt("Qual elemento deve ser procurado?\n");

            int pos = queueContains(&queues[active].data, data);

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

        case 9:
            if (queueIsEmpty(&queues[active].data))
            {
                printf("Fila Vazia!\n");
            }
            else
            {
                data = utilsReadInt("Qual elemento deve ser contado?\n");

                int count = queueCountOccurrences(&queues[active].data, data);

                printf("O elemento %d aparece %d vez(es) na fila\n", data, count);
            }

            break;

        case 10:
        {
            int sourceIndex = active;
            int sourceId = queues[sourceIndex].id;

            createQueueInstance();
            queueCopy(&queues[active].data, &queues[sourceIndex].data);

            printf("Fila %d copiada para a nova fila %d!\n", sourceId, queues[active].id);

            break;
        }

        case 11:
        {
            listQueueInstances();

            int firstNumber = utilsReadInt("Primeira fila:\n");
            int secondNumber = utilsReadInt("Segunda fila:\n");

            if (firstNumber < 1 || firstNumber > totalQueues || secondNumber < 1 || secondNumber > totalQueues)
            {
                printf("Fila invalida!\n");
                break;
            }

            if (queueCompare(&queues[firstNumber - 1].data, &queues[secondNumber - 1].data))
            {
                printf("Filas iguais!\n");
            }
            else
            {
                printf("Filas diferentes!\n");
            }

            break;
        }

        case 12:
        {
            int middle = queueFindMiddle(&queues[active].data);

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

        case 13:
        {
            data = utilsReadInt("Qual ocorrencia?\n");

            int removeds = queueRemoveOccurrences(&queues[active].data, data);

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

        case 14:
            queueRemoveDuplicates(&queues[active].data);

            printf("Duplicatas removidas!\n");

            break;

        case 15:
            data = utilsReadInt("Qual elemento?\n");

            queueInsertSorted(&queues[active].data, data);

            printf("Elemento %d inserido ordenado!\n", data);

            break;

        case 16:
            queueSort(&queues[active].data);

            printf("Fila Ordenada!\n");

            break;

        case 17:
        {
            int otherIndex = chooseOtherQueue("Copiar os elementos de qual fila para a ativa?\n");

            if (otherIndex == -1)
            {
                break;
            }

            queueMergeCopy(&queues[active].data, &queues[otherIndex].data);

            printf("Filas unidas! (a fila de origem continua intacta)\n");

            break;
        }

        case 18:
        {
            int otherIndex = chooseOtherQueue("Mover os elementos de qual fila para a ativa?\n");

            if (otherIndex == -1)
            {
                break;
            }

            queueMergeMove(&queues[active].data, &queues[otherIndex].data);

            printf("Filas unidas! (a fila de origem ficou vazia)\n");

            break;
        }

        case 19:
        {
            int otherIndex = chooseOtherQueue("Intercalar a ativa com qual fila?\n");

            if (otherIndex == -1)
            {
                break;
            }

            queueInterleave(&queues[active].data, &queues[otherIndex].data);

            printf("Filas intercaladas!\n");

            break;
        }

        case 20:
            queueRotate(&queues[active].data);

            printf("Fila Rotacionada!\n");

            break;

        case 21:
            k = utilsReadInt("Quantas vezes?\n");

            queueRotateK(&queues[active].data, k);

            printf("Fila Rotacionada %d vezes!\n", k);

            break;

        case 22:
            queueMergeSort(&queues[active].data);

            printf("Fila Ordenada!\n");

            break;

        case 23:
        {
            int sorted = queueIsSorted(&queues[active].data);

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
}