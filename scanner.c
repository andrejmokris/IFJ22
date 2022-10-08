/*
 * IFJ 2022 PROJECT
 *
 * File: scanner.c
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "scanner.h"

FILE *source_to_parse; // Source that scanner will scan for tokens
String_t *dyn_string;  // Inicialization of dynamic string

void source_to_parse_set(FILE *s)
{
    source_to_parse = s;
}
void dynamic_string_set(String_t *string)
{
    dyn_string = string;
}

Lexemes get_lexemes(FSM_States token)
{
    if (source_to_parse == NULL)
    {
        fprintf(stderr, "Error with source file!"); // TODO ERROR HANDLING
        exit(99);
    }

    if (dyn_string == NULL)
    {
        fprintf(stderr, "Error with dynamic string!"); // TODO ERROR HANDLING
        exit(99);
    }

    // inicialization of dynamic strings
    String_t work_string;
    String_t *str = &work_string;

    if (StringInit(str) == false)
    {
        fprintf(stderr, "Error with dynamic string!"); // TODO ERROR HANDLING
        exit(99);
    }

    FSM_States FSM_current_state = START_STATE;
    int c = 0; // character from input
    // char *start_of_lexeme = str;
    while (true)
    {
        c = getc(source_to_parse); // Store individual characters from the input

        switch (FSM_current_state)
        {
        case (START_STATE):
            if (c == '$')
                FSM_current_state = VAR0_STATE;
            else if (isalpha(c) || c == '_')
            {
                if (stringAppend(str, c) == false)
                {
                    stringClear(str);
                }
                FSM_current_state = ID_FUN_STATE;
            }
            else if (c == '+')
            {
                FSM_current_state = SUM_STATE;
            }
            else if (c == '-')
                FSM_current_state = SUB_STATE;
            else if (c == '*')
                FSM_current_state = MUL_STATE;
            else if (c == '/')
                FSM_current_state = DIV_STATE;
            else if (c == '=')
                FSM_current_state = ASSIGN_STATE;
            else if (c == '!')
                FSM_current_state = NOT_STATE;
            else if (c == '(')
                FSM_current_state = LPAR_STATE;
            else if (c == ')')
                FSM_current_state = RPAR_STATE;
            else if (c == '>')
                FSM_current_state = GREATER_STATE;
            else if (c == '<')
                FSM_current_state = LESSER_STATE;
            else if (c == ',')
                FSM_current_state = COMMMA_STATE;
            else if (c == '.')
                FSM_current_state = DOT_STATE;
            else if (c == ';')
                FSM_current_state = SEMICOL_STATE;
            else
            {
                FSM_current_state = ERROR_STATE;
                exit(99); //TO DO ERROR HANDLING
            }
            break;

        /*case VAR0_STATE:
            if (isalpha(c) || c == '_')
                return VAR_STATE;
            else
                return ERROR_STATE;
            break;
        case VAR_STATE:
            if (isalpha(c) || c == '_')
                return VAR_STATE;
            else
                return ERROR_STATE;
            break;

        case ID_FUN_STATE:
            if (isalpha(c) || c == '_')
                return ID_FUN_STATE;
            else
                return ERROR_STATE;
            break;*/

        case ERROR_STATE:
            fprintf(stderr, "Scenner error.");
            FSM_current_state = ERROR_STATE;
            break;
        } // switch
    }
}
int main()
{
    return 0;
}
