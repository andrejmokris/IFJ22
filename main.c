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

#include "print_inst.h"

/*
int main() {
    String_t string;
    int res;
    StringInit(&string);
    if (!checkProlog(&string)) {
        stringDeconstruct(&string);
        errorExit(LEX_ERROR, "Wrong prolog\n");
        return 1;
    }
    while (((res = get_Token(&string)) != LEX_EOF)) {
        if (res == LEX_ERR) {
            stringDeconstruct(&string);
            errorExit(LEX_ERROR, "Wrong lex\n");
        }
        printf("CUR LEX: %d | STRING: %s\n", res, string.string);
    }
    stringDeconstruct(&string);
    return 0;
}
*/

int main() {
    int res = mainParser();
    if (res == SUCCESS) {
        print_code();
    }
    printf("RESULT: %d\n", res);
    return 0;
}
