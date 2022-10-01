/*
* IFJ 2022 PROJECT
*
* File: scanner.h
*
* Scanner library
*/

#include <stdio.h>

typedef enum{
    START_STATE,

    VAR0_STATE, // variable identifier
    VAR_STATE,
    ID_FUN_STATE, // function identifier
    /*ID0_TYPE_STATE, //with optional ? before
    ID1_TYPE_STATE,*/
    /*STRING0_STATE,
    STRING1_STATE,
    STRING21_STATE,
    STRING22_STATE,
    STRING3_STATE,*/
    /*INT_PLUS_STATE, //int with + before
    INT_STATE,*/
    /*FLOAT0_STATE,
    FLOAT1_STATE,
    FLOAT_eE_STATE,
    FLOAT_FINAL_STATE,*/
    SUM_STATE,      // +
    SUB_STATE,      // -
    DIV_STATE,      // *
    MUL_STATE,      // /
    LPAR_STATE,     // (
    RPAR_STATE,     // )
    ASSIGN_STATE,   // =
    GREATER_STATE,  // >
    LESSER_STATE,   // <
    NOT_STATE,      // !
    DOT_STATE,      // .
    COMMMA_STATE,   // ,
    SEMICOL_STATE,  // ;
    ERROR_STATE
} FSM_States;

typedef struct{
    enum{
    LEX_ERR,    // Error
    LEX_ADD,    // +
    LEX_SUB,    // -
    LEX_MUL,    // *
    LEX_DIV,    // /
    LEX_GT,     // >
    LEX_LE,     // <
    LEX_GTQ,    // >=
    LEX_LEQ,    // <=
    LEX_EQ,     // ==
    LEX_NEQ,    // !=
    LEX_ASSIGN, // =
    LEX_COMMA,  // ,
    LEX_RPAR,   // )
    LEX_LPAR,   // (
    LEX_EOL,    // '\n'
    LEX_EOF,    // EOF
    LEX_ID,     // Identifier
    LEX_KEY,    // Keyword (if, while...)
    LEX_INT,    // int
    LEX_FLOAT,  // float
    LEX_STRING  // String
    }Type;
}Lexeme_Type;


FSM_States transition(FSM_States input, int edge_sign);
