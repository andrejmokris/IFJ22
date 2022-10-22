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

int main() {
    String_t string;
    StringInit(&string);
    if (!checkProlog(&string)) {
        stringDeconstruct(&string);
        errorExit(LEX_ERROR, "Wrong prolog\n");
        return 1;
    }
    /*
    int res;
    while (((res = get_Token(&string)) != LEX_EOF)) {
        if (res == LEX_ERR) {
            stringDeconstruct(&string);
            errorExit(LEX_ERROR, "Wrong lex\n");
        }
        if(!stackPush(stack, res, string)) {
            stringDeconstruct(&string);
            errorExit(INTERNAL_ERROR, "STACKPUSH FAIL\n");
        }
    }
    */
    parseExpression();
    stringDeconstruct(&string);
    return 0;
}