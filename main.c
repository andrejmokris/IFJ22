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
#include <stdio.h>
#include "main.h"

void _testik()
{
}

int main()
{
    Lexemes l = {0};
    while (l.Type_of_lexeme != LEX_EOF)
    {
        l = get_lexemes();
        puts(output_lexeme_str(l));
    }
    return 0;
}