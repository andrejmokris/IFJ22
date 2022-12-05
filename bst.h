/*
 * IFJ 2022 PROJECT
 *
 * File: bst.h
 *
 * Andrej Mokris: xmokri01
 * Veronika Simkova: xsimko14
 *
 */

#ifndef BST_
#define BST_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"
#include "error.h"

typedef struct Param {
    int dataType;
    String_t ParamID;
} *param_t;

typedef struct FunctionNode {
    int returnType;
    int nOfParams;
    int stackSize;
    bool hasReturn;
    param_t *params;
    struct TreeNode *symTable;
} *func_t;

typedef struct TreeNode {
    String_t NodeID; // identifier name e.g: X
    int dataType;
    func_t function; // pointer to function Node for info about the function
    struct TreeNode *left;
    struct TreeNode *right;
} *node_t;

node_t TreeInsert(node_t *root, int dataType, String_t NodeID);
node_t TreeInsertNode(node_t *root, node_t newNode);
node_t createNode(int dataType, String_t NodeID);
node_t TreeFind(node_t root, char *string);
void deconstructNode(node_t node);
void TreeDeconstruct(node_t root);

node_t createFuncNode(int dataType, String_t NodeID);
int addParam(node_t node, int dataType, String_t pID);

#endif
