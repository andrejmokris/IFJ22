#include "bst.h"

node_t createNode(int dataType, String_t NodeID) {
    node_t newNode = malloc(sizeof(struct TreeNode));
    String_t newString;

    if (newNode == NULL || StringInit(&newString) == false) {
        return NULL;
    }

    for (int i = 0; i < NodeID.length; i++) {
        if (stringAppend(&newString, NodeID.string[i]) == false) {
            return NULL;
        }
    }
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->dataType = dataType;
    newNode->NodeID = newString;
    return newNode;
}

void deconstructNode(node_t node) {
    stringDeconstruct(&node->NodeID);
    free(node);
}

node_t TreeInsert(node_t *root, int dataType, String_t NodeID) {
    if (*root == NULL) {
        node_t newNode = createNode(dataType, NodeID);
        if (newNode != NULL) {
            *root = newNode;
            return newNode;
        } else {
            return NULL;
        }
    } else if (strcmp(NodeID.string, (*root)->NodeID.string) == 0) {
        (*root)->dataType = dataType;
        return *root;
    } else if (strcmp(NodeID.string, (*root)->NodeID.string) > 0) {
        return TreeInsert(&((*root)->right), dataType, NodeID);
    } else {
        return TreeInsert(&((*root)->left), dataType, NodeID);
    }
}

node_t TreeFind(node_t root, char *string) {
    if (root == NULL) {
        return NULL;
    } else {
        if (strcmp(string, root->NodeID.string) == 0) {
            return root;
        } else if (strcmp(string, root->NodeID.string) > 0) {
            return TreeFind(root->right, string);
        } else {
            return TreeFind(root->left, string);
        }
    }
    return NULL;
}

void TreeDeconstruct(node_t root) {
    if(root == NULL) {
        return;
    } else {
        TreeDeconstruct(root->left);
        TreeDeconstruct(root->right);
        deconstructNode(root);
    }
}
