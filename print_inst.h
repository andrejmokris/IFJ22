#ifndef _CODEGEN
#define _CODEGEN

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dynamic_string.h"
#include "generator.h"
#include "scanner.h"

// function ord(string $c) : int

#define PRINT_CODE(_fname, _params)                      \
    do {                                                 \
        insert_after_active_dll(&list, list.string_pos); \
        _fname(_params);                                 \
        list.string_pos = code.length;                   \
    } while (0)

#define CONDITION(_labelID)                            \
    do {                                               \
        char str[99999];                               \
        PRINT_CODE(tmpF, );                            \
        PRINT_CODE(new_varTF, "a");                    \
        PRINT_CODE(assignTF, "a");                     \
        PRINT_CODE(new_varTF, "type");                 \
        PRINT_CODE(write_text, "TYPE TF@type TF@a");   \
        PRINT_CODE(push_operandTF, "type");            \
        PRINT_CODE(write_text, "PUSHS string@bool");   \
        sprintf(str, "$IsSomethingElse%ld", _labelID); \
        PRINT_CODE(jumpIfNeqS, str);                   \
        PRINT_CODE(push_operandTF, "a");               \
        sprintf(str, "$ENDOFCONDITION%ld", _labelID);  \
        PRINT_CODE(jump, str);                         \
        sprintf(str, "$IsSomethingElse%ld", _labelID); \
        PRINT_CODE(label, str);                        \
        PRINT_CODE(write_text, "TYPE TF@type TF@a");   \
        PRINT_CODE(push_operandTF, "type");            \
        PRINT_CODE(write_text, "PUSHS string@int");    \
        sprintf(str, "$Next1%ld", _labelID);           \
        PRINT_CODE(jumpIfNeqS, str);                   \
        PRINT_CODE(push_operandTF, "a");               \
        PRINT_CODE(push_int, "0");                     \
        sprintf(str, "$PUSHTRUUU%ld", _labelID);       \
        PRINT_CODE(jumpIfNeqS, str);                   \
        PRINT_CODE(push_bool, "false");                \
        sprintf(str, "$ENDOFCONDITION%ld", _labelID);  \
        PRINT_CODE(jump, str);                         \
        sprintf(str, "$Next1%ld", _labelID);           \
        PRINT_CODE(label, str);                        \
        PRINT_CODE(write_text, "TYPE TF@type TF@a");   \
        PRINT_CODE(push_operandTF, "type");            \
        PRINT_CODE(write_text, "PUSHS string@float");  \
        sprintf(str, "$Next2%ld", _labelID);           \
        PRINT_CODE(jumpIfNeqS, str);                   \
        PRINT_CODE(push_operandTF, "a");               \
        PRINT_CODE(push_float, "0x0p+0");              \
        sprintf(str, "$PUSHTRUUU%ld", _labelID);       \
        PRINT_CODE(jumpIfNeqS, str);                   \
        PRINT_CODE(push_bool, "false");                \
        sprintf(str, "$ENDOFCONDITION%ld", _labelID);  \
        PRINT_CODE(jump, str);                         \
        sprintf(str, "$Next2%ld", _labelID);           \
        PRINT_CODE(label, str);                        \
        PRINT_CODE(write_text, "TYPE TF@type TF@a");   \
        PRINT_CODE(push_operandTF, "type");            \
        PRINT_CODE(write_text, "PUSHS string@string"); \
        sprintf(str, "$Next3%ld", _labelID);           \
        PRINT_CODE(jumpIfNeqS, str);                   \
        PRINT_CODE(push_operandTF, "a");               \
        PRINT_CODE(push_string, "");                   \
        PRINT_CODE(put_OPERATOR, LEX_EQ);              \
        PRINT_CODE(push_operandTF, "a");               \
        PRINT_CODE(push_string, "0");                  \
        PRINT_CODE(put_OPERATOR, LEX_EQ);              \
        PRINT_CODE(push_operandTF, "a");               \
        PRINT_CODE(push_string, "0.0");                \
        PRINT_CODE(put_OPERATOR, LEX_EQ);              \
        PRINT_CODE(put_OPERATOR, 69);                  \
        sprintf(str, "$PUSHTRUUU%ld", _labelID);       \
        PRINT_CODE(jumpIfEqS, str);                    \
        PRINT_CODE(push_bool, "false");                \
        sprintf(str, "$ENDOFCONDITION%ld", _labelID);  \
        PRINT_CODE(jump, str);                         \
        sprintf(str, "$Next3%ld", _labelID);           \
        PRINT_CODE(label, str);                        \
        PRINT_CODE(push_bool, "false");                \
        sprintf(str, "$ENDOFCONDITION%ld", _labelID);  \
        PRINT_CODE(jump, str);                         \
        sprintf(str, "$PUSHTRUUU%ld", _labelID);       \
        PRINT_CODE(label, str);                        \
        PRINT_CODE(push_bool, "true");                 \
        sprintf(str, "$ENDOFCONDITION%ld", _labelID);  \
        PRINT_CODE(label, str);                        \
    } while (0)

