//
// Created by evanc on 10/26/26.
//

#ifndef MINECRAFT_IN_C_BST_H
#define MINECRAFT_IN_C_BST_H

struct BSTInternal;
typedef struct BSTInternal BSTInternal;

typedef struct {
    BSTInternal* internal;
} BST;

// Initializes RBST struct to an empty tree
BST initRBST();

// Inserts key
void insertRBST(BST, int key);

// Deletes key (or does nothing if key not found)
void deleteRBST(BST, int key);

// Returns 1 if key is found; otherwise 0
int isInRBST(BST, int key);

// Frees entire tree
void freeRBST(BST);

// returns error code from previous RBST operation,
// where 0 implies no error; >1 is error code
int getErrorCode(BST);

#endif //MINECRAFT_IN_C_BST_H
