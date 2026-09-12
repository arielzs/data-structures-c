#include <assert.h>
#include "common.h"
#include "tree.h"

/* Confere o fator de balanceamento em TODOS os nodes, nao so na raiz */
static int isFullyBalanced(AVLNode *node)
{
    if (node == NULL)
    {
        return 1;
    }

    int factor = balanceFactor(node);

    if (factor < -1 || factor > 1)
    {
        return 0;
    }

    return isFullyBalanced(node->left) && isFullyBalanced(node->right);
}

int main()
{
    /* ===== Insercao em ordem crescente forca rotacoes e mantem balanceada ===== */
    AVLNode *root = NULL;

    for (int i = 1; i <= 7; i++)
    {
        root = insertAVL(root, i);
    }

    assert(root->data == 4);
    assert(heightAVL(root) == 2);
    assert(countNodesAVL(root) == 7);
    assert(countLeavesAVL(root) == 4);
    assert(isFullyBalanced(root) == 1);

    /* ===== Insercao de valor repetido nao duplica ===== */
    int countBefore = countNodesAVL(root);
    root = insertAVL(root, 4);
    assert(countNodesAVL(root) == countBefore);

    /* ===== Busca ===== */
    assert(searchAVL(root, 4) != NULL);
    assert(searchAVL(root, 999) == NULL);
    assert(searchLevelAVL(root, 4, 0) == 0);
    assert(searchLevelAVL(root, 999, 0) == -1);

    /* ===== Remocao mantem o balanceamento em toda a arvore ===== */
    root = removeAVL(root, 1);
    root = removeAVL(root, 2);
    assert(countNodesAVL(root) == 5);
    assert(isFullyBalanced(root) == 1);
    assert(isBSTAVL(root, INT_MIN, INT_MAX) == 1);

    /* ===== Remocao de valor inexistente nao muda a arvore ===== */
    int countBeforeMissing = countNodesAVL(root);
    root = removeAVL(root, 12345);
    assert(countNodesAVL(root) == countBeforeMissing);

    /* ===== copyTreeAVL / equalTreesAVL ===== */
    AVLNode *copy = copyTreeAVL(root);
    assert(equalTreesAVL(root, copy) == 1);

    copy = insertAVL(copy, 999);
    assert(equalTreesAVL(root, copy) == 0);
    assert(searchAVL(root, 999) == NULL);

    clearTreeAVL(copy);

    /* ===== invertTreeAVL quebra a propriedade de BST de proposito ===== */
    if (countNodesAVL(root) > 1)
    {
        invertTreeAVL(root);
        assert(isBSTAVL(root, INT_MIN, INT_MAX) == 0);

        invertTreeAVL(root); /* volta ao estado normal */
        assert(isBSTAVL(root, INT_MIN, INT_MAX) == 1);
    }

    clearTreeAVL(root);

    /* ===== Insercao em ordem decrescente tambem deve ficar balanceada ===== */
    AVLNode *descending = NULL;

    for (int i = 10; i >= 1; i--)
    {
        descending = insertAVL(descending, i);
    }

    assert(countNodesAVL(descending) == 10);
    assert(isFullyBalanced(descending) == 1);

    clearTreeAVL(descending);

    /* ===== As 4 rotacoes, isoladas, montadas manualmente ===== */

    /* rotateRight: desbalanco Esquerda-Esquerda */
    {
        AVLNode *node3 = createNodeAVL(3);
        AVLNode *node2 = createNodeAVL(2);
        AVLNode *node1 = createNodeAVL(1);

        node3->left = node2;
        node2->left = node1;

        AVLNode *newRoot = rotateRight(node3);

        assert(newRoot->data == 2);
        assert(newRoot->left->data == 1);
        assert(newRoot->right->data == 3);

        clearTreeAVL(newRoot);
    }

    /* rotateLeft: desbalanco Direita-Direita */
    {
        AVLNode *node1 = createNodeAVL(1);
        AVLNode *node2 = createNodeAVL(2);
        AVLNode *node3 = createNodeAVL(3);

        node1->right = node2;
        node2->right = node3;

        AVLNode *newRoot = rotateLeft(node1);

        assert(newRoot->data == 2);
        assert(newRoot->left->data == 1);
        assert(newRoot->right->data == 3);

        clearTreeAVL(newRoot);
    }

    /* rotateLeftRight: desbalanco Esquerda-Direita */
    {
        AVLNode *node3 = createNodeAVL(3);
        AVLNode *node1 = createNodeAVL(1);
        AVLNode *node2 = createNodeAVL(2);

        node3->left = node1;
        node1->right = node2;

        AVLNode *newRoot = rotateLeftRight(node3);

        assert(newRoot->data == 2);
        assert(newRoot->left->data == 1);
        assert(newRoot->right->data == 3);

        clearTreeAVL(newRoot);
    }

    /* rotateRightLeft: desbalanco Direita-Esquerda */
    {
        AVLNode *node1 = createNodeAVL(1);
        AVLNode *node3 = createNodeAVL(3);
        AVLNode *node2 = createNodeAVL(2);

        node1->right = node3;
        node3->left = node2;

        AVLNode *newRoot = rotateRightLeft(node1);

        assert(newRoot->data == 2);
        assert(newRoot->left->data == 1);
        assert(newRoot->right->data == 3);

        clearTreeAVL(newRoot);
    }

    /* balance(): detecta o desbalanco sozinho e aplica a rotacao certa,
       sem passar pelo insertAVL (que ja balanceia automaticamente) */
    {
        AVLNode *node3 = createNodeAVL(3);
        AVLNode *node2 = createNodeAVL(2);
        AVLNode *node1 = createNodeAVL(1);

        node3->left = node2;
        node2->left = node1;

        node3->height = 2;
        node2->height = 1;
        node1->height = 0;

        AVLNode *result = balance(node3);

        assert(result->data == 2);
        assert(isFullyBalanced(result) == 1);

        clearTreeAVL(result);
    }

    /* ===== Arvore vazia: casos-limite ===== */
    AVLNode *empty = NULL;
    assert(heightAVL(empty) == -1);
    assert(countNodesAVL(empty) == 0);
    assert(countLeavesAVL(empty) == 0);
    assert(searchAVL(empty, 1) == NULL);
    assert(balanceFactor(empty) == 0);
    assert(isBSTAVL(empty, INT_MIN, INT_MAX) == 1);

    return 0;
}