#define TYPE_CONTROL(_var, _res, _faillabel)                   \
    do {                                                       \
        PRINT_CODE(write_text, ("TYPE TF@" _res " TF@" _var)); \
        PRINT_CODE(push_operandTF, _res);                      \
        PRINT_CODE(push_string, "string");                     \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                      \
        PRINT_CODE(write_text, ("TYPE TF@" _res " TF@" _var)); \
        PRINT_CODE(push_operandTF, _res);                      \
        PRINT_CODE(push_string, "bool");                       \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                      \
        PRINT_CODE(write_text, ("TYPE TF@" _res " TF@" _var)); \
        PRINT_CODE(push_operandTF, _res);                      \
        PRINT_CODE(push_string, "nil");                        \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                      \
        PRINT_CODE(put_OPERATOR, 69);                          \
        PRINT_CODE(put_OPERATOR, 69);                          \
        PRINT_CODE(push_bool, "true");                         \
        PRINT_CODE(jumpIfEqS, _faillabel);                     \
    } while (0)

#define IS_NIL(_var, _res)                                     \
    do {                                                       \
        PRINT_CODE(write_text, ("TYPE TF@" _res " TF@" _var)); \
        PRINT_CODE(push_operandTF, _res);                      \
        PRINT_CODE(push_string, "nil");                        \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                      \
    } while (0)

