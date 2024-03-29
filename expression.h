/*
 * IFJ 2022 PROJECT
 *
 * File: main.c
 *
 * Andrej Mokris: xmokri01
 *
 */

#ifndef _EXPRESSION
#define _EXPRESSION

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"
#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "stack.h"

#define STACK_INIT_SIZE 977

int parseExpression(int endChar, int *resDataType, node_t *symTable);

#endif