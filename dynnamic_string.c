/*
* IFJ 2022 PROJECT
*
* File: dynamic_string.c
*
* Dynamic string file
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULT_ARR_SIZE 8 //Default string length 8 bits

typedef struct 
{
    char *string;
    int length_of_string; //length of string/ number of chars
    int array_size_alloc; //Array size which need to be allocated

}Dyn_Str_Alloc;

bool Dynamic_strings_init(Dyn_Str_Alloc *str)
{
    if ((str->length_of_string = (char *) calloc(DEFAULT_ARR_SIZE, sizeof(char))) == NULL)
    {
        fprintf(stderr,"Internal error while allocating dynamic string.\n");
        return 0;
    }
    str->length_of_string = 0;
    str->string[str->length_of_string] = '\0';

    return 1;    
}

bool Dynamic_string_add_char(Dyn_Str_Alloc *str, char s)
{
    if (str != NULL)
    {
        
    }
    
}

void Dynamic_strings_free(Dyn_Str_Alloc *str)
{

}