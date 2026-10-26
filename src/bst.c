//
// Created by evanc on 10/26/26.
//

#include "../include/bst.h"
#include "../include/bst_internal_impl.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smart_array.h"

enum {
    RBST_OK = 0,
    RBST_ALLOC_FAIL = 1,
};

static int insertKeyIntoTree(BSTNode** tree, int key, size_t approximateSize);
static int shouldRebuild(size_t size);
static int rebuildRBSTWithNewRoot(BSTNode** tree, BSTNode* newRoot);
static int rebuildRBST(BSTNode** tree);
static void freeTree(BSTNode* tree);
static BSTNode* newNode(int key);
static void initNode(BSTNode* node, int key);
static size_t randomSizeTBetween(size_t start, size_t end);
static int deleteFromTree(BSTNode** tree, int key, size_t approximateSize);
static BSTNode *inOrderSuccessor(BSTNode* node);
static size_t findSplitIndex(BSTNode** sortedArray, size_t size, int key);
static void flattenTreeIntoPointerSmartArray(BSTNode* tree, SmartArray *array);
static BSTNode *makeRBST(BSTNode** nodes, size_t first, size_t last);

// Initializes RBST struct to an empty tree
BST initRBST()
{
    BST rbst;
    // if malloc fails, rbst.internal is NULL; callers should check getErrorCode()
    rbst.internal = malloc(sizeof(BSTInternal));
    if (rbst.internal != NULL) {
        rbst.internal->errorCode = RBST_OK;
        rbst.internal->root = NULL;
        rbst.internal->rootSize = 0;
    }
    return rbst;
}

// Inserts key
void insertRBST(BST rbst, int key)
{
    rbst.internal->errorCode = insertKeyIntoTree(&rbst.internal->root, key, rbst.internal->rootSize * rbst.internal->rootSize);
    if (rbst.internal->errorCode == RBST_OK) {
        rbst.internal->rootSize++;
    }
}

static int insertKeyIntoTree(BSTNode** tree, int key, size_t approximateSize)
{
    assert(tree != NULL);

    BSTNode** insertionPoint = tree;
    while (*insertionPoint != NULL) {
        BSTNode* current = *insertionPoint;
        if (shouldRebuild(approximateSize + 1)) {
            BSTNode* node = newNode(key);
            if (node == NULL) {
                return RBST_ALLOC_FAIL;
            }
            return rebuildRBSTWithNewRoot(insertionPoint, node);
        }

        insertionPoint = key > current->key ? &current->right : &current->left;
        approximateSize /= 2;
    }
    *insertionPoint = newNode(key);
    return *insertionPoint != NULL ? RBST_OK : RBST_ALLOC_FAIL;
}

static int shouldRebuild(size_t size)
{
    // These compile flags allow us to easily test the rebuild logic by forcing it to
    // always or never rebuild, while the default is probabilistic based on tree size
#if ALWAYS_REBUILD
    return 1;
#elif NEVER_REBUILD
    return 0;
#else
    if (size <= 16) {
        return 0; // never rebuild small trees
    }
    return randomSizeTBetween(0, size) == 0; // rebuild with probability 1/size
#endif
}

static int rebuildRBSTWithNewRoot(BSTNode** tree, BSTNode* newRoot)
{
    assert(tree != NULL);

    BSTNode* root = *tree;
    if (root == NULL) {
        return RBST_OK;
    }

    SmartArray array;
    initSmartArray(&array, smartArrayCheckedRealloc, sizeof(BSTNode*));
    flattenTreeIntoPointerSmartArray(root, &array);

    size_t const sizeBeforeRebuild = getSmartArrayCount(&array);
    BSTNode **sortedNodes = &SMART_ARRAY_AT(&array, 0, BSTNode*);

    size_t const splitIndex = findSplitIndex(sortedNodes, sizeBeforeRebuild, newRoot->key);

    if (splitIndex != 0) {
        newRoot->left = makeRBST(sortedNodes, 0, splitIndex - 1);
    }

    newRoot->right = makeRBST(sortedNodes, splitIndex, sizeBeforeRebuild - 1);

    *tree = newRoot;
    freeSmartArray(&array);
    return RBST_OK;
}

static BSTNode *makeRBST(BSTNode** nodes, size_t first, size_t last)
{
    if (last < first) {
        return NULL;
    }

    size_t const k = randomSizeTBetween(first, last);

    BSTNode* node = nodes[k];
    initNode(node, node->key); // reset size and child pointers

    if (first == last) {
        return node;
    }

    if (k != first) {
        node->left = makeRBST(nodes, first, k - 1);
    }

    node->right = makeRBST(nodes, k + 1, last);
    return node;
}

