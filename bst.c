/*
 * IFJ 2022 PROJECT
 *
 * File: bst.c
 *
 * Andrej Mokris: xmokri01
 * Veronika Simkova: xsimko14
 *
 */

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
    newNode->function = NULL;
    newNode->dataType = dataType;
    newNode->NodeID = newString;
    return newNode;
}

node_t createFuncNode(int dataType, String_t NodeID) {
    node_t newNode = createNode(dataType, NodeID);
    func_t funcNode = malloc(sizeof(struct FunctionNode));
    if (newNode == NULL || funcNode == NULL) {
        return NULL;
    }
    funcNode->symTable = NULL;
    funcNode->params = malloc(15 * sizeof(param_t));
    funcNode->stackSize = 15;
    funcNode->nOfParams = 0;
    newNode->function = funcNode;
    return newNode;
}

// check if function declaration does not have more params with the same ID
bool duplicateParamName(node_t node, String_t pID) {
    int par_cnt = node->function->nOfParams;
    for(int i = 0; i < par_cnt; i++) {
        if(!strcmp(node->function->params[i]->ParamID.string, pID.string)) {
            return false;
        }
    }
    return true;
}

int addParam(node_t node, int dataType, String_t pID) {
    if(!duplicateParamName(node, pID)) {
        printf("Duplicate parameter name\n");
        return OTHERSEM_ERROR;
    }
    param_t newParam = malloc(sizeof(struct Param));
    String_t paramID;
    if (newParam == NULL || !StringInit(&paramID)) {
        return INTERNAL_ERROR;
    }
    newParam->ParamID = paramID;
    newParam->dataType = dataType;
    if (!stringCopy(&paramID, &pID)) {
        return INTERNAL_ERROR;
    }
    if (node->function->nOfParams < node->function->stackSize) {
        node->function->params[node->function->nOfParams] = newParam;
        node->function->nOfParams++;
    } else {
        node->function->params =
            realloc(node->function->params, node->function->stackSize * 2);
        if (node->function->params == NULL) {
            return INTERNAL_ERROR;
        }
        node->function->stackSize = node->function->stackSize * 2;
        node->function->params[node->function->nOfParams] = newParam;
        node->function->nOfParams++;
    }
    return SUCCESS;
}

void deconstructNode(node_t node) {
    stringDeconstruct(&node->NodeID);
    if (node->function != NULL) {
        for (int i = 0; i < node->function->nOfParams; i++) {
            param_t curParam = node->function->params[i];
            stringDeconstruct(&curParam->ParamID);
            free(curParam);
        }
        TreeDeconstruct(node->function->symTable);
        free(node->function->params);
        free(node->function);
    }
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

node_t TreeInsertNode(node_t *root, node_t newNode) {
    if (*root == NULL) {
        if (newNode != NULL) {
            *root = newNode;
            return newNode;
        } else {
            return NULL;
        }
    } else if (strcmp(newNode->NodeID.string, (*root)->NodeID.string) == 0) {
        (*root)->dataType = newNode->dataType;
        return *root;
    } else if (strcmp(newNode->NodeID.string, (*root)->NodeID.string) > 0) {
        return TreeInsertNode(&((*root)->right), newNode);
    } else {
        return TreeInsertNode(&((*root)->left), newNode);
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
    if (root == NULL) {
        return;
    } else {
        TreeDeconstruct(root->left);
        TreeDeconstruct(root->right);
        deconstructNode(root);
    }
}
