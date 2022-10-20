#ifndef _DYN_STRING
#define _DYN_STRING

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_ARR_SIZE 8  // Default string length

typedef struct {
    char *string;
    int length;     // length of string/ number of chars
    int allocSize;  // Previously allocated size of string

} String_t;

bool StringInit(String_t *str);
bool resizeString(String_t *str);
bool stringAppend(String_t *str, int c);
void stringClear(String_t *str);
void stringDeconstruct(String_t *str);

#endif