#define CMP_TO_NULL(_var, _labelID)                     \
    do {                                                \
        MAKE_VARS();                                    \
        PRINT_CODE(write_text, "TYPE TF@res TF@"_var);  \
        PRINT_CODE(push_operandTF, "res");              \
        PRINT_CODE(write_text, "PUSHS string@nil");     \
        sprintf(str2, "$IsSomethingElse%ld", _labelID); \
        PRINT_CODE(jumpIfNeqS, str2);                   \
        PRINT_CODE(push_bool, "true");                  \
        sprintf(str2, "$ENDOFCOMPTONULL%ld", _labelID); \
        PRINT_CODE(jump, str2);                         \
        sprintf(str2, "$IsSomethingElse%ld", _labelID); \
        PRINT_CODE(label, str2);                        \
        PRINT_CODE(write_text, "TYPE TF@res TF@"_var);  \
        PRINT_CODE(push_operandTF, "res");              \
        PRINT_CODE(write_text, "PUSHS string@int");     \
        sprintf(str2, "$Next1%ld", _labelID);           \
        PRINT_CODE(jumpIfNeqS, str2);                   \
        PRINT_CODE(push_operandTF, _var);               \
        PRINT_CODE(push_int, "0");                      \
        sprintf(str2, "$PUSHTRUUU%ld", _labelID);       \
        PRINT_CODE(jumpIfEqS, str2);                    \
        PRINT_CODE(push_bool, "false");                 \
        sprintf(str2, "$ENDOFCOMPTONULL%ld", _labelID); \
        PRINT_CODE(jump, str2);                         \
        sprintf(str2, "$Next1%ld", _labelID);           \
        PRINT_CODE(label, str2);                        \
        PRINT_CODE(write_text, "TYPE TF@res TF@"_var);  \
        PRINT_CODE(push_operandTF, "res");              \
        PRINT_CODE(write_text, "PUSHS string@float");   \
        sprintf(str2, "$Next2%ld", _labelID);           \
        PRINT_CODE(jumpIfNeqS, str2);                   \
        PRINT_CODE(push_operandTF, _var);               \
        PRINT_CODE(push_float, "0x0p+0");               \
        sprintf(str2, "$PUSHTRUUU%ld", _labelID);       \
        PRINT_CODE(jumpIfEqS, str2);                    \
        PRINT_CODE(push_bool, "false");                 \
        sprintf(str2, "$ENDOFCOMPTONULL%ld", _labelID); \
        PRINT_CODE(jump, str2);                         \
        sprintf(str2, "$Next2%ld", _labelID);           \
        PRINT_CODE(label, str2);                        \
        PRINT_CODE(write_text, "TYPE TF@type TF@a");    \
        PRINT_CODE(push_operandTF, "type");             \
        PRINT_CODE(write_text, "PUSHS string@string");  \
        sprintf(str2, "$Next3%ld", _labelID);           \
        PRINT_CODE(jumpIfNeqS, str2);                   \
        PRINT_CODE(push_operandTF, _var);               \
        PRINT_CODE(push_string, "");                    \
        sprintf(str2, "$PUSHTRUUU%ld", _labelID);       \
        PRINT_CODE(jumpIfEqS, str2);                    \
        PRINT_CODE(push_bool, "false");                 \
        sprintf(str2, "$ENDOFCOMPTONULL%ld", _labelID); \
        PRINT_CODE(jump, str2);                         \
        sprintf(str2, "$Next3%ld", _labelID);           \
        PRINT_CODE(label, str2);                        \
        PRINT_CODE(push_bool, "false");                 \
        sprintf(str2, "$ENDOFCOMPTONULL%ld", _labelID); \
        PRINT_CODE(jump, str2);                         \
        sprintf(str2, "$PUSHTRUUU%ld", _labelID);       \
        PRINT_CODE(label, str2);                        \
        PRINT_CODE(push_bool, "true");                  \
        sprintf(str2, "$ENDOFCOMPTONULL%ld", _labelID); \
        PRINT_CODE(label, str2);                        \
    } while (0)

#define B_S_CONTROL(_var, _res)                                \
    do {                                                       \
        PRINT_CODE(write_text, ("TYPE TF@" _res " TF@" _var)); \
        PRINT_CODE(push_operandTF, _res);                      \
        PRINT_CODE(push_string, "string");                     \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                      \
        PRINT_CODE(write_text, ("TYPE TF@" _res " TF@" _var)); \
        PRINT_CODE(push_operandTF, _res);                      \
        PRINT_CODE(push_string, "bool");                       \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                      \
        PRINT_CODE(put_OPERATOR, 69);                          \
    } while (0)

#define INT2FLOAT(_res, _var, _faillabel)           \
    do {                                            \
        PRINT_CODE(write_text, ("TYPE TF@"_res      \
                                " TF@" _var));      \
        PRINT_CODE(push_operandTF, _res);           \
        PRINT_CODE(push_string, "int");             \
        PRINT_CODE(jumpIfNeqS, _faillabel);         \
        PRINT_CODE(write_text, ("INT2FLOAT TF@"_var \
                                " TF@"_var));       \
        PRINT_CODE(label, _faillabel);              \
    } while (0)

#define CMP_VAR(_var1, _var2, _res)            \
    do {                                       \
        PRINT_CODE(write_text, ("TYPE TF@"_res \
                                " TF@"_var1)); \
        PRINT_CODE(push_operandTF, _res);      \
        PRINT_CODE(write_text, ("TYPE TF@"_res \
                                " TF@"_var2)); \
        PRINT_CODE(push_operandTF, _res);      \
    } while (0)

#define CMP_VARTYPE(_var1, _var2, _res)        \
    do {                                       \
        PRINT_CODE(write_text, ("TYPE TF@"_res \
                                " TF@"_var1)); \
        PRINT_CODE(push_operandTF, _res);      \
        PRINT_CODE(push_operandTF, _var2);     \
    } while (0)

