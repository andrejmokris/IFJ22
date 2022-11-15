#ifndef _EXPRESSION
#define _EXPRESSION

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "stack.h"
#include "parser.h"
#include "error.h"
#include "bst.h"

#define STACK_INIT_SIZE 977

static char table[14][14] = { 
   // +    -    *    /   ===  !==   <    >   <=  >=     (    )    i    $
    {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // +
    {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // -
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // *
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // /
    {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // ===
    {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // !==
    {'<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '>'}, // <
    {'<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '>'}, // >
    {'<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '>'}, // <=
    {'<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '>'}, // >=
    {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', 'X'}, // (
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', '>'}, // )
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', '>'}, // i
    {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'X', '<', 'A'}  // $
};

int parseExpression(int endChar, int *resDataType, node_t symTable);

#endif