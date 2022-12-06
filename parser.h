/*
 * IFJ 2022 PROJECT
 *
 * parser.h
 *
 * Andrej Mokris: xmokri01
 * Veronika Simkova: xsimko14
 * Leos Nevoral: xnevor03
 *
 */

#ifndef parser
#define parser

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"
#include "error.h"
#include "expression.h"
#include "scanner.h"
#include "stack.h"
#include "codeStore.h"

#define SUCCESS_ELSE 1
#define SUCCESS_NOELSE 2
#define FAIL 0

int mainParser();
int endParser(int errCode);
int getParsToken();
int ParserLoop(bool getNext);
unsigned long getLabel();
bool statementList(bool getNext, node_t *symTable, node_t funcNode);
int ifRule(node_t *symTable, node_t funcNode);
bool functionCall(String_t *fName, int *returnType, node_t *symTable);
bool returnStat(node_t *symTable, node_t funcNode);
bool writeBuiltIn(int *returnType, node_t *symTable);
bool isBuiltIn();
bool parameterDataTypeVerify(int op1, int op2, node_t *symTable);
bool functionDeclaration();
bool getFuncParams(node_t funcNode);
bool getSingleParam(node_t funcNode);
bool VarAssign(node_t *symTable);
bool Type();
int getTokenfromStore();

#endif