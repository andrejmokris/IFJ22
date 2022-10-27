#ifndef BST_
#define BST_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"

typedef struct TreeNode {
    String_t NodeID; // identifier name e.g: X
    int dataType;
    struct TreeNode *left;
    struct TreeNode *right;
} *node_t;

node_t TreeInsert(node_t *root, int dataType, String_t NodeID);
node_t createNode(int dataType, String_t NodeID);
node_t TreeFind(node_t root, char *string);
void deconstructNode(node_t node);
void TreeDeconstruct(node_t root);

#endif
