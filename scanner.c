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

FILE *source_to_parse;

String_t *dyn_string; // Inicialization of dynamic string

// inicialization of variables which we will use
String_t work_string;
String_t *str = &work_string;
char escape_seq_oct[5]; //Field for octal escape sequnces
char escape_seq_hex[5]; //Field for hex escape sequnces

FSM_States state_transition(FSM_States input, int edge_sign)
{
    switch (input)
    {
    case START_STATE:
        if (edge_sign == '$')
            return VAR0_STATE;
        else if (isalpha(edge_sign) || edge_sign == '_')
            return ID_STATE; // function, or keyword, variable identifier
        else if (isalpha(edge_sign) || edge_sign == '_')
            return ID_OF_TYPE0; //string, float, int or ?string, ?float, ?int    
        else if (edge_sign == '+')
            return SUM_STATE;
        else if (edge_sign == '-')
            return SUB_STATE;
        else if (edge_sign == '*')
            return MUL_STATE;
        else if (edge_sign == '/')
            return DIV_STATE;
        else if (edge_sign == '=')
            return ASSIGN_STATE;
        else if (edge_sign == '!')
            return NOT_STATE;
        else if (edge_sign == '(')
            return LPAR_STATE;
        else if (edge_sign == ')')
            return RPAR_STATE;
        else if (edge_sign == '>')
            return GREATER_STATE;
        else if (edge_sign == '<')
            return LESSER_STATE;
        else if (edge_sign == ',')
            return COMMMA_STATE;
        else if (edge_sign == '.')
            return DOT_STATE;
        else if (edge_sign == ';')
            return SEMICOL_STATE;
        else if (edge_sign == '"') //String
            return STRING0_STATE;
        else if (edge_sign == EOF)
            return EOF_STATE;
        else
            return ERROR_STATE;
        break;

    //VARIABLES
    case VAR0_STATE:
        if (isalpha(edge_sign) || edge_sign == '_')
            return VAR_STATE;
        else
            return ERROR_STATE;
        break;
    case VAR_STATE:
        if (isalpha(edge_sign) || isdigit(edge_sign) || edge_sign == '_')
            return VAR_STATE;
        else
            return ERROR_STATE;
        break;
        
    break;

    //STRINGS
    case STRING0_STATE:
        if (edge_sign < 31 || edge_sign == '\n' || edge_sign == EOF)
        {
            return ERROR_STATE;
        }
        else if (edge_sign == '\\') // Escape sequence
        {
            return STRING_ESCAPE_STATE;
        }
        else if (edge_sign == '"') // empty string literal
        {   //final state
            return EMPTY_STRING_STATE;
        }
        else
        {
            return STRING0_STATE;
        }
        break;
    
    case STRING_ESCAPE_STATE:
        if (edge_sign == '"')
        {
            return STRING0_STATE;
        }
        else if (edge_sign == 'n')
        {
            return STRING0_STATE;
        }
        else if (edge_sign == 't')
        {
            return STRING0_STATE;
        }
        else if (edge_sign == '\\')
        {
            return STRING0_STATE;
        }
        else if (edge_sign == 'x')
        {
            return STRING_ESCAPE_STATE1;
        }else if (edge_sign >= '0' && edge_sign <= '7')
        {
            escape_seq_oct[0] = (char) edge_sign;
            return STRING_ESCAPE_STATE1;
        }
        else if (edge_sign >= 'A' && edge_sign <= 'F' )
        {
            escape_seq_hex[0] = (char) edge_sign;
            return STRING_ESCAPE_STATE1;
        }else
        {
            return ERROR_STATE;
        }
        break;

     case STRING_ESCAPE_STATE1:
        if ( edge_sign >= '0' && edge_sign <= '7')
        {
            escape_seq_oct[1] = (char) edge_sign;
            return STRING_ESCAPE_STATE2;
        }
        else if ( edge_sign >= 'A' && edge_sign <= 'F')
        {
            escape_seq_hex[1] = (char) edge_sign;
            return STRING_ESCAPE_STATE2;
        }else
        {
            return ERROR_STATE;
        }
        break;

     case STRING_ESCAPE_STATE2:
        if ( edge_sign >= '0' && edge_sign <= '7')
        {
            char *ptr_octal_helper = NULL;
            escape_seq_oct[2] = (char) edge_sign;
            escape_seq_oct[3] = '\0';
            int octal = strtol(escape_seq_oct, &ptr_octal_helper, 8); //konvert string literal to long integer 
            if(octal == 0) //escape octal sequence \000 is invalid
            {
                return ERROR_STATE;
            }
            return STRING0_STATE;
        }
        else if ( edge_sign >= 'A' && edge_sign <= 'F')
        {
            char *ptr_hex_helper = NULL;
            escape_seq_hex[2] = (char) edge_sign;
            escape_seq_hex[3] = '\0';
            int hex = strtol(escape_seq_oct, &ptr_hex_helper, 8); //konvert string literal to long integer 
            if(hex == 0) //escape octal sequence \000 is invalid
            {
                return ERROR_STATE;
            }
            return STRING0_STATE;
        }else
        {
            return ERROR_STATE;
        }
        break;

    //KEYWORD OR FUNCTIONS
    case ID_STATE:
        if (isalpha(edge_sign) || edge_sign == '_')
            return ID_STATE;
        else
            return ERROR_STATE;
        break;

    case ERROR_STATE:
        fprintf(stderr, "Scenner error.");
        return ERROR_STATE;
        break;
    } // switch
    return ERROR_STATE;
} // function