#define MAKE_VARS()                   \
    do {                              \
        PRINT_CODE(tmpF, );           \
        PRINT_CODE(new_varTF, "a");   \
        PRINT_CODE(new_varTF, "b");   \
        PRINT_CODE(new_varTF, "res"); \
        PRINT_CODE(assignTF, "b");    \
        PRINT_CODE(assignTF, "a");    \
    } while (0)

#define END_OPERATION(_str, _labelID)          \
    do {                                       \
        sprintf(_str, "$EXIT%ld", _labelID);   \
        PRINT_CODE(jump, _str);                \
        sprintf(_str, "$FAIL%ld", _labelID);   \
        PRINT_CODE(label, _str);               \
        PRINT_CODE(write_text, "EXIT int@53"); \
        sprintf(_str, "$EXIT%ld", _labelID);   \
        PRINT_CODE(label, _str);               \
    } while (0)

#define PRINT_SUBSTRING()                                         \
    do {                                                          \
        PRINT_CODE(label, "substring");                           \
        PRINT_CODE(write_text, "PUSHFRAME");                      \
        PRINT_CODE(tmpF, );                                       \
        PRINT_CODE(new_varTF, "res");                             \
        PRINT_CODE(new_varTF, "check");                           \
        PRINT_CODE(write_text, "MOVE TF@res LF@i");               \
        PRINT_CODE(write_text, "MOVE TF@check int@69");           \
        CMP_VAR("res", "check", "res");                           \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                         \
        PRINT_CODE(write_text, "MOVE TF@res LF@j");               \
        PRINT_CODE(write_text, "MOVE TF@check int@69");           \
        CMP_VAR("res", "check", "res");                           \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                         \
        PRINT_CODE(write_text, "MOVE TF@res LF@s");               \
        PRINT_CODE(write_text, "MOVE TF@check string@tvojemama"); \
        CMP_VAR("res", "check", "res");                           \
        PRINT_CODE(put_OPERATOR, LEX_EQ);                         \
        PRINT_CODE(put_OPERATOR, 420);                            \
        PRINT_CODE(jumpIfEqS, "substringcontinue");               \
        PRINT_CODE(write_text, "EXIT int@4");                     \
        PRINT_CODE(label, "substringcontinue");                   \
        PRINT_CODE(write_text, "DEFVAR LF@strlen");               \
        PRINT_CODE(write_text, "PUSHS LF@s");                     \
        PRINT_CODE(write_text, "CREATEFRAME");                    \
        PRINT_CODE(write_text, "DEFVAR TF@string");               \
        PRINT_CODE(write_text, "POPS TF@string");                 \
        PRINT_CODE(write_text, "CALL strlen");                    \
        PRINT_CODE(write_text, "PUSHS int@1");                    \
        PRINT_CODE(write_text, "ADDS");                           \
        PRINT_CODE(write_text, "POPS LF@strlen");                 \
        PRINT_CODE(write_text, "CLEARS");                         \
        PRINT_CODE(write_text, "PUSHS LF@i");                     \
        PRINT_CODE(write_text, "PUSHS int@0");                    \
        PRINT_CODE(write_text, "LTS");                            \
        PRINT_CODE(write_text, "PUSHS bool@true");                \
        PRINT_CODE(write_text, "JUMPIFNEQS SUBSTRINGELSE1");      \
        PRINT_CODE(write_text, "PUSHS nil@nil");                  \
        PRINT_CODE(write_text, "POPFRAME");                       \
        PRINT_CODE(write_text, "RETURN");                         \
        PRINT_CODE(write_text, "JUMP SUBSTRINGELSEEND1");         \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSE1");           \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSEEND1");        \
        PRINT_CODE(write_text, "PUSHS LF@j");                     \
        PRINT_CODE(write_text, "PUSHS int@0");                    \
        PRINT_CODE(write_text, "LTS");                            \
        PRINT_CODE(write_text, "PUSHS bool@true");                \
        PRINT_CODE(write_text, "JUMPIFNEQS SUBSTRINGELSE2");      \
        PRINT_CODE(write_text, "PUSHS nil@nil");                  \
        PRINT_CODE(write_text, "POPFRAME");                       \
        PRINT_CODE(write_text, "RETURN");                         \
        PRINT_CODE(write_text, "JUMP SUBSTRINGELSEEND2");         \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSE2");           \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSEEND2");        \
        PRINT_CODE(write_text, "PUSHS LF@i");                     \
        PRINT_CODE(write_text, "PUSHS LF@j");                     \
        PRINT_CODE(write_text, "GTS");                            \
        PRINT_CODE(write_text, "PUSHS bool@true");                \
        PRINT_CODE(write_text, "JUMPIFNEQS SUBSTRINGELSE3");      \
        PRINT_CODE(write_text, "PUSHS nil@nil");                  \
        PRINT_CODE(write_text, "POPFRAME");                       \
        PRINT_CODE(write_text, "RETURN");                         \
        PRINT_CODE(write_text, "JUMP SUBSTRINGELSEEND3");         \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSE3");           \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSEEND3");        \
        PRINT_CODE(write_text, "PUSHS LF@i");                     \
        PRINT_CODE(write_text, "PUSHS LF@strlen");                \
        PRINT_CODE(write_text, "CREATEFRAME");                    \
        PRINT_CODE(write_text, "DEFVAR TF@a");                    \
        PRINT_CODE(write_text, "DEFVAR TF@b");                    \
        PRINT_CODE(write_text, "DEFVAR TF@res");                  \
        PRINT_CODE(write_text, "POPS TF@b");                      \
        PRINT_CODE(write_text, "POPS TF@a");                      \
        PRINT_CODE(write_text, "GT TF@res TF@a TF@b");            \
        PRINT_CODE(write_text, "PUSHS TF@res");                   \
        PRINT_CODE(write_text, "EQ TF@res TF@a TF@b");            \
        PRINT_CODE(write_text, "PUSHS TF@res");                   \
        PRINT_CODE(write_text, "ORS");                            \
        PRINT_CODE(write_text, "PUSHS bool@true");                \
        PRINT_CODE(write_text, "JUMPIFNEQS SUBSTRINGELSE4");      \
        PRINT_CODE(write_text, "PUSHS nil@nil");                  \
        PRINT_CODE(write_text, "POPFRAME");                       \
        PRINT_CODE(write_text, "RETURN");                         \
        PRINT_CODE(write_text, "JUMP SUBSTRINGELSEEND4");         \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSE4");           \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSEEND4");        \
        PRINT_CODE(write_text, "PUSHS LF@j");                     \
        PRINT_CODE(write_text, "PUSHS LF@strlen");                \
        PRINT_CODE(write_text, "CREATEFRAME");                    \
        PRINT_CODE(write_text, "DEFVAR TF@a");                    \
        PRINT_CODE(write_text, "DEFVAR TF@b");                    \
        PRINT_CODE(write_text, "DEFVAR TF@res");                  \
        PRINT_CODE(write_text, "POPS TF@b");                      \
        PRINT_CODE(write_text, "POPS TF@a");                      \
        PRINT_CODE(write_text, "GT TF@res TF@a TF@b");            \
        PRINT_CODE(write_text, "PUSHS TF@res");                   \
        PRINT_CODE(write_text, "EQ TF@res TF@a TF@b");            \
        PRINT_CODE(write_text, "PUSHS TF@res");                   \
        PRINT_CODE(write_text, "ORS");                            \
        PRINT_CODE(write_text, "PUSHS bool@true");                \
        PRINT_CODE(write_text, "JUMPIFNEQS SUBSTRINGELSE5");      \
        PRINT_CODE(write_text, "PUSHS nil@nil");                  \
        PRINT_CODE(write_text, "POPFRAME");                       \
        PRINT_CODE(write_text, "RETURN");                         \
        PRINT_CODE(write_text, "JUMP SUBSTRINGELSEEND5");         \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSE5");           \
        PRINT_CODE(write_text, "LABEL SUBSTRINGELSEEND5");        \
                                                                  \
        PRINT_CODE(new_var, "ans");                               \
        PRINT_CODE(write_text, "MOVE LF@ans string@");            \
        PRINT_CODE(new_var, "tmp");                               \
                                                                  \
        PRINT_CODE(write_text, "LABEL substringgwhileStart0");    \
        PRINT_CODE(write_text, "PUSHS LF@i");                     \
        PRINT_CODE(write_text, "PUSHS LF@j");                     \
        PRINT_CODE(write_text, "LTS");                            \
        PRINT_CODE(write_text, "PUSHS bool@true");                \
        PRINT_CODE(write_text, "JUMPIFNEQS substringgwhileEnd0"); \
        PRINT_CODE(write_text, "GETCHAR LF@tmp LF@s LF@i");       \
        PRINT_CODE(write_text, "CONCAT LF@ans LF@ans LF@tmp");    \
        PRINT_CODE(write_text, "PUSHS LF@i");                     \
        PRINT_CODE(write_text, "PUSHS int@1");                    \
        PRINT_CODE(write_text, "ADDS");                           \
        PRINT_CODE(write_text, "POPS LF@i");                      \
        PRINT_CODE(write_text, "CLEARS");                         \
        PRINT_CODE(write_text, "JUMP substringgwhileStart0");     \
        PRINT_CODE(write_text, "LABEL substringgwhileEnd0");      \
                                                                  \
        PRINT_CODE(push_operand, "ans");                          \
        PRINT_CODE(popF, );                                       \
        PRINT_CODE(exit_function, );                              \
        PRINT_CODE(write_text, "");                               \
    } while (0)

