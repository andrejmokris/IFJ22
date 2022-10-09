/*
* IFJ 2022 PROJECT
*
* File: scanner.h
*
* Scanner library
*/

#include "dynamic_string.h"

typedef enum{
    START_STATE,

    VAR0_STATE, // variable identifier
    VAR_STATE,
    ID_STATE, // function, or keyword, variable identifier
    ID_OF_TYPE0, // string, float, int or ?string, ?float, ?int 
    ID_OF_TYPE1,
    STRING0_STATE,
    STRING1_STATE,
    STRING_ESCAPE_STATE, //escape sequences
    STRING_ESCAPE_STATE1,
    STRING_ESCAPE_STATE2,
    EMPTY_STRING_STATE, // String "" - empty

    COMM_LINE_STATE,
    COMM_BLOCK_STATE,
    COMM_BLOCK_STATE_FIN,

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
    EOF_STATE,
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
    LEX_SEMICOL,// ;
    LEX_RPAR,   // )
    LEX_LPAR,   // (
    LEX_EOL,    // '\n'
    LEX_EOF,    // EOF
    LEX_ID,     // Identifier
    LEX_FUNID,  // Function identifier
    //LEX_KEY,    // Keyword (if, while...)
    LEX_IF,
    LEX_WHILE,
    LEX_DO,
    LEX_INT,    // int
    LEX_FLOAT,  // float
    LEX_STRING  // String
    }Type_of_lexeme;
    size_t data;
}Lexemes; //tokens


FSM_States transition(FSM_States input, int edge_sign);

Lexemes make_lexemes(FSM_States End_state, char* Token);

Lexemes get_lexemes();

char *output_lexeme_str(Lexemes input);
