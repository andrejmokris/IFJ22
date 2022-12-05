/*
 * IFJ 2022 PROJECT
 *
 * File: codestore.c
 *
 * Andrej Mokris: xmokri01
 *
 */

#include "codeStore.h"

CodeStore_t initCodeStore(size_t size) {
    CodeStore_t newCodeStore = malloc(sizeof(struct CodeStore));
    if(newCodeStore == NULL) {
        return NULL;
    }
    newCodeStore->items = malloc(size * sizeof(token_t));
    if(newCodeStore->items == NULL) {
        return NULL;
    }
    newCodeStore->stackSize = size;
    newCodeStore->elementCount = 0;
    newCodeStore->lastPrinted = 0;
    return newCodeStore;
}

token_t createToken(String_t *source, int lexID) {
    token_t newToken = malloc(sizeof(struct token));
    String_t newString;
    
    if (newToken == NULL || StringInit(&newString) == false) {
        return NULL;
    }

    stringCopy(&newString, source);
    newToken->tokenVal = newString;
    newToken->lexID = lexID;
    return newToken;
}

bool CodeStoreAppend(CodeStore_t store, String_t *source, int lexID) {
    token_t newToken = createToken(source, lexID);
    if(newToken == NULL) {
        return false;
    }

    if(store->elementCount < store->stackSize) {
        store->items[store->elementCount] = newToken;
        store->elementCount++;
    } else {
        store->items = realloc(store->items, (store->stackSize * 2) * sizeof(token_t));
        store->stackSize = store->stackSize * 2;
        if(store->items == NULL) {
            return false;
        }
        store->items[store->elementCount] = newToken;
        store->elementCount++;
    }
    return true;
}