#define PRINT_CHR()                                     \
    do {                                                \
        PRINT_CODE(label, "chr");                       \
        PRINT_CODE(pushF, );                            \
        PRINT_CODE(tmpF, );                             \
        PRINT_CODE(new_varTF, "res");                   \
        PRINT_CODE(new_varTF, "check");                 \
        PRINT_CODE(write_text, "MOVE TF@res LF@i");     \
        PRINT_CODE(write_text, "MOVE TF@check int@69"); \
        CMP_VAR("res", "check", "res");                 \
        PRINT_CODE(jumpIfEqS, "tutotamtooo");           \
        PRINT_CODE(write_text, "EXIT int@4");           \
        PRINT_CODE(label, "tutotamtooo");               \
        PRINT_CODE(write_text, "INT2CHAR TF@res LF@i"); \
        PRINT_CODE(write_text, "PUSHS TF@res");         \
        PRINT_CODE(popF, );                             \
        PRINT_CODE(exit_function, );                    \
        PRINT_CODE(write_text, "");                     \
    } while (0)

#define PRINT_FLOATVAL()                                              \
    do {                                                              \
        PRINT_CODE(label, "floatval");                                \
        PRINT_CODE(pushF, );                                          \
        PRINT_CODE(tmpF, );                                           \
        PRINT_CODE(new_varTF, "val2convert");                         \
        PRINT_CODE(new_varTF, "check");                               \
        PRINT_CODE(new_varTF, "res");                                 \
        PRINT_CODE(write_text, "MOVE TF@val2convert LF@val2convert"); \
        PRINT_CODE(write_text, "MOVE TF@check string@string");        \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "stringinfloatvalerr");                 \
        PRINT_CODE(write_text, "MOVE TF@val2convert LF@val2convert"); \
        PRINT_CODE(write_text, "MOVE TF@check string@float");         \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "floatinfloatvalgood");                 \
        PRINT_CODE(write_text, "MOVE TF@val2convert LF@val2convert"); \
        PRINT_CODE(write_text, "MOVE TF@check string@int");           \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "intinginfloatvalgood");                \
        PRINT_CODE(write_text, "MOVE TF@val2convert LF@val2convert"); \
        PRINT_CODE(write_text, "MOVE TF@check string@nil");           \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "nullinfloatvalgood");                  \
        PRINT_CODE(label, "stringinfloatvalerr");                     \
        PRINT_CODE(write_text, "EXIT int@4");                         \
        PRINT_CODE(label, "floatinfloatvalgood");                     \
        PRINT_CODE(write_text, "PUSHS LF@val2convert");               \
        PRINT_CODE(popF, );                                           \
        PRINT_CODE(exit_function, );                                  \
        PRINT_CODE(label, "intinginfloatvalgood");                    \
        PRINT_CODE(write_text, "INT2FLOAT TF@res LF@val2convert");    \
        PRINT_CODE(write_text, "PUSHS TF@res");                       \
        PRINT_CODE(popF, );                                           \
        PRINT_CODE(exit_function, );                                  \
        PRINT_CODE(label, "nullinfloatvalgood");                      \
        PRINT_CODE(write_text, "PUSHS FLOAT@0x0p+0");                 \
        PRINT_CODE(popF, );                                           \
        PRINT_CODE(exit_function, );                                  \
        PRINT_CODE(write_text, "");                                   \
    } while (0)

