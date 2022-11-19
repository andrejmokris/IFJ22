/*
 * IFJ 2022 PROJECT
 *
 * File: dynamic_string.c
 *
 * Dynamic string file
 */

#include "dynamic_string.h"

#include <string.h>

#include "error.h"

bool StringInit(String_t *str) {
    if ((str->string = malloc(DEFAULT_ARR_SIZE * sizeof(char))) == NULL) {
        return false;
    }
    str->length = 0;
    str->allocSize = DEFAULT_ARR_SIZE;
    str->string[0] = '\0';
    return true;
}

bool resizeString(String_t *str) {
    str->string = (char *)realloc(str->string, str->length * 2);
    if (str->string != NULL) {
        str->allocSize = str->allocSize * 2;
        return true;
    }
    errorExit(INTERNAL_ERROR, "Memory could not be allocated\n");
    return false;
}

// add one character at the end of the string
bool stringConcatenate(String_t *str, const char *addon_string) {
    int str_len = strlen(addon_string);
    if (str != NULL && str->string != NULL) {
        if (str->length + str_len > str->allocSize - 1) {
            if (!resizeString(str)) {
                return false;
            }
        }
        strncat(str->string, addon_string, str_len);
        str->length += str_len;
        str->string[str->length] = '\0';
    }
    // errorExit(INTERNAL_ERROR, "Memory could not be allocated\n");
    return true;
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
    // errorExit(INTERNAL_ERROR, "Memory could not be allocated\n");
    return false;
}

bool stringCopy(String_t *dest, String_t *source) {
    if (dest == NULL || source == NULL) {
        return false;
    }
    for (int i = 0; i < source->length; i++) {
        if (!stringAppend(dest, source->string[i])) {
            return false;
        }
    }
    return true;
}

void stringClear(String_t *str) {
    str->length = 0;
    memset(str->string, 0, strlen(str->string));
    str->string[0] = '\0';
}

void stringDeconstruct(String_t *str) { free(str->string); }