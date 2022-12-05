/*
 * IFJ 2022 PROJECT
 *
 * main.h
 *
 * Andrej Mokris: xmokri01
 * Leos Nevoral: xnevor03
 * Adam Pap: xpapad11
 *
 */

#include "expression.h"

#include "generator.h"
#include "parser.h"
#include "print_inst.h"

extern String_t code;
extern Tinstruction_list list;

static char table[15][15] = {
    // 0    1    2    3    4    5    6    7    8    9    10   11   12  13   14
    // +    -    *    /   ===  !==   <    >   <=  >=     (    )    i    $   .
    {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>',
     '>'},  // + 0
    {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>',
     '>'},  // - 1
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>',
     '>'},  // * 2
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>',
     '>'},  // / 3
    {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>',
     '<'},  // === 4
    {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>',
     '<'},  // !== 5
    {'<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '>',
     '<'},  // < 6
    {'<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '>',
     '<'},  // > 7
    {'<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '>',
     '<'},  // <= 8
    {'<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '>',
     '<'},  // >= 9
    {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', 'X',
     '<'},  // ( 10
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', '>',
     '>'},  // ) 11
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', '>',
     '>'},  // i 12
    {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'X', '<', 'A',
     '<'},  // $13
    {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>',
     '>'}  // . 14
};

// E = E + E
int addRule(Stack *stack, StackElement *op1, StackElement *op2) {
    unsigned long labelID = getLabel();
    char str1[9999];
    char str2[9999];
    MAKE_VARS();
    sprintf(str1, "$FAIL%ld", labelID);
    TYPE_CONTROL("a", "res", str1, getLabel());
    TYPE_CONTROL("b", "res", str1, getLabel());
    CMP_VAR("a", "b", "res");
    sprintf(str1, "$ADD%ld", labelID);
    PRINT_CODE(jumpIfEqS, str1);
    sprintf(str1, "$ADDLABEL1%ld", labelID);
    INT2FLOAT("res", "a", str1);
    sprintf(str1, "$ADDLABEL2%ld", labelID);
    INT2FLOAT("res", "b", str1);
    sprintf(str1, "$ADD%ld", labelID);
    PRINT_CODE(label, str1);
    PRINT_CODE(write_text, "ADD TF@res TF@a TF@b");
    PRINT_CODE(push_operandTF, "res");
    END_OPERATION(str1, labelID);

    op1->dataType = LEX_FLOAT;
    stackPushElement(stack, op1);
    elementDeconstruct(op2);
    return SUCCESS;
}

// E = E - E
int subRule(Stack *stack, StackElement *op1, StackElement *op2) {
    unsigned long labelID = getLabel();
    char str1[9999];
    char str2[9999];
    MAKE_VARS();
    sprintf(str1, "$FAIL%ld", labelID);
    TYPE_CONTROL("a", "res", str1, getLabel());
    TYPE_CONTROL("b", "res", str1, getLabel());
    CMP_VAR("a", "b", "res");
    sprintf(str1, "$SUB%ld", labelID);
    PRINT_CODE(jumpIfEqS, str1);
    sprintf(str1, "$SUBLABEL1%ld", labelID);
    INT2FLOAT("res", "a", str1);
    sprintf(str1, "$SUBLABEL2%ld", labelID);
    INT2FLOAT("res", "b", str1);
    sprintf(str1, "$SUB%ld", labelID);
    PRINT_CODE(label, str1);
    PRINT_CODE(write_text, "SUB TF@res TF@a TF@b");
    PRINT_CODE(push_operandTF, "res");
    END_OPERATION(str1, labelID);

    op1->dataType = LEX_FLOAT;
    stackPushElement(stack, op1);
    elementDeconstruct(op2);
    return SUCCESS;
}