#define PRINT_INTVAL()                                                \
    do {                                                              \
        PRINT_CODE(label, "intval");                                  \
        PRINT_CODE(pushF, );                                          \
        PRINT_CODE(tmpF, );                                           \
        PRINT_CODE(new_varTF, "val2convert");                         \
        PRINT_CODE(new_varTF, "check");                               \
        PRINT_CODE(new_varTF, "res");                                 \
        PRINT_CODE(write_text, "MOVE TF@val2convert LF@val2convert"); \
        PRINT_CODE(write_text, "MOVE TF@check string@string");        \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "stringinintvalerr");                   \
        PRINT_CODE(write_text, "MOVE TF@check string@float");         \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "floatinintvalgood");                   \
        PRINT_CODE(write_text, "MOVE TF@check string@int");           \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "intinintvalgood");                     \
        PRINT_CODE(write_text, "MOVE TF@check string@nil");           \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "nullinintvalgood");                    \
        PRINT_CODE(label, "stringinintvalerr");                       \
        PRINT_CODE(write_text, "EXIT int@4");                         \
        PRINT_CODE(label, "floatinintvalgood");                       \
        PRINT_CODE(write_text, "FLOAT2INT TF@res LF@val2convert");    \
        PRINT_CODE(write_text, "PUSHS TF@res");                       \
        PRINT_CODE(popF, );                                           \
        PRINT_CODE(exit_function, );                                  \
        PRINT_CODE(label, "intinintvalgood");                         \
        PRINT_CODE(write_text, "PUSHS LF@val2convert");               \
        PRINT_CODE(popF, );                                           \
        PRINT_CODE(exit_function, );                                  \
        PRINT_CODE(label, "nullinintvalgood");                        \
        PRINT_CODE(write_text, "PUSHS INT@0");                        \
        PRINT_CODE(popF, );                                           \
        PRINT_CODE(exit_function, );                                  \
        PRINT_CODE(write_text, "");                                   \
    } while (0)

