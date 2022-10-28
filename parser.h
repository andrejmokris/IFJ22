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
int ParserLoop();

#endif