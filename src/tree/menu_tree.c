#include "common.h"
#include "utils.h"
#include "tree.h"

typedef enum
{
    TREE_BST,
    TREE_AVL
} TreeType;

typedef struct
{
    int id;
    TreeType type;
    void *root;
} Tree;

static Tree *trees = NULL;
static int totalTrees = 0;
static int treesCapacity = 0;
static int active = -1; /* indice em 'trees', -1 = nenhuma arvore ativa */

static const char *typeName(TreeType type)
{
    if (type == TREE_BST)
    {
        return "BST";
    }

    return "AVL";
}

static void createTree(TreeType type)
{
    if (totalTrees == treesCapacity)
    {
        if (treesCapacity == 0)
        {
            treesCapacity = 4;
        }
        else
        {
            treesCapacity = treesCapacity * 2;
        }

        trees = realloc(trees, treesCapacity * sizeof(Tree));

        if (trees == NULL)
        {
            printf("Erro ao alocar memoria\n");
            exit(1);
        }
    }

    trees[totalTrees].id = totalTrees + 1;
    trees[totalTrees].type = type;
    trees[totalTrees].root = NULL;

    active = totalTrees;
    totalTrees++;

    printf("Arvore %d (%s) criada e definida como ativa!\n", trees[active].id, typeName(type));
}

static void listTrees(void)
{
    if (totalTrees == 0)
    {
        printf("Nenhuma arvore criada ainda. Use o comando [1] para criar uma.\n");
        return;
    }

    printf("=== Arvores criadas ===\n");

    for (int i = 0; i < totalTrees; i++)
    {
        if (i == active)
        {
            printf("Arvore %d - %s (ativa)\n", trees[i].id, typeName(trees[i].type));
        }
        else
        {
            printf("Arvore %d - %s\n", trees[i].id, typeName(trees[i].type));
        }
    }
}

static void switchTree(int number)
{
    if (number < 1 || number > totalTrees)
    {
        printf("Arvore invalida!\n");
        return;
    }

    active = number - 1;
    printf("Arvore ativa agora: %d (%s)\n", trees[active].id, typeName(trees[active].type));
}