// E = E * E
int mulRule(Stack *stack, StackElement *op1, StackElement *op2) {
    unsigned long labelID = getLabel();
    char str1[9999];
    char str2[9999];
    MAKE_VARS();
    sprintf(str1, "$FAIL%ld", labelID);
    TYPE_CONTROL("a", "res", str1, getLabel());
    TYPE_CONTROL("b", "res", str1, getLabel());
    CMP_VAR("a", "b", "res");
    sprintf(str1, "$MULL%ld", labelID);
    PRINT_CODE(jumpIfEqS, str1);
    sprintf(str1, "$MULLABEL1%ld", labelID);
    INT2FLOAT("res", "a", str1);
    sprintf(str1, "$MULLABEL2%ld", labelID);
    INT2FLOAT("res", "b", str1);
    sprintf(str1, "$MULL%ld", labelID);
    PRINT_CODE(label, str1);
    PRINT_CODE(write_text, "MUL TF@res TF@a TF@b");
    PRINT_CODE(push_operandTF, "res");
    END_OPERATION(str1, labelID);

    op1->dataType = LEX_FLOAT;
    stackPushElement(stack, op1);
    elementDeconstruct(op2);
    return SUCCESS;
}

// E = E / E
int divRule(Stack *stack, StackElement *op1, StackElement *op2) {
    unsigned long labelID = getLabel();
    char str1[9999];
    char str2[9999];
    MAKE_VARS();
    sprintf(str1, "$FAIL%ld", labelID);
    TYPE_CONTROL("a", "res", str1, getLabel());
    TYPE_CONTROL("b", "res", str1, getLabel());
    sprintf(str1, "$DIVLABEL1%ld", labelID);
    INT2FLOAT("res", "a", str1);
    sprintf(str1, "$DIVLABEL2%ld", labelID);
    INT2FLOAT("res", "b", str1);
    PRINT_CODE(write_text, "DIV TF@res TF@a TF@b");
    PRINT_CODE(push_operandTF, "res");
    END_OPERATION(str1, labelID);
    op1->dataType = LEX_FLOAT;
    stackPushElement(stack, op1);
    elementDeconstruct(op2);
    return SUCCESS;
}

