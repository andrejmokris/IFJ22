/*
 * IFJ 2022 PROJECT
 *
 * File: scanner.h
 *
 * Scanner library
 */

#include "dynamic_string.h"

typedef enum
{
    START_STATE,

    VAR0_STATE, // variable identifier
    VAR_STATE,
    ID_STATE,    // function, or keyword, variable identifier
    STRING0_STATE, // String states
    STRING1_STATE,
    EMPTY_STRING_STATE,
    STRING_ESCAPE_STATE, // escape sequences
    STRING_ESCAPE_STATE1,
    STRING_ESCAPE_STATE2,
    COMM_LINE_STATE, // Comments
    COMM_BLOCK_STATE,
    COMM_BLOCK_STATE_FIN,
    INT0_STATE, // Numbers states, int, float, exponents
    INT1_STATE,
    FLOAT0_STATE,
    FLOAT1_STATE,
    EXPONENT_STATE0,
    EXPONENT_STATE1,
    EXPONENT_STATE2,
    EXPONENT_NUM_STATE,
    SUM_STATE,    // +
    SUB_STATE,    // -
    DIV_STATE,    // *
    MUL_STATE,    // /
    LPAR_STATE,   // (
    RPAR_STATE,   // )
    ASSIGN_STATE, // =
    ASSIGN_STATE1,
    GREATER_STATE, // >
    LESSER_STATE,  // <
    NOT_STATE,     // !
    DOT_STATE,     // .
    COMMMA_STATE,  // ,
    SEMICOL_STATE, // ;
    EOF_STATE
} FSM_States;

typedef struct
{
    enum
    {
        LEX_ERR,      // Error   0
        LEX_ADD,      // +       1
        LEX_SUB,      // -       2
        LEX_MUL,      // *       3
        LEX_DIV,      // /       4
        LEX_GT,       // >       5
        LEX_LE,       // <       6
        LEX_GTQ,      // >=      7
        LEX_LEQ,      // <=      8
        LEX_EQ,       // ==      9
        LEX_NEQ,      // !=      10
        LEX_ASSIGN,   // =       11
        LEX_COMMA,    // ,       12
        LEX_SEMICOL,  // ;       13
        LEX_RPAR,     // )       14
        LEX_LPAR,     // (       15
        LEX_RCRB,     // }       16
        LEX_LCRB,     // {       17
        LEX_QM,       // ?       18
        LEX_COLON,    // :       19
        LEX_DOT,      // .       20
        LEX_EOL,      // '\n'    21
        LEX_EOF,      // EOF     22
        LEX_ID,       // Identifier  23
        LEX_FUNID,    // Function identifier 24
        LEX_IF,       //         25
        LEX_ELSE,     //         26
        LEX_WHILE,    //         27
        LEX_DO,       //         28
        LEX_INT,      // int     29
        LEX_FLOAT,    // float   30
        LEX_EXPONENT, // exponent number 31
        LEX_STRING,   // String  32
        LEX_VOID,     //         33
        LEX_NULL,     //         34
        LEX_RETURN    //         35
    } Type_of_lexeme;
    size_t data;
} Lexemes; // tokens

int get_Token(String_t *str);
// FSM_States transition(FSM_States input, int edge_sign);

// Lexemes make_lexemes(FSM_States End_state, char* Token);

// Lexemes get_lexemes();

// char *output_lexeme_str(Lexemes input);
