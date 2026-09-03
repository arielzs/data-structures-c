#include <stdio.h>
#include <stdlib.h>

// Verifica BST
// Comparações entre duas (copia, igual, inverter)
// AVL

typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(int data)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Erro ao alocar memoria\n");
        return NULL;
    }

    newNode->data = data;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

void preOrder(Node *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(Node *root)
{
    if (root != NULL)
    {
        inOrder(root->left);
        printf("%d ", root->data);
        inOrder(root->right);
    }
}

void postOrder(Node *root)
{
    if (root != NULL)
    {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->data);
    }
}

void clearTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    clearTree(root->left);
    clearTree(root->right);
    free(root);
}

Node *insertNode(Node *root, int data)
{
    if (root == NULL)
    {
        return createNode(data);
    }

    if (data < root->data)
    {
        root->left = insertNode(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = insertNode(root->right, data);
    }
    else
    {
        printf("Valor Repetido\n");
        return root;
    }

    return root;
}

Node *removeNode(Node *root, int data)
{
    if (root == NULL)
    {
        printf("Elemento nao encontrado\n");
        return NULL;
    }

    if (data < root->data)
    {
        root->left = removeNode(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = removeNode(root->right, data);
    }
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }
        else if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            Node *successor = root->right;

            while (successor->left != NULL)
            {
                successor = successor->left;
            }

            root->data = successor->data;

            root->right = removeNode(root->right, successor->data);
        }
    }

    return root;
}

int height(Node *root)
{
    if (root == NULL)
    {
        return -1;
    }

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    if (leftHeight > rightHeight)
    {
        return leftHeight + 1;
    }
    else
    {
        return rightHeight + 1;
    }
}

int countNodes(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }

    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }

    if (root->left == NULL && root->right == NULL)
    {
        return 1;
    }

    return countLeaves(root->left) + countLeaves(root->right);
}

Node *search(Node *root, int data)
{
    if (root == NULL || root->data == data)
    {
        return root;
    }

    if (data > root->data)
    {
        return search(root->right, data);
    }

    return search(root->left, data);
}

int searchLevel(Node *root, int data, int level)
{
    if (root == NULL)
    {
        return -1;
    }

    if (root->data == data)
    {
        return level;
    }

    if (data > root->data)
    {
        return searchLevel(root->right, data, level + 1);
    }

    return searchLevel(root->left, data, level + 1);
}

int main()
{
    int command, data;

    Node *root1 = NULL;
    Node *root2 = NULL;

    Node **activeRoot = &root1;
    int activeNum = 1;

    while (1)
    {
        printf("\n=== MENU | Arvore ativa: %d ===\n", activeNum);
        printf("[0] Encerra o programa\n");
        printf("[1] Imprime em Ordem\n");
        printf("[2] Insere elemento\n");
        printf("[3] Remove elemento\n");
        printf("[4] Busca um elemento\n");
        printf("[5] Altura da Arvore\n");
        printf("[6] Nodes da Arvore\n");
        printf("[7] Folhas da Arvore\n");
        printf("[8] Nivel de um Node\n");
        printf("[9] Troca de Arvore\n");

        scanf("%d", &command);

        switch (command)
        {
        case 0:
            clearTree(root1);
            clearTree(root2);
            return 0;

        case 1:
            if (*activeRoot == NULL)
            {
                printf("Arvore Vazia\n");
            }
            else
            {
                inOrder(*activeRoot);
                printf("\n");
            }

            break;

        case 2:
            printf("Qual elemento?\n");

            scanf("%d", &data);

            *activeRoot = insertNode(*activeRoot, data);

            break;

        case 3:
            printf("Qual elemento?\n");

            scanf("%d", &data);

            *activeRoot = removeNode(*activeRoot, data);

            break;

        case 4:
        {
            printf("Qual elemento?\n");

            scanf("%d", &data);

            Node *result = search(*activeRoot, data);

            if (result != NULL)
            {
                printf("Elemento encontrado!\n");
            }
            else
            {
                printf("Elemento nao encontrado!\n");
            }

            break;
        }

        case 5:
        {
            int treeHeight = height(*activeRoot);

            printf("A altura da arvore eh: %d\n", treeHeight);

            break;
        }

        case 6:
        {
            int treeNodes = countNodes(*activeRoot);

            printf("A arvore tem %d nodes\n", treeNodes);

            break;
        }

        case 7:
        {
            int treeLeaves = countLeaves(*activeRoot);

            printf("A arvore tem %d folhas\n", treeLeaves);

            break;
        }

        case 8:
            printf("Qual Node?\n");

            scanf("%d", &data);

            int level = searchLevel(*activeRoot, data, 0);

            if (level == -1)
            {
                printf("Valor nao encontrado\n");
            }
            else
            {
                printf("Nivel: %d\n", level);
            }

            break;

        case 9:
            if (activeNum == 1)
            {
                activeRoot = &root2;
                activeNum = 2;
            }
            else
            {
                activeRoot = &root1;
                activeNum = 1;
            }

            printf("Arvore ativa: %d\n", activeNum);

            break;

        default:
            printf("Insira um comando valido!\n");
            break;
        }
    }

    return 0;
}