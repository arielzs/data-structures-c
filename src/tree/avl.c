#include "common.h"
#include "tree.h"

NodeAVL *createNodeAVL(int data)
{
    NodeAVL *newNode = malloc(sizeof(NodeAVL));

    if (newNode == NULL)
    {
        printf("Erro ao alocar memoria\n");
        return NULL;
    }

    newNode->data = data;
    newNode->height = 0;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

int heightAVL(NodeAVL *root)
{
    if (root == NULL)
    {
        return -1;
    }

    return root->height;
}

/* Atualiza a altura de um node com base na altura dos filhos */
static void updateHeight(NodeAVL *root)
{
    int leftHeight = heightAVL(root->left);
    int rightHeight = heightAVL(root->right);

    if (leftHeight > rightHeight)
    {
        root->height = leftHeight + 1;
    }
    else
    {
        root->height = rightHeight + 1;
    }
}

int balanceFactor(NodeAVL *root)
{
    if (root == NULL)
    {
        return 0;
    }

    return heightAVL(root->left) - heightAVL(root->right);
}

NodeAVL *rotateRight(NodeAVL *root)
{
    NodeAVL *newRoot = root->left;
    NodeAVL *transferred = newRoot->right;

    newRoot->right = root;
    root->left = transferred;

    updateHeight(root);
    updateHeight(newRoot);

    return newRoot;
}

NodeAVL *rotateLeft(NodeAVL *root)
{
    NodeAVL *newRoot = root->right;
    NodeAVL *transferred = newRoot->left;

    newRoot->left = root;
    root->right = transferred;

    updateHeight(root);
    updateHeight(newRoot);

    return newRoot;
}

NodeAVL *rotateLeftRight(NodeAVL *root)
{
    root->left = rotateLeft(root->left);
    return rotateRight(root);
}

NodeAVL *rotateRightLeft(NodeAVL *root)
{
    root->right = rotateRight(root->right);
    return rotateLeft(root);
}

NodeAVL *balance(NodeAVL *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    updateHeight(root);

    int factor = balanceFactor(root);

    if (factor > 1)
    {
        if (balanceFactor(root->left) < 0)
        {
            return rotateLeftRight(root);
        }

        return rotateRight(root);
    }

    if (factor < -1)
    {
        if (balanceFactor(root->right) > 0)
        {
            return rotateRightLeft(root);
        }

        return rotateLeft(root);
    }

    return root;
}

NodeAVL *copyTreeAVL(NodeAVL *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    NodeAVL *newNode = createNodeAVL(root->data);

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->left = copyTreeAVL(root->left);
    newNode->right = copyTreeAVL(root->right);
    newNode->height = root->height;

    return newNode;
}

NodeAVL *insertAVL(NodeAVL *root, int data)
{
    if (root == NULL)
    {
        return createNodeAVL(data);
    }

    if (data < root->data)
    {
        root->left = insertAVL(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = insertAVL(root->right, data);
    }
    else
    {
        printf("Valor Repetido\n");
        return root;
    }

    return balance(root);
}

NodeAVL *removeAVL(NodeAVL *root, int data)
{
    if (root == NULL)
    {
        printf("Elemento nao encontrado\n");
        return NULL;
    }

    if (data < root->data)
    {
        root->left = removeAVL(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = removeAVL(root->right, data);
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
            NodeAVL *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            NodeAVL *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            NodeAVL *successor = root->right;

            while (successor->left != NULL)
            {
                successor = successor->left;
            }

            root->data = successor->data;
            root->right = removeAVL(root->right, successor->data);
        }
    }

    return balance(root);
}

/* ===================== Utilitarias (equivalentes as da BST) ===================== */

void preOrderAVL(NodeAVL *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
        preOrderAVL(root->left);
        preOrderAVL(root->right);
    }
}

void inOrderAVL(NodeAVL *root)
{
    if (root != NULL)
    {
        inOrderAVL(root->left);
        printf("%d ", root->data);
        inOrderAVL(root->right);
    }
}

void postOrderAVL(NodeAVL *root)
{
    if (root != NULL)
    {
        postOrderAVL(root->left);
        postOrderAVL(root->right);
        printf("%d ", root->data);
    }
}

void clearTreeAVL(NodeAVL *root)
{
    if (root == NULL)
    {
        return;
    }

    clearTreeAVL(root->left);
    clearTreeAVL(root->right);
    free(root);
}

int countNodesAVL(NodeAVL *root)
{
    if (root == NULL)
    {
        return 0;
    }

    return 1 + countNodesAVL(root->left) + countNodesAVL(root->right);
}

int countLeavesAVL(NodeAVL *root)
{
    if (root == NULL)
    {
        return 0;
    }

    if (root->left == NULL && root->right == NULL)
    {
        return 1;
    }

    return countLeavesAVL(root->left) + countLeavesAVL(root->right);
}

NodeAVL *searchAVL(NodeAVL *root, int data)
{
    if (root == NULL || root->data == data)
    {
        return root;
    }

    if (data > root->data)
    {
        return searchAVL(root->right, data);
    }

    return searchAVL(root->left, data);
}

int searchLevelAVL(NodeAVL *root, int data, int level)
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
        return searchLevelAVL(root->right, data, level + 1);
    }

    return searchLevelAVL(root->left, data, level + 1);
}

int equalTreesAVL(NodeAVL *root1, NodeAVL *root2)
{
    if (root1 == NULL && root2 == NULL)
    {
        return 1;
    }

    if (root1 == NULL || root2 == NULL)
    {
        return 0;
    }

    if (root1->data != root2->data)
    {
        return 0;
    }

    return equalTreesAVL(root1->left, root2->left) && equalTreesAVL(root1->right, root2->right);
}

void invertTreeAVL(NodeAVL *root)
{
    if (root == NULL)
    {
        return;
    }

    NodeAVL *temp = root->left;
    root->left = root->right;
    root->right = temp;

    invertTreeAVL(root->left);
    invertTreeAVL(root->right);
}

int isBSTAVL(NodeAVL *root, int min, int max)
{
    if (root == NULL)
    {
        return 1;
    }

    if (root->data <= min || root->data >= max)
    {
        return 0;
    }

    return isBSTAVL(root->left, min, root->data) && isBSTAVL(root->right, root->data, max);
}