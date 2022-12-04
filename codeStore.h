#ifndef _CODESTORE
#define _CODESTORE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"
#include "scanner.h"

#define DOLLAR_SIGN 0

typedef struct token {
    int lexID;
    String_t tokenVal;
} *token_t;

typedef struct CodeStore {
    size_t stackSize;
    size_t elementCount;
    size_t lastPrinted;
    token_t *items;
} *CodeStore_t;

CodeStore_t initCodeStore(size_t size);
token_t createToken(String_t *source, int lexID);
bool CodeStoreAppend(CodeStore_t store, String_t *source, int lexID);

#endif