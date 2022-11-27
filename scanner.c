/*
 * IFJ 2022 PROJECT
 *
 * File: scanner.c
 *
 *
 */
#include "scanner.h"

int get_Token(String_t *str) {
    int edge_sign = 0;
    int input_state = START_STATE;

    bool numInDecPart =
        false;  // check if any number is after . in decimal part

    char escape_seq_oct[5];  // Field for octal escape sequnces
    char escape_seq_hex[5];  // Field for hex escape sequnces

    stringClear(str);

    while (true) {
        edge_sign = fgetc(stdin);
        switch (input_state) {
            case START_STATE:
                if (edge_sign == EOF) {
                    return LEX_EOF;
                }
                if (isspace(edge_sign)) {
                    break;
                } else {
                    switch (edge_sign) {
                        case '+':
                            stringAppend(str, edge_sign);
                            return LEX_ADD;
                        case '-':
                            stringAppend(str, edge_sign);
                            return LEX_SUB;
                        case '*':
                            stringAppend(str, edge_sign);
                            return LEX_MUL;
                        case '(':
                            stringAppend(str, edge_sign);
                            return LEX_LPAR;
                        case ')':
                            stringAppend(str, edge_sign);
                            return LEX_RPAR;
                        case '}':
                            return LEX_RCRB;
                        case '{':
                            return LEX_LCRB;
                        case ';':
                            return LEX_SEMICOL;
                        case ',':
                            return LEX_COMMA;
                        case '.':
                            stringAppend(str, edge_sign);
                            return LEX_DOT;
                        case ':':
                            return LEX_COLON;
                        case '?':
                            stringAppend(str, edge_sign);
                            if ((edge_sign = fgetc(stdin)) == '>') {
                                return LEX_EPILOG;
                            } else {
                                ungetc(edge_sign, stdin);
                                input_state = ID_STATE;
                                break;
                            }
                        case '/':
                            input_state = DIV_STATE;
                            break;
                        case '<':
                            input_state = LESSER_STATE;
                            break;
                        case '$':
                            input_state = VAR0_STATE;
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
                            if (isalpha(edge_sign) || edge_sign == '_') {
                                input_state = ID_STATE;
                                stringAppend(str, edge_sign);
                            } else if (isdigit(edge_sign)) {
                                input_state = INT0_STATE;
                                stringAppend(str, edge_sign);
                            } else {
                                input_state = START_STATE;
                                return LEX_ERR;
                            }
                    }
                    break;
                }
                break;

            // COMMENTS, BLOCK COMMENTS
            case DIV_STATE:
                if (edge_sign == '/') {
                    input_state = COMM_LINE_STATE;
                } else if (edge_sign == '*') {
                    input_state = COMM_BLOCK_STATE;
                } else {
                    ungetc(edge_sign, stdin);
                    return LEX_DIV;
                }
                break;
            case COMM_LINE_STATE:
                if (edge_sign == '\n' || edge_sign == EOF) {
                    input_state = START_STATE;
                }
                break;
            case COMM_BLOCK_STATE:
                if (edge_sign == '*') {
                    int newChar;
                    if ((newChar = fgetc(stdin)) ==
                        '/')  // block comment detected, back to START state
                    {
                        input_state = START_STATE;
                    } else {
                        ungetc(newChar,
                               stdin);  // return character back to stdin
                    }
                } else if (edge_sign == EOF) {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
                break;

            // STRINGS
            case STRING0_STATE:
                if (edge_sign == EOF || edge_sign == '\n' || edge_sign < 31 ||
                    edge_sign == '$') {
                    input_state = START_STATE;
                    return LEX_ERR;
                } else if (edge_sign == ' ') {
                    stringAppend(str, '\\');
                    stringAppend(str, '0');
                    stringAppend(str, '3');
                    stringAppend(str, '2');
                    input_state = STRING0_STATE;
                    break;
                } else if (edge_sign == '\\') {
                    input_state = STRING_ESCAPE_STATE;
                } else if (edge_sign != '"') {
                    input_state = STRING0_STATE;
                    stringAppend(str, edge_sign);
                    break;
                } else {
                    input_state = START_STATE;
                    return LEX_STRING;
                }
                break;

            case STRING_ESCAPE_STATE:
                if (edge_sign == '"') {
                    stringAppend(str, edge_sign);
                    input_state = STRING0_STATE;
                } else if (edge_sign == '$') {
                    stringAppend(str, edge_sign);
                    input_state = STRING0_STATE;
                } else if (edge_sign == '\\') {
                    stringAppend(str, '\\');
                    stringAppend(str, '0');
                    stringAppend(str, '9');
                    stringAppend(str, '2');
                    input_state = STRING0_STATE;
                } else if (edge_sign == 'n') {
                    stringAppend(str, '\\');
                    stringAppend(str, '0');
                    stringAppend(str, '1');
                    stringAppend(str, '0');
                    input_state = STRING0_STATE;
                } else if (edge_sign == 't') {
                    stringAppend(str, '\\');
                    stringAppend(str, '0');
                    stringAppend(str, '0');
                    stringAppend(str, '9');
                    input_state = STRING0_STATE;
                } else if (edge_sign == '\\') {
                    stringAppend(str, edge_sign);
                    input_state = STRING0_STATE;
                } else if (edge_sign == 'x') {
                    input_state = HEX_ESCAPE_STATE2;
                } else if (edge_sign >= '0' && edge_sign <= '7') {
                    escape_seq_oct[0] = (char)edge_sign;
                    input_state = OCTAL_ESCAPE_STATE2;
                } else {
                    stringAppend(str, '\\');
                    stringAppend(str, edge_sign);
                    input_state = STRING0_STATE;
                }
                break;

            case HEX_ESCAPE_STATE2:
                if ((edge_sign >= '0' && edge_sign <= '9') ||
                    (edge_sign >= 'a' && edge_sign <= 'f') ||
                    (edge_sign >= 'A' && edge_sign <= 'F')) {
                    escape_seq_hex[0] = (char)edge_sign;
                    input_state = HEX_ESCAPE_STATE3;
                } else {
                    return LEX_ERR;
                }
                break;

            case HEX_ESCAPE_STATE3:
                if ((edge_sign >= '0' && edge_sign <= '9') ||
                    (edge_sign >= 'a' && edge_sign <= 'f') ||
                    (edge_sign >= 'A' && edge_sign <= 'F')) {
                    escape_seq_hex[1] = (char)edge_sign;
                    escape_seq_hex[2] = '\0';
                    char *ptr_hex_helper;
                    long hex =
                        strtol(escape_seq_hex, &ptr_hex_helper,
                               16);  // convert string literal to long integer
                    if (hex == 0 ||
                        hex > 255)  // escape octal sequence \000 is invalid
                    {
                        // input_state = START_STATE;
                        return LEX_ERR;
                    }
                    if (hex == 10) {
                        stringAppend(str, '\\');
                        stringAppend(str, '0');
                        stringAppend(str, '1');
                        stringAppend(str, '0');
                        input_state = STRING0_STATE;
                    } else if (hex == 9) {
                        stringAppend(str, '\\');
                        stringAppend(str, '0');
                        stringAppend(str, '0');
                        stringAppend(str, '9');
                        input_state = STRING0_STATE;
                    } else if (hex == 92) {
                        stringAppend(str, '\\');
                        stringAppend(str, '0');
                        stringAppend(str, '9');
                        stringAppend(str, '2');
                    } else {
                        stringAppend(str, hex);
                    }
                    input_state = STRING0_STATE;
                } else {
                    return LEX_ERR;
                }
                break;

            case OCTAL_ESCAPE_STATE2:
                if (edge_sign >= '0' && edge_sign <= '7') {
                    escape_seq_oct[1] = (char)edge_sign;
                    input_state = OCTAL_ESCAPE_STATE3;
                } else {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
                break;

            case OCTAL_ESCAPE_STATE3:
                if (edge_sign >= '0' && edge_sign <= '7') {
                    escape_seq_oct[2] = edge_sign;
                    char *ptr_octal_helper;
                    escape_seq_oct[3] = '\0';
                    long octal =
                        strtol(escape_seq_oct, &ptr_octal_helper,
                               8);   // convert string literal to long integer
                    if (octal == 0)  // escape octal sequence \000 is invalid
                    {
                        // input_state = START_STATE;
                        return LEX_ERR;
                    }
                    if (octal == 10) {
                        stringAppend(str, '\\');
                        stringAppend(str, '0');
                        stringAppend(str, '1');
                        stringAppend(str, '0');
                        input_state = STRING0_STATE;
                    } else if (octal == 9) {
                        stringAppend(str, '\\');
                        stringAppend(str, '0');
                        stringAppend(str, '0');
                        stringAppend(str, '9');
                        input_state = STRING0_STATE;
                    } else if (octal == 92) {
                        stringAppend(str, '\\');
                        stringAppend(str, '0');
                        stringAppend(str, '9');
                        stringAppend(str, '2');
                    } else {
                        stringAppend(str, octal);
                    }
                    input_state = STRING0_STATE;
                    input_state = STRING0_STATE;
                } else {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
                break;

            // VARIABLES
            case VAR0_STATE:
                if (isalpha(edge_sign) || edge_sign == '_') {
                    stringAppend(str, edge_sign);
                    input_state = VAR_STATE;
                    break;
                } else {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
            case VAR_STATE:
                if (isalnum(edge_sign) || edge_sign == '_') {
                    stringAppend(str, edge_sign);
                    break;
                } else {
                    ungetc(edge_sign, stdin);
                    input_state = START_STATE;
                    return LEX_ID;
                }
                break;

            // IDENTIFIERS
            case ID_STATE:
                if (isalnum(edge_sign) || edge_sign == '_') {
                    stringAppend(str, edge_sign);
                    break;
                } else {
                    ungetc(edge_sign, stdin);
                    input_state = START_STATE;
                    if (str->string[0] == '?') {
                        if (!strcmp(str->string, "?int")) {
                            return LEX_TYPE_INT_OPT;
                        } else if (!strcmp(str->string, "?float")) {
                            return LEX_TYPE_FLOAT_OPT;
                        } else if (!strcmp(str->string, "?string")) {
                            return LEX_TYPE_STRING_OPT;
                        } else {
                            return LEX_ERR;
                        }
                    }
                    if (!strcmp(str->string, "if")) {
                        return LEX_IF;
                    } else if (!strcmp(str->string, "else")) {
                        return LEX_ELSE;
                    } else if (!strcmp(str->string, "function")) {
                        return LEX_FUNKW;
                    } else if (!strcmp(str->string, "while")) {
                        return LEX_WHILE;
                    } else if (!strcmp(str->string, "void")) {
                        return LEX_VOID;
                    } else if (!strcmp(str->string, "null")) {
                        return LEX_NULL;
                    } else if (!strcmp(str->string, "return")) {
                        return LEX_RETURN;
                    } else if (!strcmp(str->string, "int")) {
                        return LEX_TYPE_INT;
                    } else if (!strcmp(str->string, "float")) {
                        return LEX_TYPE_FLOAT;
                    } else if (!strcmp(str->string, "string")) {
                        return LEX_TYPE_STRING;
                    }
                    return LEX_FUNID;
                }

            // =, ==, !=
            case ASSIGN_STATE:
                if (edge_sign == '=') {
                    input_state = ASSIGN_STATE1;
                    break;
                }
                ungetc(edge_sign, stdin);
                input_state = START_STATE;
                return LEX_ASSIGN;

            case NOT_STATE:
                if (edge_sign == '=') {
                    int new_char = fgetc(stdin);
                    if (new_char != '=') {
                        input_state = START_STATE;
                        return LEX_ERR;
                    }
                    input_state = START_STATE;
                    return LEX_NEQ;
                } else {
                    input_state = START_STATE;
                    return LEX_ERR;
                }

            case ASSIGN_STATE1:
                if (edge_sign == '=') {
                    input_state = START_STATE;
                    return LEX_EQ;
                } else {
                    input_state = START_STATE;
                    return LEX_ERR;
                }

            // <, >, <=, =>
            case LESSER_STATE:
                input_state = START_STATE;
                if (edge_sign == '=') {
                    return LEX_LEQ;
                } else {
                    ungetc(edge_sign, stdin);
                    return LEX_LE;
                }

            case GREATER_STATE:
                input_state = START_STATE;
                if (edge_sign == '=') {
                    return LEX_GTQ;
                } else {
                    ungetc(edge_sign, stdin);
                    return LEX_GT;
                }

            // NUMBERS
            case INT0_STATE:
                if (isdigit(edge_sign)) {
                    stringAppend(str, edge_sign);
                    break;
                } else if (edge_sign == '.') {
                    stringAppend(str, edge_sign);
                    input_state = FLOAT0_STATE;
                    break;
                } else if (edge_sign == 'E' || edge_sign == 'e') {
                    stringAppend(str, edge_sign);
                    input_state = EXPONENT_STATE0;
                    break;
                } else {
                    input_state = START_STATE;
                    ungetc(edge_sign, stdin);
                    return LEX_INT;
                }

            case FLOAT0_STATE:
                if (isdigit(edge_sign)) {
                    numInDecPart = true;
                    stringAppend(str, edge_sign);
                    break;
                } else if (edge_sign == '.') {
                    input_state = START_STATE;
                    return LEX_ERR;
                } else if (edge_sign == 'e' || edge_sign == 'E') {
                    if (numInDecPart) {
                        stringAppend(str, edge_sign);
                        input_state = EXPONENT_STATE0;
                        break;
                    } else {
                        input_state = START_STATE;
                        return LEX_ERR;
                    }
                } else {
                    if (!numInDecPart) {
                        input_state = START_STATE;
                        return LEX_ERR;
                    }
                    input_state = START_STATE;
                    ungetc(edge_sign, stdin);
                    return LEX_FLOAT;
                }

            case EXPONENT_STATE0:
                if (edge_sign == '-' || edge_sign == '+') {
                    input_state = EXPONENT_STATE2;
                    stringAppend(str, edge_sign);
                    break;
                } else if (edge_sign == 'e' || edge_sign == 'E' ||
                           !isdigit(edge_sign)) {
                    input_state = START_STATE;
                    return LEX_ERR;
                } else if (isdigit(edge_sign)) {
                    input_state = EXPONENT_STATE2;
                    stringAppend(str, edge_sign);
                    break;
                } else {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
                break;

            case EXPONENT_STATE1:
                if (isdigit(edge_sign)) {
                    input_state = EXPONENT_STATE2;
                    stringAppend(str, edge_sign);
                    break;
                } else {
                    input_state = START_STATE;
                    return LEX_ERR;
                }
                break;

            case EXPONENT_STATE2:
                if (isdigit(edge_sign)) {
                    stringAppend(str, edge_sign);
                } else {
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

bool checkProlog(String_t *str) {
    int startLex = get_Token(str);
    if (startLex != LEX_LE) {
        return false;
    }
    startLex = fgetc(stdin);
    if (startLex != '?') {
        return false;
    }
    int curChar = get_Token(str);
    if (curChar != LEX_FUNID || strcmp(str->string, "php")) {
        return false;
    }
    if ((get_Token(str) != LEX_FUNID || strcmp(str->string, "declare")) ||
        (get_Token(str) != LEX_LPAR)) {
        return false;
    }
    if ((get_Token(str) != LEX_FUNID || strcmp(str->string, "strict_types")) ||
        (get_Token(str) != LEX_ASSIGN)) {
        return false;
    }
    if ((get_Token(str) != LEX_INT || strcmp(str->string, "1")) ||
        (get_Token(str) != LEX_RPAR) || (get_Token(str) != LEX_SEMICOL)) {
        return false;
    }
    return true;
}