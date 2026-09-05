#include "common.h"
#include "tree.h"

AVLNode *createNodeAVL(int data)
{
    AVLNode *newNode = malloc(sizeof(AVLNode));

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

int heightAVL(AVLNode *root)
{
    if (root == NULL)
    {
        return -1;
    }

    return root->height;
}

/* Atualiza a altura de um node com base na altura dos filhos */
static void updateHeight(AVLNode *root)
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

int balanceFactor(AVLNode *root)
{
    if (root == NULL)
    {
        return 0;
    }

    return heightAVL(root->left) - heightAVL(root->right);
}

AVLNode *rotateRight(AVLNode *root)
{
    AVLNode *newRoot = root->left;
    AVLNode *transferred = newRoot->right;

    newRoot->right = root;
    root->left = transferred;

    updateHeight(root);
    updateHeight(newRoot);

    return newRoot;
}

AVLNode *rotateLeft(AVLNode *root)
{
    AVLNode *newRoot = root->right;
    AVLNode *transferred = newRoot->left;

    newRoot->left = root;
    root->right = transferred;

    updateHeight(root);
    updateHeight(newRoot);

    return newRoot;
}

AVLNode *rotateLeftRight(AVLNode *root)
{
    root->left = rotateLeft(root->left);
    return rotateRight(root);
}

AVLNode *rotateRightLeft(AVLNode *root)
{
    root->right = rotateRight(root->right);
    return rotateLeft(root);
}

AVLNode *balance(AVLNode *root)
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

AVLNode *copyTreeAVL(AVLNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    AVLNode *newNode = createNodeAVL(root->data);

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->left = copyTreeAVL(root->left);
    newNode->right = copyTreeAVL(root->right);
    newNode->height = root->height;

    return newNode;
}

AVLNode *insertAVL(AVLNode *root, int data)
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

AVLNode *removeAVL(AVLNode *root, int data)
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
            AVLNode *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            AVLNode *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            AVLNode *successor = root->right;

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

void preOrderAVL(AVLNode *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
        preOrderAVL(root->left);
        preOrderAVL(root->right);
    }
}

void inOrderAVL(AVLNode *root)
{
    if (root != NULL)
    {
        inOrderAVL(root->left);
        printf("%d ", root->data);
        inOrderAVL(root->right);
    }
}

void postOrderAVL(AVLNode *root)
{
    if (root != NULL)
    {
        postOrderAVL(root->left);
        postOrderAVL(root->right);
        printf("%d ", root->data);
    }
}

void clearTreeAVL(AVLNode *root)
{
    if (root == NULL)
    {
        return;
    }

    clearTreeAVL(root->left);
    clearTreeAVL(root->right);
    free(root);
}

int countNodesAVL(AVLNode *root)
{
    if (root == NULL)
    {
        return 0;
    }

    return 1 + countNodesAVL(root->left) + countNodesAVL(root->right);
}

int countLeavesAVL(AVLNode *root)
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

AVLNode *searchAVL(AVLNode *root, int data)
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

int searchLevelAVL(AVLNode *root, int data, int level)
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

int equalTreesAVL(AVLNode *root1, AVLNode *root2)
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

void invertTreeAVL(AVLNode *root)
{
    if (root == NULL)
    {
        return;
    }

    AVLNode *temp = root->left;
    root->left = root->right;
    root->right = temp;

    invertTreeAVL(root->left);
    invertTreeAVL(root->right);
}

int isBSTAVL(AVLNode *root, int min, int max)
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