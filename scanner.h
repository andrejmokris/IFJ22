/*
 * IFJ 2022 PROJECT
 *
 * scanner.h
 *
 * Adam Pap: xpapad11
 *
 */

#ifndef _SCANNER
#define _SCANNER

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"

typedef enum {
    START_STATE,
    VAR0_STATE,  // variable identifier
    VAR_STATE,
    ID_STATE,       // function, or keyword, variable identifier
    STRING0_STATE,  // String states
    STRING_ESCAPE_STATE,  // escape sequences
    STRING_ESCAPE_STATE1,
    STRING_ESCAPE_STATE2,
    COMM_LINE_STATE,  // Comments
    COMM_BLOCK_STATE,
    INT0_STATE,  // Numbers states, int, float, exponents
    FLOAT0_STATE,
    EXPONENT_STATE0,
    EXPONENT_STATE2,
    SUM_STATE,     // +
    SUB_STATE,     // -
    DIV_STATE,     // *
    MUL_STATE,     // /
    LPAR_STATE,    // (
    RPAR_STATE,    // )
    ASSIGN_STATE,  // =
    ASSIGN_STATE1,
    GREATER_STATE,  // >
    LESSER_STATE,   // <
    NOT_STATE,      // !
    DOT_STATE,      // .
    COMMMA_STATE,   // ,
    SEMICOL_STATE,  // ;
    EOF_STATE,
    OCTAL_ESCAPE_STATE2,
    OCTAL_ESCAPE_STATE3,
    HEX_ESCAPE_STATE2,
    HEX_ESCAPE_STATE3
} FSM_States;

enum {
    LEX_ERR,       // Error   0
    LEX_ADD,       // +       1
    LEX_SUB,       // -       2
    LEX_MUL,       // *       3
    LEX_DIV,       // /       4
    LEX_EQ,        // ===     5
    LEX_NEQ,       // !==     6
    LEX_LE,        // <       7
    LEX_GT,        // >       8
    LEX_LEQ,       // <=      9
    LEX_GTQ,       // >=      10
    LEX_LPAR,      // (       11
    LEX_RPAR,      // )       12
    LEX_I,         // i       13 
    LEX_DOLLAR,    // $       14
    LEX_DOT,       // .       15
    LEX_ASSIGN,    // =       16
    LEX_COMMA,     // ,       17
    LEX_SEMICOL,   // ;       18
    LEX_RCRB,      // }       19
    LEX_LCRB,      // {       20
    LEX_QM,        // ?       21
    LEX_COLON,     // :       22
    LEX_EOL,       // '\n'    23
    LEX_EOF,       // EOF     24
    LEX_ID,        // ID      25
    LEX_FUNID,     //         26
    LEX_FUNKW,     // Func    27
    LEX_IF,        //         28
    LEX_ELSE,      //         29
    LEX_WHILE,     //         30
    LEX_DO,        //         31
    LEX_INT,       // int     32
    LEX_FLOAT,     // float   33
    LEX_EXPONENT,  //         34
    LEX_STRING,    // String  35
    LEX_VOID,      //         36
    LEX_NULL,      //         37
    LEX_RETURN,    //         38
    LEX_BEG_HANDLE,//         39
    LEX_E,         //         40
    LEX_BOOL,      //         41
    LEX_TYPE_STRING,//        42
    LEX_TYPE_FLOAT,//         43
    LEX_TYPE_INT, //          44
    LEX_TYPE_STRING_OPT,//    45
    LEX_TYPE_FLOAT_OPT,//     46
    LEX_TYPE_INT_OPT,//       47
    LEX_EPILOG,  //            48
} Type_of_lexeme;

int get_Token(String_t *str);
bool checkProlog(String_t *str);
bool checkEpilog();

#endif