void runTreeMenu(void)
{
    int command, data;
    int running = 1;

    while (running)
    {
        printf("\n=== MENU ARVORES");

        if (active != -1)
        {
            printf(" | Arvore ativa: %d (%s)", trees[active].id, typeName(trees[active].type));
        }

        printf(" ===\n");

        printf("[0] Voltar ao menu principal\n");
        printf("[1] Cria uma nova arvore\n");
        printf("[2] Lista as arvores criadas\n");
        printf("[3] Troca a arvore ativa\n");
        printf("[4] Imprime em Ordem\n");
        printf("[5] Imprime em Pre-Ordem\n");
        printf("[6] Imprime em Pos-Ordem\n");
        printf("[7] Insere elemento\n");
        printf("[8] Remove elemento\n");
        printf("[9] Busca um elemento\n");
        printf("[10] Altura da Arvore\n");
        printf("[11] Nodes da Arvore\n");
        printf("[12] Folhas da Arvore\n");
        printf("[13] Nivel de um Node\n");
        printf("[14] Copia a arvore ativa (cria uma nova)\n");
        printf("[15] Compara duas arvores\n");
        printf("[16] Inverte arvore ativa\n");
        printf("[17] Verifica se eh uma BST valida\n");
        printf("--- Comandos exclusivos da AVL (testar balanceamento) ---\n");
        printf("[18] Mostra o fator de balanceamento da raiz\n");
        printf("[19] Rotaciona a raiz para a direita\n");
        printf("[20] Rotaciona a raiz para a esquerda\n");
        printf("[21] Rotacao dupla Esquerda-Direita na raiz\n");
        printf("[22] Rotacao dupla Direita-Esquerda na raiz\n");
        printf("[23] Forca o balanceamento da raiz\n");

        command = utilsReadInt("Escolha: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("Voce precisa criar uma arvore primeiro! (comando 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
        {
            int typeChoice = utilsReadInt("Qual tipo de arvore?\n[1] BST\n[2] AVL\n");

            if (typeChoice == 1)
            {
                createTree(TREE_BST);
            }
            else if (typeChoice == 2)
            {
                createTree(TREE_AVL);
            }
            else
            {
                printf("Tipo invalido!\n");
            }

            break;
        }

        case 2:
            listTrees();
            break;

        case 3:
        {
            listTrees();

            int number = utilsReadInt("Qual arvore deseja ativar?\n");

            switchTree(number);
            break;
        }

        case 4:
            if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else if (trees[active].type == TREE_BST)
            {
                inOrder((BSTNode *)trees[active].root);
                printf("\n");
            }
            else
            {
                inOrderAVL((AVLNode *)trees[active].root);
                printf("\n");
            }
            break;

        case 5:
            if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else if (trees[active].type == TREE_BST)
            {
                preOrder((BSTNode *)trees[active].root);
                printf("\n");
            }
            else
            {
                preOrderAVL((AVLNode *)trees[active].root);
                printf("\n");
            }
            break;

        case 6:
            if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else if (trees[active].type == TREE_BST)
            {
                postOrder((BSTNode *)trees[active].root);
                printf("\n");
            }
            else
            {
                postOrderAVL((AVLNode *)trees[active].root);
                printf("\n");
            }
            break;

        case 7:
            data = utilsReadInt("Qual elemento?\n");

            if (trees[active].type == TREE_BST)
            {
                trees[active].root = insertNode((BSTNode *)trees[active].root, data);
            }
            else
            {
                trees[active].root = insertAVL((AVLNode *)trees[active].root, data);
            }
            break;

        case 8:
            data = utilsReadInt("Qual elemento?\n");

            if (trees[active].type == TREE_BST)
            {
                trees[active].root = removeNode((BSTNode *)trees[active].root, data);
            }
            else
            {
                trees[active].root = removeAVL((AVLNode *)trees[active].root, data);
            }
            break;

        case 9:
        {
            data = utilsReadInt("Qual elemento?\n");

            int found;

            if (trees[active].type == TREE_BST)
            {
                found = search((BSTNode *)trees[active].root, data) != NULL;
            }
            else
            {
                found = searchAVL((AVLNode *)trees[active].root, data) != NULL;
            }

            if (found)
            {
                printf("Elemento encontrado!\n");
            }
            else
            {
                printf("Elemento nao encontrado!\n");
            }

            break;
        }

        case 10:
        {
            int treeHeight;

            if (trees[active].type == TREE_BST)
            {
                treeHeight = height((BSTNode *)trees[active].root);
            }
            else
            {
                treeHeight = heightAVL((AVLNode *)trees[active].root);
            }

            printf("A altura da arvore eh: %d\n", treeHeight);

            break;
        }

        case 11:
        {
            int totalNodes;

            if (trees[active].type == TREE_BST)
            {
                totalNodes = countNodes((BSTNode *)trees[active].root);
            }
            else
            {
                totalNodes = countNodesAVL((AVLNode *)trees[active].root);
            }

            printf("A arvore tem %d nodes\n", totalNodes);

            break;
        }

        case 12:
        {
            int totalLeaves;

            if (trees[active].type == TREE_BST)
            {
                totalLeaves = countLeaves((BSTNode *)trees[active].root);
            }
            else
            {
                totalLeaves = countLeavesAVL((AVLNode *)trees[active].root);
            }

            printf("A arvore tem %d folhas\n", totalLeaves);

            break;
        }

        case 13:
        {
            data = utilsReadInt("Qual BSTNode?\n");

            int level;

            if (trees[active].type == TREE_BST)
            {
                level = searchLevel((BSTNode *)trees[active].root, data, 0);
            }
            else
            {
                level = searchLevelAVL((AVLNode *)trees[active].root, data, 0);
            }

            if (level == -1)
            {
                printf("Valor nao encontrado\n");
            }
            else
            {
                printf("Nivel: %d\n", level);
            }

            break;
        }

        case 14:
        {
            int sourceIndex = active;
            TreeType type = trees[sourceIndex].type;
            int sourceId = trees[sourceIndex].id;
            void *copy;

            if (type == TREE_BST)
            {
                copy = copyTree((BSTNode *)trees[sourceIndex].root);
            }
            else
            {
                copy = copyTreeAVL((AVLNode *)trees[sourceIndex].root);
            }

            createTree(type);
            trees[active].root = copy;

            printf("Arvore %d copiada para a nova arvore %d!\n", sourceId, trees[active].id);

            break;
        }

        case 15:
        {
            listTrees();

            int firstNumber = utilsReadInt("Primeira arvore:\n");
            int secondNumber = utilsReadInt("Segunda arvore:\n");

            if (firstNumber < 1 || firstNumber > totalTrees || secondNumber < 1 || secondNumber > totalTrees)
            {
                printf("Arvore invalida!\n");
                break;
            }

            Tree *treeA = &trees[firstNumber - 1];
            Tree *treeB = &trees[secondNumber - 1];

            if (treeA->type != treeB->type)
            {
                printf("Arvores de tipos diferentes (BST e AVL) sao sempre consideradas diferentes!\n");
                break;
            }

            int equal;

            if (treeA->type == TREE_BST)
            {
                equal = equalTrees((BSTNode *)treeA->root, (BSTNode *)treeB->root);
            }
            else
            {
                equal = equalTreesAVL((AVLNode *)treeA->root, (AVLNode *)treeB->root);
            }

            if (equal)
            {
                printf("As arvores sao iguais!\n");
            }
            else
            {
                printf("As arvores sao diferentes!\n");
            }

            break;
        }

        case 16:
            if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else if (trees[active].type == TREE_BST)
            {
                invertTree((BSTNode *)trees[active].root);
                printf("Arvore invertida!\n");
            }
            else
            {
                invertTreeAVL((AVLNode *)trees[active].root);
                printf("Arvore invertida!\n");
            }
            break;

        case 17:
        {
            int valid;

            if (trees[active].type == TREE_BST)
            {
                valid = isBST((BSTNode *)trees[active].root, INT_MIN, INT_MAX);
            }
            else
            {
                valid = isBSTAVL((AVLNode *)trees[active].root, INT_MIN, INT_MAX);
            }

            if (valid)
            {
                printf("A arvore eh uma BST valida!\n");
            }
            else
            {
                printf("A arvore nao eh uma BST valida!\n");
            }

            break;
        }

        case 18:
            if (trees[active].type != TREE_AVL)
            {
                printf("Esse comando so funciona em arvores AVL!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else
            {
                int factor = balanceFactor((AVLNode *)trees[active].root);
                printf("Fator de balanceamento da raiz: %d\n", factor);
            }
            break;

        case 19:
            if (trees[active].type != TREE_AVL)
            {
                printf("Esse comando so funciona em arvores AVL!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else if (((AVLNode *)trees[active].root)->left == NULL)
            {
                printf("Nao eh possivel rotacionar para a direita: a raiz nao tem filho a esquerda!\n");
            }
            else
            {
                trees[active].root = rotateRight((AVLNode *)trees[active].root);
                printf("Rotacao a direita aplicada na raiz!\n");
            }
            break;

        case 20:
            if (trees[active].type != TREE_AVL)
            {
                printf("Esse comando so funciona em arvores AVL!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else if (((AVLNode *)trees[active].root)->right == NULL)
            {
                printf("Nao eh possivel rotacionar para a esquerda: a raiz nao tem filho a direita!\n");
            }
            else
            {
                trees[active].root = rotateLeft((AVLNode *)trees[active].root);
                printf("Rotacao a esquerda aplicada na raiz!\n");
            }
            break;

        case 21:
            if (trees[active].type != TREE_AVL)
            {
                printf("Esse comando so funciona em arvores AVL!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else if (((AVLNode *)trees[active].root)->left == NULL || ((AVLNode *)trees[active].root)->left->right == NULL)
            {
                printf("Nao eh possivel fazer a rotacao dupla Esquerda-Direita nessa configuracao!\n");
            }
            else
            {
                trees[active].root = rotateLeftRight((AVLNode *)trees[active].root);
                printf("Rotacao dupla Esquerda-Direita aplicada na raiz!\n");
            }
            break;

        case 22:
            if (trees[active].type != TREE_AVL)
            {
                printf("Esse comando so funciona em arvores AVL!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else if (((AVLNode *)trees[active].root)->right == NULL || ((AVLNode *)trees[active].root)->right->left == NULL)
            {
                printf("Nao eh possivel fazer a rotacao dupla Direita-Esquerda nessa configuracao!\n");
            }
            else
            {
                trees[active].root = rotateRightLeft((AVLNode *)trees[active].root);
                printf("Rotacao dupla Direita-Esquerda aplicada na raiz!\n");
            }
            break;

        case 23:
            if (trees[active].type != TREE_AVL)
            {
                printf("Esse comando so funciona em arvores AVL!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else
            {
                trees[active].root = balance((AVLNode *)trees[active].root);
                printf("Balanceamento forcado na raiz!\n");
            }
            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }
}