Lexemes make_lexemes(FSM_States End_state, char *Token)
{
    switch (End_state)
    {
    case RPAR_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_RPAR};

    case LPAR_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_LPAR};

    case SEMICOL_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_SEMICOL};

    case ASSIGN_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_ASSIGN};

    case ID_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_FUNID};

    case STRING0_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_STRING};

    case VAR0_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_ID};

    case VAR_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_ID};
    
    case EMPTY_STRING_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_STRING};

    case ERROR_STATE:
        return (Lexemes){.Type_of_lexeme = LEX_ERR};

    default:
        exit(99); // TODO ERROR HANDLING
        break;
    }
}

Lexemes get_lexemes()
{

    if (!StringInit(str))
    {
        fprintf(stderr, "Error with dynamic string 2!"); // TODO ERROR HANDLING
        exit(99);
    }

    FSM_States FSM_current_state = START_STATE;
    char *start_of_lexeme = (char *)str;

    while (true)
    {
        int edge = getchar(); // needed int to analyze EOF
        if (edge == EOF)
        {
            if (FSM_current_state == START_STATE)
            {
                return (Lexemes){.Type_of_lexeme = LEX_EOF};
            }
            return make_lexemes(FSM_current_state, start_of_lexeme);
        }

        FSM_States FSM_next_state = state_transition(FSM_current_state, edge);
        if (FSM_next_state == ERROR_STATE)
        {
            ungetc(edge, stdin); // we havent used edge sign to built a lexeme, so we need to return it back
            resizeString(str);
            return make_lexemes(FSM_current_state, start_of_lexeme);
        }
        // if we proceeded and used a char to create a lexeme, we will add this char to our dynamic string
        stringAppend(str, edge);
        FSM_current_state = FSM_next_state; // next state
    }
}

char *output_lexeme_str(Lexemes input)
{
    switch (input.Type_of_lexeme)
    {
    case LEX_RPAR:
        return ")";

    case LEX_LPAR:
        return "(";

    case LEX_SEMICOL:
        return ";";

    case LEX_ASSIGN:
        return "=";

    case LEX_FUNID:
        return work_string.string + input.data;

    case LEX_STRING:
        return work_string.string + input.data;

    case LEX_ID:
        return work_string.string + input.data;
    case LEX_ERR:
    fprintf(stderr,"Lexikalny error");
    }
    return "ERROR";
}

/*int main()
{
    Lexemes l = {0};
    while (l.Type_of_lexeme != LEX_EOF)
    {
        l = get_lexemes();
        puts(output_lexeme_str(l));
    }
    return 0;
}*/