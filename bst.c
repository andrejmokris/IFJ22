#include "bst.h"

node_t *insert(node_t **root, int val) {
    if (*root == NULL) {
        *root = malloc(sizeof(node_t));
        if(*root != NULL) {
            (*root)->data = val;
            (*root)->left = (*root)->right = NULL;
            return *root;
        } else {
            fprintf(stderr, "Memory not allocated\n");
            return NULL;
        }
    }
    if(val < (*root)->data) {
        return insert(&((*root)->left), val);
    } else {
        return insert(&((*root)->right), val);
    }
}

node_t *find(node_t *root, int val) {
    if(root == NULL) {
        return NULL;
    }
    if (root->data == val) {
        return root;
    } else if (val > root->data) {
        return find(root->right, val);
    } else {
        return find(root->left, val);
    }
    
}

void freeTree(node_t *root) {
    if(root->left != NULL) {
        freeTree(root->left);
    }
    if(root->right != NULL) {
        freeTree(root->right);
    }
    free(root);
}

int demo() {
    node_t *root = NULL;
    time_t t;

    srand((unsigned) time(&t));
    for(int i = 0 ; i < 100 ; i++ ) {
        insert(&root, rand() % 100);
    }
    insert(&root, 5);
    insert(&root, 2);
    insert(&root, 11);
    insert(&root, 8);
    
    node_t *result = find(root, 2);
    if (result) {
        printf("NODE FOUND: %d\n", result->data);
    } else {
        printf("NODE NOT FOUND\n");
    }
    freeTree(root);
    return 0;
}