static size_t findSplitIndex(BSTNode** sortedArray, size_t size, int key)
{
    size_t low = 0, high = size;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (sortedArray[mid]->key < key) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

static size_t randomSizeTBetween(size_t start, size_t end)
{
    assert(start <= end);
    return ((size_t)rand() % (end - start + 1)) + start;
}

static BSTNode* newNode(int key)
{
    BSTNode* node = malloc(sizeof(BSTNode));
    if (node != NULL) {
        initNode(node, key);
    }
    return node;
}

static void initNode(BSTNode* node, int key)
{
    assert(node != NULL);
    node->key = key;
    node->left = node->right = NULL;
}

// Deletes key (or does nothing if key not found)
void deleteRBST(BST rbst, int key)
{
    if (rbst.internal == NULL || rbst.internal->root == NULL) {
        return;
    }
    if (deleteFromTree(&rbst.internal->root, key, rbst.internal->rootSize * rbst.internal->rootSize)) {
        rbst.internal->rootSize--;
    }
}

static int deleteFromTree(BSTNode** tree, int key, size_t approximateSize)
{
    if (*tree == NULL) {
        return 0; // Not found
    }

    BSTNode* root = *tree;

    if (key == root->key) {
        if (root->left != NULL && root->right != NULL) {
            BSTNode* successor = inOrderSuccessor(root);
            root->key = successor->key; // Copy successor's key to root
            // This will always return 1 since successor is guaranteed to be found in the right subtree
            (void)deleteFromTree(&root->right, successor->key, approximateSize / 2);
        } else {
            BSTNode* child = root->left != NULL ? root->left : root->right;
            free(root);
            *tree = child;
        }
        return 1; // Found and deleted
    }

    BSTNode** subtreeToDeleteFrom = key > root->key ? &root->right : &root->left;
    if (deleteFromTree(subtreeToDeleteFrom, key, approximateSize / 2)) {

        // Probabilistic rebuild of this subtree after deletion
        if (shouldRebuild(approximateSize)) {
            // We can ignore this return value because if it failed to rebuild, we still found and deleted the node.
            (void)rebuildRBST(subtreeToDeleteFrom);
        }

        return 1; // Found and deleted in a subtree
    }
    return 0; // Not found
}

static BSTNode *inOrderSuccessor(BSTNode* node)
{
    BSTNode* successor = node->right;
    while (successor != NULL && successor->left != NULL) {
        successor = successor->left;
    }
    return successor;
}

static int rebuildRBST(BSTNode** tree)
{
    assert(tree != NULL);
    BSTNode* root = *tree;
    if (root == NULL) {
        return RBST_OK;
    }

    SmartArray array;
    initSmartArray(&array, smartArrayCheckedRealloc, sizeof(BSTNode*));
    flattenTreeIntoPointerSmartArray(root, &array);

    size_t const size = getSmartArrayCount(&array);
    BSTNode **sortedNodes = &SMART_ARRAY_AT(&array, 0, BSTNode*);

    *tree = makeRBST(sortedNodes, 0, size - 1);
    freeSmartArray(&array);
    return RBST_OK;
}

static void flattenTreeIntoPointerSmartArray(BSTNode* tree, SmartArray *array)
{
    if (tree != NULL) {
        flattenTreeIntoPointerSmartArray(tree->left, array);
        smartArrayAppend(array, &tree);
        flattenTreeIntoPointerSmartArray(tree->right, array);
    }
}

// Returns 1 if key is found; otherwise 0
int isInRBST(BST rbst, int key)
{
    for (BSTNode* current = rbst.internal->root; current != NULL; current = key > current->key ? current->right : current->left) {
        if (key == current->key) {
            return 1;
        }
    }
    return 0;
}

// Frees entire tree
void freeRBST(BST rbst)
{
    freeTree(rbst.internal->root);
    free(rbst.internal);
}

static void freeTree(BSTNode* tree)
{
    if (tree != NULL) {
        freeTree(tree->left);
        freeTree(tree->right);
        free(tree);
    }
}

// returns error code from previous RBST operation,
// where 0 implies no error; >1 is error code
int getErrorCode(BST rbst)
{
    if (rbst.internal == NULL) {
        return RBST_ALLOC_FAIL;
    }

    return rbst.internal->errorCode;
}