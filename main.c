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
    StringInit(&string);
    if (!checkProlog(&string)) {
        errorExit(LEX_ERROR, "Wrong prolog\n");
        return 1;
    }
    int res;
    while (((res = get_Token(&string)) != LEX_EOF)) {
        if (res == LEX_ERR) {
            errorExit(LEX_ERROR, "Wrong lex\n");
        }
        printf("RESULT: %d\n", res);
        if (string.length > 0) {
            printf("STRING VALUE: %s\n", string.string);
        }
    }
    stringDeconstruct(&string);
    return 0;
}