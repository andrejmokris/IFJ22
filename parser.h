#ifndef parser
#define parser

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst.h"
#include "error.h"
#include "expression.h"
#include "scanner.h"
#include "stack.h"

#define SUCCESS_ELSE 1
#define SUCCESS_NOELSE 2
#define FAIL 0

#define PRINT_CODE(_fname, _params)                      \
    do {                                                 \
        insert_after_active_dll(&list, list.string_pos); \
        _fname(_params);                                 \
        list.string_pos = code.length;                   \
    } while (0)

int mainParser();
int endParser(int errCode);
int getParsToken();
int ParserLoop(bool getNext);
bool statementList(bool getNext, node_t *symTable, node_t funcNode);
int ifRule(node_t *symTable, node_t funcNode);
bool functionCall(String_t *fName, int *returnType, char scope, node_t *symTable);
bool returnStat(node_t *symTable, node_t funcNode);
bool writeBuiltIn(int *returnType);
bool isBuiltIn();
bool parameterDataTypeVerify(int op1, int op2, node_t *symTable);
bool functionDeclaration();
bool getFuncParams(node_t funcNode);
bool getSingleParam(node_t funcNode);
bool VarAssign(node_t *symTable);
bool Type();

#endif