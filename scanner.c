/*
 * IFJ 2022 PROJECT
 *
 * File: scanner.c
 *
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

int get_Token(String_t *str)
{
    int edge_sign = 0;
    int input_state = START_STATE;

    bool numInDecPart = false; // check if any number is after . in decimal part

    char escape_seq_oct[5]; // Field for octal escape sequnces
    char escape_seq_hex[5]; // Field for hex escape sequnces

    // stringClear(str);

    while (true)
    {
        edge_sign = fgetc(stdin);
        switch (input_state)
        {
        case START_STATE:
            if (edge_sign == EOF)
            {
                return LEX_EOF;
            }
            if (isspace(edge_sign))
            {
                break;
            }
            else
            {
                switch (edge_sign)
                {
                case '+':
                    return LEX_ADD;
                case '-':
                    return LEX_SUB;
                case '*':
                    return LEX_MUL;
                case '(':
                    return LEX_LPAR;
                case ')':
                    return LEX_RPAR;
                case '}':
                    return LEX_RCRB;
                case '{':
                    return LEX_LCRB;
                case ';':
                    return LEX_SEMICOL;
                case ',':
                    return LEX_COMMA;
                case '?':
                    return LEX_QM;
                case ':':
                    return LEX_COLON;
                case '/':
                    input_state = DIV_STATE;
                    break;
                case '<':
                    input_state = LESSER_STATE;
                    break;
                case '$':
                    input_state = VAR_STATE;
                    break;
                case '>':
                    input_state = GREATER_STATE;
                    break;
                case '"':
                    input_state = STRING0_STATE;
                    break;
                case '=':
                    input_state = ASSIGN_STATE;
                    break;
                case '!':
                    input_state = NOT_STATE;
                    break;
                default:
                    if (isalpha(edge_sign) || edge_sign == '_')
                    {
                        input_state = ID_STATE;
                        stringAppend(str, edge_sign);
                    }
                    else if (isdigit(edge_sign))
                    {
                        input_state = INT0_STATE;
                        stringAppend(str, edge_sign);
                    }
                    else
                    {
                        input_state = START_STATE;
                        return LEX_ERR;
                    }
                }
                break;
            }
            break;

        // COMMENTS, BLOCK COMMENTS
        case DIV_STATE:
            if (edge_sign == '/')
            {
                input_state = COMM_LINE_STATE;
            }
            else if (edge_sign == '*')
            {
                input_state = COMM_BLOCK_STATE;
            }
            else
            {
                return LEX_DIV;
            }
            break;
        case COMM_LINE_STATE:
            if (edge_sign == '\n' || edge_sign == EOF)
            {
                input_state = START_STATE;
            }
            break;
        case COMM_BLOCK_STATE:
            if (edge_sign == '*')
            {
                int newChar;
                if ((newChar = fgetc(stdin)) == '/') // block comment detected, back to START state
                {
                    input_state = START_STATE;
                }
                else
                {
                    ungetc(newChar, stdin); // return character back to stdin
                }
            }
            else if (edge_sign == EOF)
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
            break;

        // STRINGS
        case STRING0_STATE:
            if (edge_sign == EOF || edge_sign == '\n' || edge_sign < 31)
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
            else if (edge_sign == '\\')
            {
                input_state = STRING_ESCAPE_STATE;
            }
            else if (edge_sign != '"')
            {
                input_state = STRING0_STATE;
                stringAppend(str, edge_sign);
                break;
            }
            else
            {
                input_state = START_STATE;
                return LEX_STRING;
            }
            break;

        case STRING_ESCAPE_STATE:
            if (edge_sign == '"')
            {
                stringAppend(str, edge_sign);
                input_state = STRING0_STATE;
            }
            else if (edge_sign == 'n')
            {
                stringAppend(str, edge_sign);
                input_state = STRING0_STATE;
            }
            else if (edge_sign == 't')
            {
                stringAppend(str, edge_sign);
                input_state = STRING0_STATE;
            }
            else if (edge_sign == '\\')
            {
                stringAppend(str, edge_sign);
                input_state = STRING0_STATE;
            }
            else if (edge_sign == 'x')
            {
                stringAppend(str, edge_sign);
                input_state = STRING0_STATE;
            }
            else if (edge_sign >= '0' && edge_sign <= '7')
            {
                escape_seq_oct[0] = (char)edge_sign;
                input_state = STRING_ESCAPE_STATE1;
            }
            else if (edge_sign >= 'A' && edge_sign <= 'F')
            {
                escape_seq_hex[0] = (char)edge_sign;
                input_state = STRING_ESCAPE_STATE1;
            }
            else
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
            break;

        case STRING_ESCAPE_STATE1:
            if (edge_sign >= '0' && edge_sign <= '7')
            {
                escape_seq_oct[1] = (char)edge_sign;
                input_state = STRING_ESCAPE_STATE2;
            }
            else if (edge_sign >= 'A' && edge_sign <= 'F')
            {
                escape_seq_hex[1] = (char)edge_sign;
                input_state = STRING_ESCAPE_STATE2;
            }
            else
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
            break;

        case STRING_ESCAPE_STATE2:
            if (edge_sign >= '0' && edge_sign <= '7')
            {
                char *ptr_octal_helper = NULL;
                escape_seq_oct[2] = (char)edge_sign;
                escape_seq_oct[3] = '\0';
                int octal = strtol(escape_seq_oct, &ptr_octal_helper, 8); // convert string literal to long integer
                if (octal == 0)                                           // escape octal sequence \000 is invalid
                {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
                input_state = START_STATE;
                return LEX_STRING;
            }
            else if (edge_sign >= 'A' && edge_sign <= 'F')
            {
                char *ptr_hex_helper = NULL;
                escape_seq_hex[2] = (char)edge_sign;
                escape_seq_hex[3] = '\0';
                int hex = strtol(escape_seq_oct, &ptr_hex_helper, 8); // convert string literal to long integer
                if (hex == 0)                                         // escape octal sequence \000 is invalid
                {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
                input_state = START_STATE;
                return LEX_STRING;
            }
            else
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
            break;

        // VARIABLES
        case VAR0_STATE:
            if (isalpha(edge_sign) || edge_sign == '_')
            {
                stringAppend(str, edge_sign);
                input_state = VAR_STATE;
                break;
            }
            else
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
        case VAR_STATE:
            if (isalnum(edge_sign) || edge_sign == '_')
            {
                stringAppend(str, edge_sign);
                break;
            }
            else
            {
                ungetc(edge_sign, stdin);
                input_state = START_STATE;
                return LEX_ID;
            }
            break;

        // IDENTIFIERS
        case ID_STATE:
            if (isalnum(edge_sign) || edge_sign == '_')
            {
                stringAppend(str, edge_sign);
                break;
            }
            else // control if a identifier is key word
            {
                ungetc(edge_sign, stdin);
                input_state = START_STATE;
                if (!strcmp(str->string, "if"))
                {
                    return LEX_IF;
                }
                else if (!strcmp(str->string, "else"))
                {
                    return LEX_ELSE;
                }
                else if (!strcmp(str->string, "function"))
                {
                    return LEX_FUNID;
                }
                else if (!strcmp(str->string, "while"))
                {
                    return LEX_WHILE;
                }
                else if (!strcmp(str->string, "void"))
                {
                    return LEX_VOID;
                }
                else if (!strcmp(str->string, "null"))
                {
                    return LEX_NULL;
                }
                else if (!strcmp(str->string, "return"))
                {
                    return LEX_RETURN;
                }
                else if (!strcmp(str->string, "int"))
                {
                    return LEX_INT;
                }
                else if (!strcmp(str->string, "float"))
                {
                    return LEX_FLOAT;
                }
                else if (!strcmp(str->string, "string"))
                {
                    return LEX_STRING;
                }
                return LEX_FUNID;
            }

        // =, ==, !=
        case ASSIGN_STATE:
            if (edge_sign == '=')
            {
                input_state = ASSIGN_STATE1;
                break;
            }
            ungetc(edge_sign, stdin);
            input_state = START_STATE;
            return LEX_ASSIGN;

        case NOT_STATE:
            if (edge_sign == '=')
            {
                input_state = START_STATE;
                return LEX_NEQ;
            }
            else
            {
                input_state = START_STATE;
                return LEX_ERR;
            }

        case ASSIGN_STATE1:
            if (edge_sign == '=')
            {
                input_state = START_STATE;
                return LEX_EQ;
            }
            else
            {
                input_state = START_STATE;
                return LEX_ERR;
            }

        // <, >, <=, =>
        case LESSER_STATE:
            input_state = START_STATE;
            if (edge_sign == '=')
            {
                return LEX_LEQ;
            }
            else
            {
                ungetc(edge_sign, stdin);
                return LEX_LE;
            }

        case GREATER_STATE:
            input_state = START_STATE;
            if (edge_sign == '=')
            {
                return LEX_GTQ;
            }
            else
            {
                ungetc(edge_sign, stdin);
                return LEX_GT;
            }

        // NUMBERS
        case INT0_STATE:
            if (isdigit(edge_sign))
            {
                stringAppend(str, edge_sign);
                break;
            }
            else if (edge_sign == '.')
            {
                stringAppend(str, edge_sign);
                input_state = FLOAT0_STATE;
                break;
            }
            else if (edge_sign == 'E' || edge_sign == 'e')
            {
                stringAppend(str, edge_sign);
                input_state = EXPONENT_STATE0;
                break;
            }
            else
            {
                input_state = START_STATE;
                ungetc(edge_sign, stdin);
                return LEX_INT;
            }

        case FLOAT0_STATE:
            if (isdigit(edge_sign))
            {
                numInDecPart = true;
                stringAppend(str, edge_sign);
                break;
            }
            else if (edge_sign == '.')
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
            else
            {
                if (!numInDecPart)
                {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
                input_state = START_STATE;
                ungetc(edge_sign, stdin);
                return LEX_FLOAT;
            }

        case EXPONENT_STATE0:
            if (edge_sign == '-' || edge_sign == '+')
            {
                input_state = EXPONENT_STATE1;
                stringAppend(str, edge_sign);
                break;
            }
            else if (edge_sign == 'e' || edge_sign == 'E')
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
            else if (isdigit(edge_sign))
            {
                input_state = EXPONENT_STATE2;
                stringAppend(str, edge_sign);
                break;
            }
            break;

        case EXPONENT_STATE1:
            if (isdigit(edge_sign))
            {
                input_state = EXPONENT_STATE2;
                stringAppend(str, edge_sign);
                break;
            }
            else
            {
                input_state = START_STATE;
                return LEX_ERR;
            }
            break;

        case EXPONENT_STATE2:
            if (isdigit(edge_sign))
            {
                stringAppend(str, edge_sign);
            }
            else
            {
                input_state = START_STATE;
                ungetc(edge_sign, stdin);
                stringAppend(str, edge_sign);
                return LEX_EXPONENT;
            }
            break;

        default:
            break;
        }
    }
}