#define PRINT_STRVAL()                                                \
    do {                                                              \
        PRINT_CODE(label, "strval");                                  \
        PRINT_CODE(pushF, );                                          \
        PRINT_CODE(tmpF, );                                           \
        PRINT_CODE(new_varTF, "val2convert");                         \
        PRINT_CODE(new_varTF, "check");                               \
        PRINT_CODE(new_varTF, "res");                                 \
        PRINT_CODE(write_text, "MOVE TF@val2convert LF@val2convert"); \
        PRINT_CODE(write_text, "MOVE TF@check string@string");        \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "stringinstrvalgood");                  \
        PRINT_CODE(write_text, "MOVE TF@check string@float");         \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "floatinstrvalbad");                    \
        PRINT_CODE(write_text, "MOVE TF@check string@int");           \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "floatinstrvalbad");                    \
        PRINT_CODE(write_text, "MOVE TF@check string@nil");           \
        CMP_VARTYPE("val2convert", "check", "res");                   \
        PRINT_CODE(jumpIfEqS, "nullinstrvalgood");                    \
        PRINT_CODE(label, "stringinstrvalgood");                      \
        PRINT_CODE(write_text, "PUSHS TF@val2convert");               \
        PRINT_CODE(popF, );                                           \
        PRINT_CODE(exit_function, );                                  \
        PRINT_CODE(label, "floatinstrvalbad");                        \
        PRINT_CODE(write_text, "EXIT INT@4");                         \
        PRINT_CODE(label, "nullinstrvalgood");                        \
        PRINT_CODE(write_text, "PUSHS STRING@");                      \
        PRINT_CODE(popF, );                                           \
        PRINT_CODE(exit_function, );                                  \
        PRINT_CODE(write_text, "");                                   \
    } while (0)

