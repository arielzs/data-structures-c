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
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    trees[totalTrees].id = totalTrees + 1;
    trees[totalTrees].type = type;
    trees[totalTrees].root = NULL;

    active = totalTrees;
    totalTrees++;

    printf("Tree %d (%s) created and set as active!\n", trees[active].id, typeName(type));
}

static void listTrees()
{
    if (totalTrees == 0)
    {
        printf("No trees created yet. Use command [1] to create one.\n");
        return;
    }

    printf("=== Created trees ===\n");

    for (int i = 0; i < totalTrees; i++)
    {
        if (i == active)
        {
            printf("Tree %d - %s (active)\n", trees[i].id, typeName(trees[i].type));
        }
        else
        {
            printf("Tree %d - %s\n", trees[i].id, typeName(trees[i].type));
        }
    }
}

static void switchTree(int number)
{
    if (number < 1 || number > totalTrees)
    {
        printf("Invalid tree!\n");
        return;
    }

    active = number - 1;
    printf("Active tree is now: %d (%s)\n", trees[active].id, typeName(trees[active].type));
}

void runTreeMenu()
{
    int command, data;
    int running = 1;

    while (running)
    {
        printf("\n=== TREES MENU");

        if (active != -1)
        {
            printf(" | Active tree: %d (%s)", trees[active].id, typeName(trees[active].type));
        }

        printf(" ===\n");

        printf("[0] Back to main menu\n");
        printf("[1] Create a new tree\n");
        printf("[2] List the created trees\n");
        printf("[3] Switch the active tree\n");
        printf("[4] Print in-order\n");
        printf("[5] Print pre-order\n");
        printf("[6] Print post-order\n");
        printf("[7] Insert element\n");
        printf("[8] Remove element\n");
        printf("[9] Search for an element\n");
        printf("[10] Tree height\n");
        printf("[11] Tree node count\n");
        printf("[12] Tree leaf count\n");
        printf("[13] Level of a node\n");
        printf("[14] Copy the active tree (creates a new one)\n");
        printf("[15] Compare two trees\n");
        printf("[16] Invert the active tree\n");
        printf("[17] Check if it is a valid BST\n");
        printf("--- AVL-only commands (balancing tests) ---\n");
        printf("[18] Show the root balance factor\n");
        printf("[19] Rotate the root right\n");
        printf("[20] Rotate the root left\n");
        printf("[21] Left-Right double rotation on the root\n");
        printf("[22] Right-Left double rotation on the root\n");
        printf("[23] Force balancing of the root\n");

        command = utilsReadInt("Choice: ");

        if (active == -1 && command != 0 && command != 1 && command != 2)
        {
            printf("You need to create a tree first! (command 1)\n");
            continue;
        }

        switch (command)
        {
        case 0:
            running = 0;
            break;

        case 1:
        {
            int typeChoice = utilsReadInt("Which type of tree?\n[1] BST\n[2] AVL\n");

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
                printf("Invalid type!\n");
            }

            break;
        }

        case 2:
            listTrees();
            break;

        case 3:
        {
            listTrees();

            int number = utilsReadInt("Which tree do you want to activate?\n");

            switchTree(number);
            break;
        }

        case 4:
            if (trees[active].root == NULL)
            {
                printf("Empty tree\n");
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
                printf("Empty tree\n");
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
                printf("Empty tree\n");
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
            data = utilsReadInt("Which element?\n");

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
            data = utilsReadInt("Which element?\n");

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
            data = utilsReadInt("Which element?\n");

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
                printf("Element found!\n");
            }
            else
            {
                printf("Element not found!\n");
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

            printf("Tree height: %d\n", treeHeight);

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

            printf("The tree has %d nodes\n", totalNodes);

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

            printf("The tree has %d leaves\n", totalLeaves);

            break;
        }

        case 13:
        {
            data = utilsReadInt("Which node?\n");

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
                printf("Value not found\n");
            }
            else
            {
                printf("Level: %d\n", level);
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

            printf("Tree %d copied to new tree %d!\n", sourceId, trees[active].id);

            break;
        }

        case 15:
        {
            listTrees();

            int firstNumber = utilsReadInt("First tree:\n");
            int secondNumber = utilsReadInt("Second tree:\n");

            if (firstNumber < 1 || firstNumber > totalTrees || secondNumber < 1 || secondNumber > totalTrees)
            {
                printf("Invalid tree!\n");
                break;
            }

            Tree *treeA = &trees[firstNumber - 1];
            Tree *treeB = &trees[secondNumber - 1];

            if (treeA->type != treeB->type)
            {
                printf("Trees of different types (BST and AVL) are always considered different!\n");
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
                printf("The trees are equal!\n");
            }
            else
            {
                printf("The trees are different!\n");
            }

            break;
        }

        case 16:
            if (trees[active].root == NULL)
            {
                printf("Empty tree\n");
            }
            else if (trees[active].type == TREE_BST)
            {
                invertTree((BSTNode *)trees[active].root);
                printf("Tree inverted!\n");
            }
            else
            {
                invertTreeAVL((AVLNode *)trees[active].root);
                printf("Tree inverted!\n");
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
                printf("The tree is a valid BST!\n");
            }
            else
            {
                printf("The tree is not a valid BST!\n");
            }

            break;
        }

        case 18:
            if (trees[active].type != TREE_AVL)
            {
                printf("This command only works on AVL trees!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Empty tree\n");
            }
            else
            {
                int factor = balanceFactor((AVLNode *)trees[active].root);
                printf("Root balance factor: %d\n", factor);
            }
            break;

        case 19:
            if (trees[active].type != TREE_AVL)
            {
                printf("This command only works on AVL trees!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Empty tree\n");
            }
            else if (((AVLNode *)trees[active].root)->left == NULL)
            {
                printf("Cannot rotate right: the root has no left child!\n");
            }
            else
            {
                trees[active].root = rotateRight((AVLNode *)trees[active].root);
                printf("Right rotation applied to the root!\n");
            }
            break;

        case 20:
            if (trees[active].type != TREE_AVL)
            {
                printf("This command only works on AVL trees!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Empty tree\n");
            }
            else if (((AVLNode *)trees[active].root)->right == NULL)
            {
                printf("Cannot rotate left: the root has no right child!\n");
            }
            else
            {
                trees[active].root = rotateLeft((AVLNode *)trees[active].root);
                printf("Left rotation applied to the root!\n");
            }
            break;

        case 21:
            if (trees[active].type != TREE_AVL)
            {
                printf("This command only works on AVL trees!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Empty tree\n");
            }
            else if (((AVLNode *)trees[active].root)->left == NULL || ((AVLNode *)trees[active].root)->left->right == NULL)
            {
                printf("Cannot do a Left-Right double rotation in this configuration!\n");
            }
            else
            {
                trees[active].root = rotateLeftRight((AVLNode *)trees[active].root);
                printf("Left-Right double rotation applied to the root!\n");
            }
            break;

        case 22:
            if (trees[active].type != TREE_AVL)
            {
                printf("This command only works on AVL trees!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Empty tree\n");
            }
            else if (((AVLNode *)trees[active].root)->right == NULL || ((AVLNode *)trees[active].root)->right->left == NULL)
            {
                printf("Cannot do a Right-Left double rotation in this configuration!\n");
            }
            else
            {
                trees[active].root = rotateRightLeft((AVLNode *)trees[active].root);
                printf("Right-Left double rotation applied to the root!\n");
            }
            break;

        case 23:
            if (trees[active].type != TREE_AVL)
            {
                printf("This command only works on AVL trees!\n");
            }
            else if (trees[active].root == NULL)
            {
                printf("Empty tree\n");
            }
            else
            {
                trees[active].root = balance((AVLNode *)trees[active].root);
                printf("Balance forced on the root!\n");
            }
            break;

        default:
            printf("Enter a valid command!\n");
            break;
        }
    }
}