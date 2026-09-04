#ifndef TREE_H
#define TREE_H

/* Ponto de entrada do submenu de arvores, chamado pelo menu.c */
void runTreeMenu();

/* ========================= BST ========================= */

typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(int data);
void preOrder(Node *root);
void inOrder(Node *root);
void postOrder(Node *root);
void clearTree(Node *root);
Node *insertNode(Node *root, int data);
Node *removeNode(Node *root, int data);
int height(Node *root);
int countNodes(Node *root);
int countLeaves(Node *root);
Node *search(Node *root, int data);
int searchLevel(Node *root, int data, int level);
Node *copyTree(Node *root);
int equalTrees(Node *root1, Node *root2);
void invertTree(Node *root);
int isBST(Node *root, int min, int max);

/* ========================= AVL ========================= */

typedef struct NodeAVL
{
    int data;
    int height;
    struct NodeAVL *left;
    struct NodeAVL *right;
} NodeAVL;

NodeAVL *createNodeAVL(int data);
int heightAVL(NodeAVL *root);
NodeAVL *copyTreeAVL(NodeAVL *root);

/* Balanceamento */
int balanceFactor(NodeAVL *root);
NodeAVL *rotateRight(NodeAVL *root);
NodeAVL *rotateLeft(NodeAVL *root);
NodeAVL *rotateLeftRight(NodeAVL *root);
NodeAVL *rotateRightLeft(NodeAVL *root);
NodeAVL *balance(NodeAVL *root);

NodeAVL *insertAVL(NodeAVL *root, int data);
NodeAVL *removeAVL(NodeAVL *root, int data);

/* Equivalentes as funcoes utilitarias da BST, para a arvore AVL */
void preOrderAVL(NodeAVL *root);
void inOrderAVL(NodeAVL *root);
void postOrderAVL(NodeAVL *root);
void clearTreeAVL(NodeAVL *root);
int countNodesAVL(NodeAVL *root);
int countLeavesAVL(NodeAVL *root);
NodeAVL *searchAVL(NodeAVL *root, int data);
int searchLevelAVL(NodeAVL *root, int data, int level);
int equalTreesAVL(NodeAVL *root1, NodeAVL *root2);
void invertTreeAVL(NodeAVL *root);
int isBSTAVL(NodeAVL *root, int min, int max);

#endif