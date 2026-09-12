#include <assert.h>
#include "common.h"
#include "tree.h"

int main()
{
    /* ===== Insercao respeita a ordem de BST ===== */
    BSTNode *root = NULL;

    root = insertNode(root, 50);
    root = insertNode(root, 30);
    root = insertNode(root, 70);
    root = insertNode(root, 20);
    root = insertNode(root, 40);
    root = insertNode(root, 60);
    root = insertNode(root, 80);

    assert(root->data == 50);
    assert(countNodes(root) == 7);
    assert(countLeaves(root) == 4);
    assert(height(root) == 2);
    assert(isBST(root, INT_MIN, INT_MAX) == 1);

    /* ===== Insercao de valor repetido nao duplica ===== */
    int countBefore = countNodes(root);
    root = insertNode(root, 50);
    assert(countNodes(root) == countBefore);

    /* ===== Busca ===== */
    assert(search(root, 40) != NULL);
    assert(search(root, 999) == NULL);
    assert(searchLevel(root, 50, 0) == 0);
    assert(searchLevel(root, 20, 0) == 2);
    assert(searchLevel(root, 999, 0) == -1);

    /* ===== Remocao: caso folha (sem filhos) ===== */
    root = removeNode(root, 20);
    assert(search(root, 20) == NULL);
    assert(countNodes(root) == 6);
    assert(isBST(root, INT_MIN, INT_MAX) == 1);

    /* ===== Remocao: caso 1 filho =====
       apos remover 20, o node 30 fica so com o filho direito (40) */
    root = removeNode(root, 30);
    assert(search(root, 30) == NULL);
    assert(search(root, 40) != NULL);
    assert(isBST(root, INT_MIN, INT_MAX) == 1);

    /* ===== Remocao: caso 2 filhos (raiz) =====
       exercita a busca pelo sucessor (menor valor da subarvore direita) */
    root = removeNode(root, 50);
    assert(search(root, 50) == NULL);
    assert(isBST(root, INT_MIN, INT_MAX) == 1);
    assert(countNodes(root) == 4);

    /* ===== Remocao de valor inexistente nao muda a arvore ===== */
    int countBeforeMissing = countNodes(root);
    root = removeNode(root, 12345);
    assert(countNodes(root) == countBeforeMissing);

    /* ===== copyTree / equalTrees ===== */
    BSTNode *copy = copyTree(root);
    assert(equalTrees(root, copy) == 1);

    /* Alterar a copia nao pode afetar o original */
    copy = insertNode(copy, 999);
    assert(equalTrees(root, copy) == 0);
    assert(search(root, 999) == NULL);

    clearTree(copy);

    /* ===== invertTree quebra a propriedade de BST de proposito ===== */
    if (countNodes(root) > 1)
    {
        invertTree(root);
        assert(isBST(root, INT_MIN, INT_MAX) == 0);

        invertTree(root); /* volta ao estado normal */
        assert(isBST(root, INT_MIN, INT_MAX) == 1);
    }

    clearTree(root);

    /* ===== Arvore vazia: casos-limite ===== */
    BSTNode *empty = NULL;
    assert(height(empty) == -1);
    assert(countNodes(empty) == 0);
    assert(countLeaves(empty) == 0);
    assert(search(empty, 1) == NULL);
    assert(isBST(empty, INT_MIN, INT_MAX) == 1);

    return 0;
}