#include "common.h"
#include "tree.h"

BSTNode *createNode(int data)
{
    BSTNode *newNode = malloc(sizeof(BSTNode));

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

void preOrder(BSTNode *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(BSTNode *root)
{
    if (root != NULL)
    {
        inOrder(root->left);
        printf("%d ", root->data);
        inOrder(root->right);
    }
}

void postOrder(BSTNode *root)
{
    if (root != NULL)
    {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->data);
    }
}

void clearTree(BSTNode *root)
{
    if (root == NULL)
    {
        return;
    }

    clearTree(root->left);
    clearTree(root->right);
    free(root);
}

BSTNode *insertNode(BSTNode *root, int data)
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

BSTNode *removeNode(BSTNode *root, int data)
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
            BSTNode *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            BSTNode *successor = root->right;

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

int height(BSTNode *root)
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

int countNodes(BSTNode *root)
{
    if (root == NULL)
    {
        return 0;
    }

    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(BSTNode *root)
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

BSTNode *search(BSTNode *root, int data)
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

int searchLevel(BSTNode *root, int data, int level)
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

BSTNode *copyTree(BSTNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    BSTNode *newNode = createNode(root->data);

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->left = copyTree(root->left);
    newNode->right = copyTree(root->right);

    return newNode;
}

int equalTrees(BSTNode *root1, BSTNode *root2)
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

    return equalTrees(root1->left, root2->left) && equalTrees(root1->right, root2->right);
}

void invertTree(BSTNode *root)
{
    if (root == NULL)
    {
        return;
    }

    BSTNode *temp = root->left;
    root->left = root->right;
    root->right = temp;

    invertTree(root->left);
    invertTree(root->right);
}

int isBST(BSTNode *root, int min, int max)
{
    if (root == NULL)
    {
        return 1;
    }

    if (root->data <= min || root->data >= max)
    {
        return 0;
    }

    return isBST(root->left, min, root->data) && isBST(root->right, root->data, max);
}