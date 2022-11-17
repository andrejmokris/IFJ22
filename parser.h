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

#define SUCCESS_ELSE    1
#define SUCCESS_NOELSE  2
#define FAIL            0

int mainParser();
int endParser(int errCode);
int getParsToken();
int ParserLoop(bool getNext);
bool statementList(bool getNext, node_t *symTable);
int ifRule(node_t *symTable);
bool functionCall();
bool parameterDataTypeVerify(int op1, int op2);
bool functionDeclaration();
bool getFuncParams(node_t funcNode);
bool getSingleParam(node_t funcNode);
bool VarAssign(node_t *symTable);
bool Type();

#endif