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

int mainParser();
int getParsToken();
int ParserLoop();
bool statementList();
bool ifRule();
bool functionCall();
bool parameterDataTypeVerify(int op1, int op2);
bool functionDeclaration();
bool getFuncParams(node_t funcNode);
bool getSingleParam(node_t funcNode);
bool VarAssign();
bool Type();

#endif