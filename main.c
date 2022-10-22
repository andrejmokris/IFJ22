/*
 * IFJ 2022 PROJECT
 *
 * File: main.c
 *
 * Andrej Mokris: xmokri01
 * Veronika Simkova: xsimko14
 * Leos Nevoral: xnevor03
 * Adam Pap: xpapad11
 *
 */
#include "main.h"

#include <stdio.h>

#include "error.h"

int main() {
    String_t string;
    Stack *stack = initStack(5000);
    StringInit(&string);
    if (!checkProlog(&string)) {
        stringDeconstruct(&string);
        stackDeconstruct(stack);
        errorExit(LEX_ERROR, "Wrong prolog\n");
        return 1;
    }
    int res;
    while (((res = get_Token(&string)) != LEX_EOF)) {
        if (res == LEX_ERR) {
            stringDeconstruct(&string);
            stackDeconstruct(stack);
            errorExit(LEX_ERROR, "Wrong lex\n");
        }
        if(!stackPush(stack, res, string)) {
            stringDeconstruct(&string);
            stackDeconstruct(stack);
            errorExit(INTERNAL_ERROR, "STACKPUSH FAIL\n");
        }
    }
    printf("DLZKA STACKU: %ld\n", stack->elementCount);
    stringDeconstruct(&string);
    stackDeconstruct(stack);
    return 0;
}