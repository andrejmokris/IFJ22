/*
* IFJ 2022 PROJECT
*
* File: dynamic_string.c
*
* Dynamic string file
*/

#include "dynamic_string.h"

bool StringInit(String_t *str)
{
    if ((str->string = malloc(DEFAULT_ARR_SIZE * sizeof(char))) == NULL)
    {
        fprintf(stderr,"Internal error while allocating dynamic string.\n");
        return false;
    }
    str->length = 0;
    str->allocSize = DEFAULT_ARR_SIZE;
    str->string[0] = '\0';
    return true;
}

bool resizeString(String_t *str) {
    str->string = (char *) realloc(str->string, str->length * 2);
    if(str->string != NULL) {
        str->allocSize = str->allocSize * 2;
        return true;
    }
    return false;
}

// add one character at the end of the string
bool stringAppend(String_t *str, int c) {
    if (str != NULL && str->string != NULL) {
        if (str->length - 2 < str->allocSize) {
            str->string[str->length++] = c;
            str->string[str->length] = '\0';
            return true;
        } else {
            if (resizeString(str)) {
                str->string[str->length++] = c;
                str->string[str->length] = '\0';
                return true;
            }
        }
    }
    return false;
}

void stringClear(String_t *str) {
    str->length = 0;
    memset(str->string,0,strlen(str->string));
    str->string[0] = '\0';
}

void stringDeconstruct(String_t *str) {
    free(str->string);
}