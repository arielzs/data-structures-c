#ifndef TREE_H
#define TREE_H

/* Ponto de entrada do submenu de arvores, chamado pelo menu.c */
void runTreeMenu(void);

/* ========================= BST ========================= */

typedef struct BSTNode
{
    int data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *createNode(int data);
void preOrder(BSTNode *root);
void inOrder(BSTNode *root);
void postOrder(BSTNode *root);
void clearTree(BSTNode *root);
BSTNode *insertNode(BSTNode *root, int data);
BSTNode *removeNode(BSTNode *root, int data);
int height(BSTNode *root);
int countNodes(BSTNode *root);
int countLeaves(BSTNode *root);
BSTNode *search(BSTNode *root, int data);
int searchLevel(BSTNode *root, int data, int level);
BSTNode *copyTree(BSTNode *root);
int equalTrees(BSTNode *root1, BSTNode *root2);
void invertTree(BSTNode *root);
int isBST(BSTNode *root, int min, int max);

/* ========================= AVL ========================= */

typedef struct AVLNode
{
    int data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode *createNodeAVL(int data);
int heightAVL(AVLNode *root);
AVLNode *copyTreeAVL(AVLNode *root);

/* Balanceamento */
int balanceFactor(AVLNode *root);
AVLNode *rotateRight(AVLNode *root);
AVLNode *rotateLeft(AVLNode *root);
AVLNode *rotateLeftRight(AVLNode *root);
AVLNode *rotateRightLeft(AVLNode *root);
AVLNode *balance(AVLNode *root);

AVLNode *insertAVL(AVLNode *root, int data);
AVLNode *removeAVL(AVLNode *root, int data);

/* Equivalentes as funcoes utilitarias da BST, para a arvore AVL */
void preOrderAVL(AVLNode *root);
void inOrderAVL(AVLNode *root);
void postOrderAVL(AVLNode *root);
void clearTreeAVL(AVLNode *root);
int countNodesAVL(AVLNode *root);
int countLeavesAVL(AVLNode *root);
AVLNode *searchAVL(AVLNode *root, int data);
int searchLevelAVL(AVLNode *root, int data, int level);
int equalTreesAVL(AVLNode *root1, AVLNode *root2);
void invertTreeAVL(AVLNode *root);
int isBSTAVL(AVLNode *root, int min, int max);

#endif