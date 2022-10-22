#ifndef bst
#define bst

#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} node_t;

node_t *insert(node_t **root, int val);
node_t *find(node_t *root, int val);
void freeTree(node_t *root);


#endif