int reduceExpression(Stack *stack) {
    StackElement *popArr[3];
    int cnt = 0;
    StackElement *lastPopped;
    while ((lastPopped = stackPop(stack)) &&
           lastPopped->tokenID != LEX_BEG_HANDLE) {
        popArr[cnt] = lastPopped;
        cnt++;
    }
    elementDeconstruct(lastPopped);
    char str[99999];
    // i -> E
    if (cnt == 1 && popArr[0]->tokenID == LEX_I) {
        popArr[0]->tokenID = LEX_E;
        if (popArr[0]->isID == false) {
            if (popArr[0]->dataType == LEX_INT) {
                PRINT_CODE(push_int, popArr[0]->tokenVal.string);
            } else if (popArr[0]->dataType == LEX_FLOAT ||
                       popArr[0]->dataType == LEX_EXPONENT) {
                char *ptr;
                double ret;
                ret = strtod(popArr[0]->tokenVal.string, &ptr);
                sprintf(str, "%a", ret);
                PRINT_CODE(push_float, str);  // float val
            } else if (popArr[0]->dataType == LEX_STRING) {
                PRINT_CODE(push_string, popArr[0]->tokenVal.string);
            } else if (popArr[0]->dataType == LEX_NULL) {
                PRINT_CODE(push_null, );
            }
        }
        stackPushElement(stack, popArr[0]);
        return SUCCESS;
    }

    if (cnt == 3 && popArr[0]->tokenID == LEX_E &&
        popArr[2]->tokenID == LEX_E) {
        unsigned long labelID;
        char str1[9999];
        char str2[9999];
        char str3[9999];
        switch (popArr[1]->tokenID) {
            case LEX_ADD:
                // E+E -> E
                elementDeconstruct(popArr[1]);
                return addRule(stack, popArr[0], popArr[2]);
            case LEX_SUB:
                // E-E -> E
                elementDeconstruct(popArr[1]);
                return subRule(stack, popArr[0], popArr[2]);
            case LEX_MUL:
                // E*E -> E
                elementDeconstruct(popArr[1]);
                return mulRule(stack, popArr[0], popArr[2]);
            case LEX_DIV:
                // E/E -> E
                elementDeconstruct(popArr[1]);
                return divRule(stack, popArr[0], popArr[2]);
            case LEX_NEQ:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                MAKE_VARS();
                CMP_VAR("a", "b", "res");
                labelID = getLabel();
                sprintf(str1, "$NEQSLABEL1%ld", labelID);
                sprintf(str2, "$NEQSLABEL2%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str1);
                PRINT_CODE(push_operandTF, "a");
                PRINT_CODE(push_operandTF, "b");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(put_OPERATOR, 0);
                PRINT_CODE(jump, str2);
                PRINT_CODE(label, str1);
                PRINT_CODE(push_bool, "true");
                PRINT_CODE(label, str2);

                return SUCCESS;
            case LEX_LEQ:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                labelID = getLabel();
                MAKE_VARS();

                sprintf(str1, "$GTQEND%ld", labelID);
                sprintf(str3, "$LEQDONTCMPTONULL1%ld", labelID);
                IS_NIL("a", "res");
                PRINT_CODE(push_bool, "true");
                PRINT_CODE(jumpIfNeqS, str3);
                CMP_TO_NULL("b", getLabel());
                PRINT_CODE(jump, str1);
                PRINT_CODE(label, str3);

                IS_NIL("b", "res");
                PRINT_CODE(push_bool, "true");
                sprintf(str3, "$LEQDONTCMPTONULL2%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str3);
                CMP_TO_NULL("a", getLabel());
                PRINT_CODE(jump, str1);
                PRINT_CODE(label, str3);

                sprintf(str1, "$LEQCOMPARE%ld", labelID);
                sprintf(str2, "$LEQNOTCOMPARE%ld", labelID);
                CMP_VAR("a", "b", "res");
                PRINT_CODE(jumpIfEqS, str1);
                B_S_CONTROL("a", "res");
                B_S_CONTROL("b", "res");
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                sprintf(str3, "$LEQVALIDTYPE%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str3);
                PRINT_CODE(write_text, "EXIT int@7");
                PRINT_CODE(label, str3);
                sprintf(str3, "$LESKIP1%ld", labelID);
                INT2FLOAT("res", "a", str3);
                sprintf(str3, "$LESKIP2%ld", labelID);
                INT2FLOAT("res", "b", str3);
                PRINT_CODE(label, str1);
                PRINT_CODE(push_operandTF, "a");
                PRINT_CODE(push_operandTF, "b");
                PRINT_CODE(put_OPERATOR, LEX_LE);
                PRINT_CODE(push_operandTF, "a");
                PRINT_CODE(push_operandTF, "b");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(put_OPERATOR, 69);
                sprintf(str3, "$GTQEND%ld", labelID);
                PRINT_CODE(jump, str3);
                PRINT_CODE(label, str2);
                PRINT_CODE(push_bool, "false");
                PRINT_CODE(label, str3);
                return SUCCESS;
                break;
            case LEX_GTQ:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                labelID = getLabel();
                MAKE_VARS();

                sprintf(str1, "$GTQEND%ld", labelID);
                IS_NIL("a", "res");
                PRINT_CODE(push_bool, "true");
                sprintf(str3, "$GTQDONTCMPTONULL1%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str3);
                CMP_TO_NULL("b", getLabel());
                PRINT_CODE(jump, str1);
                PRINT_CODE(label, str3);

                IS_NIL("b", "res");
                PRINT_CODE(push_bool, "true");
                sprintf(str3, "$GTQDONTCMPTONULL2%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str3);
                CMP_TO_NULL("a", getLabel());
                PRINT_CODE(jump, str1);
                PRINT_CODE(label, str3);

                sprintf(str1, "$GTQCOMPARE%ld", labelID);
                sprintf(str2, "$GTQNOTCOMPARE%ld", labelID);
                CMP_VAR("a", "b", "res");
                PRINT_CODE(jumpIfEqS, str1);
                B_S_CONTROL("a", "res");
                B_S_CONTROL("b", "res");
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                sprintf(str3, "$GTQVALIDTYPE%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str3);
                PRINT_CODE(write_text, "EXIT int@7");
                PRINT_CODE(label, str3);
                sprintf(str3, "$GTQSKIP1%ld", labelID);
                INT2FLOAT("res", "a", str3);
                sprintf(str3, "$GTQSKIP2%ld", labelID);
                INT2FLOAT("res", "b", str3);
                PRINT_CODE(label, str1);
                PRINT_CODE(push_operandTF, "a");
                PRINT_CODE(push_operandTF, "b");
                PRINT_CODE(put_OPERATOR, LEX_GT);
                PRINT_CODE(push_operandTF, "a");
                PRINT_CODE(push_operandTF, "b");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(put_OPERATOR, 69);
                sprintf(str3, "$GTQEND%ld", labelID);
                PRINT_CODE(jump, str3);
                PRINT_CODE(label, str2);
                PRINT_CODE(push_bool, "false");
                PRINT_CODE(label, str3);

                return SUCCESS;
                break;
            case LEX_GT:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                labelID = getLabel();
                sprintf(str1, "$GREATERCOMPARE%ld", labelID);
                sprintf(str2, "$GREATERNOTCOMPARE%ld", labelID);
                MAKE_VARS();
                IS_NIL("a", "res");
                IS_NIL("b", "res");
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                PRINT_CODE(jumpIfEqS, str2);
                CMP_VAR("a", "b", "res");
                PRINT_CODE(jumpIfEqS, str1);
                B_S_CONTROL("a", "res");
                B_S_CONTROL("b", "res");
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                sprintf(str3, "$GREATERVALIDTYPE%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str3);
                PRINT_CODE(write_text, "EXIT int@7");
                PRINT_CODE(label, str3);
                sprintf(str3, "$GTSKIP1%ld", labelID);
                INT2FLOAT("res", "a", str3);
                sprintf(str3, "$GTSKIP2%ld", labelID);
                INT2FLOAT("res", "b", str3);
                PRINT_CODE(label, str1);
                PRINT_CODE(push_operandTF, "a");
                PRINT_CODE(push_operandTF, "b");
                PRINT_CODE(put_OPERATOR, LEX_GT);
                sprintf(str3, "$GREATEREND%ld", labelID);
                PRINT_CODE(jump, str3);
                PRINT_CODE(label, str2);
                PRINT_CODE(push_bool, "false");
                PRINT_CODE(label, str3);
                return SUCCESS;
            case LEX_LE:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                labelID = getLabel();
                sprintf(str1, "$LESSCOMPARE%ld", labelID);
                sprintf(str2, "$LESSNOTCOMPARE%ld", labelID);
                MAKE_VARS();
                IS_NIL("a", "res");
                IS_NIL("b", "res");
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                PRINT_CODE(jumpIfEqS, str2);
                CMP_VAR("a", "b", "res");
                PRINT_CODE(jumpIfEqS, str1);
                B_S_CONTROL("a", "res");
                B_S_CONTROL("b", "res");
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                sprintf(str3, "$LESSVALIDTYPE%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str3);
                PRINT_CODE(write_text, "EXIT int@7");
                PRINT_CODE(label, str3);
                sprintf(str3, "$LESKIP1%ld", labelID);
                INT2FLOAT("res", "a", str3);
                sprintf(str3, "$LESKIP2%ld", labelID);
                INT2FLOAT("res", "b", str3);
                PRINT_CODE(label, str1);
                PRINT_CODE(push_operandTF, "a");
                PRINT_CODE(push_operandTF, "b");
                PRINT_CODE(put_OPERATOR, LEX_LE);
                sprintf(str3, "$LESSEND%ld", labelID);
                PRINT_CODE(jump, str3);
                PRINT_CODE(label, str2);
                PRINT_CODE(push_bool, "false");
                PRINT_CODE(label, str3);

                return SUCCESS;
            case LEX_EQ:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                labelID = getLabel();
                MAKE_VARS();
                CMP_VAR("a", "b", "res");
                labelID = getLabel();
                sprintf(str1, "$EQSLABEL1%ld", labelID);
                sprintf(str2, "$EQSLABEL2%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str1);
                PRINT_CODE(push_operandTF, "a");
                PRINT_CODE(push_operandTF, "b");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(jump, str2);
                PRINT_CODE(label, str1);
                PRINT_CODE(push_bool, "false");
                PRINT_CODE(label, str2);
                return SUCCESS;
            case LEX_DOT:
                // CONCAT
                popArr[0]->dataType = LEX_STRING;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                labelID = getLabel();
                sprintf(str1, "$NCOMPCAT%ld", labelID);
                sprintf(str2, "$COMPCAT%ld", labelID);
                sprintf(str3, "$SECONDOPERANDCOMPARE%ld", labelID);
                // save operands into A and B
                MAKE_VARS();
                // Do the type check
                PRINT_CODE(write_text, "TYPE TF@res TF@a");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(push_string, "float");
                PRINT_CODE(jumpIfEqS, str1);
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(push_string, "int");
                PRINT_CODE(jumpIfEqS, str1);
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(push_string, "string");
                PRINT_CODE(jumpIfEqS, str3);
                PRINT_CODE(write_text, "MOVE TF@a string@");
                // check second operand
                PRINT_CODE(label, str3);
                PRINT_CODE(write_text, "TYPE TF@res TF@b");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(push_string, "int");
                PRINT_CODE(jumpIfEqS, str1);
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(push_string, "float");
                PRINT_CODE(jumpIfEqS, str1);
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(push_string, "string");
                PRINT_CODE(jumpIfEqS, str2);
                PRINT_CODE(write_text, "MOVE TF@b string@");
                PRINT_CODE(jump, str2);
                // EXIT LABEL
                PRINT_CODE(label, str1);
                PRINT_CODE(write_text, "EXIT int@7");
                // good data types label
                PRINT_CODE(label, str2);
                PRINT_CODE(write_text, "CONCAT TF@res TF@a TF@b");
                PRINT_CODE(push_operandTF, "res");
                return SUCCESS;
            default:
                return SYNTAX_ERROR;
        }
    }
    // (E) -> E
    if (cnt == 3 && popArr[0]->tokenID == LEX_RPAR &&
        popArr[2]->tokenID == LEX_LPAR) {
        if (popArr[1]->tokenID == LEX_E) {
            stackPushElement(stack, popArr[1]);
            elementDeconstruct(popArr[0]);
            elementDeconstruct(popArr[2]);
            return SUCCESS;
        }
    }
    return SYNTAX_ERROR;
}

int finishReducing(Stack *stack) {
    int res = SUCCESS;
    while (res == SUCCESS && stack->elementCount > 2) {
        // printStack(stack);
        res = reduceExpression(stack);
    }
    return res;
}

int parseExpression(int endChar, int *resDataType, node_t *symTable) {
    String_t string;
    Stack *stack = initStack(STACK_INIT_SIZE);
    bool endAnalysis = false;

    if (!StringInit(&string) || stack == NULL || !stringAppend(&string, '$') ||
        !stackPush(stack, LEX_DOLLAR, string)) {
        stackDeconstruct(stack);
        stringDeconstruct(&string);
        return INTERNAL_ERROR;
    }

    int curLex = getTokenfromStore(&string);
    if (curLex == LEX_RPAR && (endChar == LEX_COMMA || endChar == LEX_RPAR)) {
        stackDeconstruct(stack);
        stringDeconstruct(&string);
        *resDataType = LEX_VOID;
        return RUN_ERROR;
    } else if (curLex == endChar && endChar == LEX_SEMICOL) {
        stackDeconstruct(stack);
        stringDeconstruct(&string);
        *resDataType = LEX_VOID;
        return SUCCESS;
    }
    if (curLex == LEX_FUNID) {
        if (functionCall(&string, resDataType, 'a', symTable)) {
            stackDeconstruct(stack);
            stringDeconstruct(&string);
            return SUCCESS;
        } else {
            stackDeconstruct(stack);
            stringDeconstruct(&string);
            return SYNTAX_ERROR;
        }
    }

    int dataType = curLex;
    do {
        bool isID = false;
        // printStack(stack);
        if (curLex == LEX_EOF) {
            stackDeconstruct(stack);
            stringDeconstruct(&string);
            return SYNTAX_ERROR;
        } else if (curLex == LEX_ID) {
            node_t curID = TreeFind(*symTable, string.string);
            if (curID == NULL) {
                printf("STRING: %s\n", string.string);
                printf("Undefined Variable\n");
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return UNDEFVAR_ERROR;
            } else {
                isID = true;
                char initialized[9999];
                char typeinstruction[9999];
                unsigned long labelID = getLabel();
                sprintf(initialized, "checkinitializedgood%lu", labelID);
                sprintf(typeinstruction, "TYPE LF@$optype LF@%s",
                        string.string);
                // check initialization of var
                PRINT_CODE(write_text, typeinstruction);
                PRINT_CODE(write_text, "PUSHS LF@$optype");
                PRINT_CODE(push_string, "");
                PRINT_CODE(jumpIfNeqS, initialized);
                PRINT_CODE(write_text, "EXIT int@5");
                PRINT_CODE(label, initialized);
                PRINT_CODE(push_operand, string.string);
                dataType = curID->dataType;
            }
        }
        // printf("DATATYPE: %d\n", dataType);
        if (dataType == LEX_TYPE_FLOAT) {
            dataType = LEX_FLOAT;
        } else if (dataType == LEX_TYPE_INT) {
            dataType = LEX_INT;
        } else if (dataType == LEX_TYPE_STRING) {
            dataType = LEX_STRING;
        }
        if (dataType == LEX_INT || dataType == LEX_FLOAT ||
            dataType == LEX_STRING || dataType == LEX_BOOL ||
            dataType == LEX_EXPONENT || dataType == LEX_NULL ||
            dataType == LEX_TYPE_STRING_OPT || dataType == LEX_TYPE_INT_OPT ||
            dataType == LEX_TYPE_FLOAT_OPT) {
            curLex = LEX_I;
        }
        if (curLex == LEX_EOF) {
            stackDeconstruct(stack);
            stringDeconstruct(&string);
            return SYNTAX_ERROR;
        }
        // Condition for ending the precendence analysis
        if (curLex > 15) {
            if (curLex == endChar) {
                int finishRes = finishReducing(stack);

                if (finishRes == SUCCESS) {
                    // printStack(stack);
                    break;
                } else {
                    stackDeconstruct(stack);
                    stringDeconstruct(&string);
                    return finishRes;
                }
            } else {
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return SYNTAX_ERROR;
            }
        }
        char operation = table[stack->topNonTerm->tokenID - 1][curLex - 1];
        if (operation == 'X') {
            if (curLex == endChar) {
                break;
            } else {
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return SYNTAX_ERROR;
            }
        }
        if (operation == '<') {
            stackInsertHandle(stack);
            stackPush(stack, curLex, string);
            stack->topElement->dataType = dataType;
            stack->topElement->isID = isID;

            curLex = getTokenfromStore(&string);
            dataType = curLex;
        } else if (operation == '>') {
            // REDUCING EXPRESSION
            int reduceRes = reduceExpression(stack);
            if (reduceRes != SUCCESS) {
                printf("Error occured during the reduction\n");
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return reduceRes;
            }
            setTopNoTerm(stack);
        } else if (operation == '=') {
            stackPush(stack, curLex, string);
            curLex = getTokenfromStore(&string);
            dataType = curLex;
        }
    } while (endAnalysis == false);

    *resDataType = stack->items[1]->dataType;
    stackDeconstruct(stack);
    stringDeconstruct(&string);
    return SUCCESS;
}