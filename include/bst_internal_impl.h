//
// Created by evanc on 10/26/26.
//

#ifndef MINECRAFT_IN_C_BST_INTERNAL_IMPL_H
#define MINECRAFT_IN_C_BST_INTERNAL_IMPL_H

#include <stddef.h>

typedef struct BSTNode {
    int key;
    // size_t size; /* subtree size, used for rebuilding */
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

struct BSTInternal {
    int errorCode;
    BSTNode* root;
    size_t rootSize;
};

#endif //MINECRAFT_IN_C_BST_INTERNAL_IMPL_H