#define PRINT_ORD()                                              \
    do {                                                         \
        PRINT_CODE(label, "ord");                                \
        PRINT_CODE(pushF, );                                     \
        PRINT_CODE(tmpF, );                                      \
        PRINT_CODE(new_varTF, "string");                         \
        PRINT_CODE(push_operand, "string");                      \
        PRINT_CODE(assignTF, "string");                          \
        PRINT_CODE(call, "strlen");                              \
        PRINT_CODE(push_int, "0");                               \
        PRINT_CODE(jumpIfEqS, "label_skip_ord");                 \
        PRINT_CODE(new_var, "a");                                \
        PRINT_CODE(write_text, "STRI2INT LF@a LF@string int@0"); \
        PRINT_CODE(push_operand, "a");                           \
        PRINT_CODE(jump, "labelordENDENDENDEND");                \
        PRINT_CODE(label, "label_skip_ord");                     \
        PRINT_CODE(push_int, "0");                               \
        PRINT_CODE(label, "labelordENDENDENDEND");               \
        PRINT_CODE(popF, );                                      \
        PRINT_CODE(exit_function, );                             \
        PRINT_CODE(write_text, "");                              \
    } while (0)

#define PRINT_STRLEN()                                                   \
    do {                                                                 \
        char str1[99999];                                                \
        char str2[99999];                                                \
        unsigned long labelID = getLabel();                              \
        sprintf(str1, "$goodtype%ld", labelID);                          \
        sprintf(str2, "$badtype%ld", labelID);                           \
        PRINT_CODE(label, "strlen");                                     \
        PRINT_CODE(pushF, );                                             \
        PRINT_CODE(new_var, "length");                                   \
        PRINT_CODE(new_var, "type");                                     \
        PRINT_CODE(write_text, "TYPE LF@type LF@string");                \
        PRINT_CODE(push_operand, "type");                                \
        PRINT_CODE(push_string, "string");                               \
                                                                         \
        PRINT_CODE(jumpIfNeqS, str2);                                    \
        PRINT_CODE(write_text, "STRLEN LF@length LF@string");            \
        PRINT_CODE(push_operand, "length");                              \
        PRINT_CODE(jump, str1);                                          \
        PRINT_CODE(label, str2);                                         \
        PRINT_CODE(                                                      \
            write_text,                                                  \
            "WRITE string@Invalid\\032data\\032Type\\032in\\032STRLEN"); \
        PRINT_CODE(write_text, "EXIT INT@4");                            \
        PRINT_CODE(label, str1);                                         \
        PRINT_CODE(popF, );                                              \
        PRINT_CODE(exit_function, );                                     \
        PRINT_CODE(write_text, "");                                      \
    } while (0)

#define WINSTRUCTION(_name)                                    \
    do {                                                       \
        if (stringConcatenate(&code, (_name "\n")) == false) { \
            endParser(INTERNAL_ERROR);                         \
        }                                                      \
    } while (0)

#define WTEXT(_text)                                      \
    do {                                                  \
        if (stringConcatenate(&code, (_text)) == false) { \
            endParser(INTERNAL_ERROR);                    \
        }                                                 \
    } while (0)

bool print_code();

bool put_OPERATOR(int type);

bool write_text(const char *text);

bool call(const char *id);

bool push_operand(const char *id);

bool push_operandTF(const char *id);

bool push_int(const char *value);

bool push_float(const char *value);

bool push_string(const char *value);

bool push_bool(const char *value);

bool push_null();

bool assign(const char *id);

bool assignTF(const char *id);

bool label(const char *id);

bool tmpF();

bool pushF();

bool popF();

bool new_var(const char *id);

bool new_varTF(const char *id);

bool jumpIfNeqS(const char *label);

bool jump(const char *label);

bool jumpIfEqS(const char *label);

bool write(const char *text);

bool exit_function();

bool clears();

bool write_var(const char *name);

#endif