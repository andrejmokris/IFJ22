#include "expression.h"

#include "generator.h"
#include "parser.h"
#include "print_inst.h"

extern String_t code;
extern Tinstruction_list list;

static char table[15][15] = {
    // 0    1    2    3    4    5    6    7    8    9    10   11   12  13   14
    // +    -    *    /   ===  !==   <    >   <=  >=     (    )    i    $    .
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
     '<'},  // $ 13
    {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>',
     '>'}  // . 14
};

// E = E + E
int addRule(Stack *stack, StackElement *op1, StackElement *op2) {
    unsigned long labelID = getLabel();
    char str1[99999];
    MAKE_VARS();
    sprintf(str1, "ADDFAIL%ld", labelID);
    TYPE_CONTROL("a", str1);
    TYPE_CONTROL("b", str1);
    CMP_VAR("a", "b", "res");
    sprintf(str1, "ADD%ld", labelID);
    PRINT_CODE(jumpIfEqS, str1);
    sprintf(str1, "ADDLABEL%ld", labelID);
    INT2FLOAT("res", "a", str1);
    sprintf(str1, "ADDLABEL2%ld", labelID);
    INT2FLOAT("res", "b", str1);
    sprintf(str1, "ADD%ld", labelID);
    PRINT_CODE(label, str1);
    PRINT_CODE(write_text, "ADD TF@res TF@a TF@b");
    PRINT_CODE(push_operandTF, "res");
    END_OPERATION(str1, labelID);
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        // PRINT_CODE(put_OPERATOR, LEX_ADD);
        return SUCCESS;
    } else if ((op1->dataType == LEX_FLOAT && op2->dataType == LEX_INT) ||
               (op1->dataType == LEX_INT && op2->dataType == LEX_FLOAT)) {
        op1->dataType = LEX_FLOAT;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return SUCCESS;
    }
    elementDeconstruct(op1);
    elementDeconstruct(op2);
    return TYPECOMP_ERORR;
}

// E = E - E
int subRule(Stack *stack, StackElement *op1, StackElement *op2) {
    unsigned long labelID = getLabel();
    char str1[99999];
    MAKE_VARS();
    sprintf(str1, "SUBFAIL%ld", labelID);
    TYPE_CONTROL("a", str1);
    TYPE_CONTROL("b", str1);
    CMP_VAR("a", "b", "res");
    sprintf(str1, "SUB%ld", labelID);
    PRINT_CODE(jumpIfEqS, str1);
    sprintf(str1, "SUBLABEL%ld", labelID);
    INT2FLOAT("res", "a", str1);
    sprintf(str1, "SUBLABEL2%ld", labelID);
    INT2FLOAT("res", "b", str1);
    sprintf(str1, "SUB%ld", labelID);
    PRINT_CODE(label, str1);
    PRINT_CODE(write_text, "SUB TF@res TF@a TF@b");
    PRINT_CODE(push_operandTF, "res");
    END_OPERATION(str1, labelID);
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        // PRINT_CODE(put_OPERATOR, LEX_SUB);
        return SUCCESS;
    } else if ((op1->dataType == LEX_FLOAT && op2->dataType == LEX_INT) ||
               (op1->dataType == LEX_INT && op2->dataType == LEX_FLOAT)) {
        op1->dataType = LEX_FLOAT;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return SUCCESS;
    }
    elementDeconstruct(op1);
    elementDeconstruct(op2);
    return TYPECOMP_ERORR;
}

// E = E * E
int mulRule(Stack *stack, StackElement *op1, StackElement *op2) {
    unsigned long labelID = getLabel();
    char str1[99999];
    MAKE_VARS();
    sprintf(str1, "MULFAIL%ld", labelID);
    TYPE_CONTROL("a", str1);
    TYPE_CONTROL("b", str1);
    CMP_VAR("a", "b", "res");
    sprintf(str1, "MULL%ld", labelID);
    PRINT_CODE(jumpIfEqS, str1);
    sprintf(str1, "MULLABEL%ld", labelID);
    INT2FLOAT("res", "a", str1);
    sprintf(str1, "MULLABEL2%ld", labelID);
    INT2FLOAT("res", "b", str1);
    sprintf(str1, "MULL%ld", labelID);
    PRINT_CODE(label, str1);
    PRINT_CODE(write_text, "MUL TF@res TF@a TF@b");
    PRINT_CODE(push_operandTF, "res");
    END_OPERATION(str1, labelID);

    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        // PRINT_CODE(put_OPERATOR, LEX_MUL);
        return SUCCESS;
    } else if ((op1->dataType == LEX_FLOAT && op2->dataType == LEX_INT) ||
               (op1->dataType == LEX_INT && op2->dataType == LEX_FLOAT)) {
        op1->dataType = LEX_FLOAT;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return SUCCESS;
    }
    elementDeconstruct(op1);
    elementDeconstruct(op2);
    return TYPECOMP_ERORR;
}

// E = E / E
int divRule(Stack *stack, StackElement *op1, StackElement *op2) {
    unsigned long labelID = getLabel();
    char str1[99999];
    MAKE_VARS();
    sprintf(str1, "DIVFAIL%ld", labelID);
    TYPE_CONTROL("a", str1);
    TYPE_CONTROL("b", str1);
    sprintf(str1, "DIVLABEL%ld", labelID);
    INT2FLOAT("res", "a", str1);
    sprintf(str1, "DIVLABEL2%ld", labelID);
    INT2FLOAT("res", "b", str1);
    PRINT_CODE(write_text, "DIV TF@res TF@a TF@b");
    PRINT_CODE(push_operandTF, "res");
    END_OPERATION(str1, labelID);

    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
        op1->dataType = LEX_FLOAT;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return SUCCESS;
    } else if ((op1->dataType == LEX_FLOAT && op2->dataType == LEX_INT) ||
               (op1->dataType == LEX_INT && op2->dataType == LEX_FLOAT)) {
        op1->dataType = LEX_FLOAT;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return SUCCESS;
    }
    elementDeconstruct(op1);
    elementDeconstruct(op2);
    return TYPECOMP_ERORR;
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

    // i -> E
    if (cnt == 1 && popArr[0]->tokenID == LEX_I) {
        popArr[0]->tokenID = LEX_E;
        if (popArr[0]->isID == false) {
            if (popArr[0]->dataType == LEX_INT) {
                PRINT_CODE(push_int, popArr[0]->tokenVal.string);
            } else if (popArr[0]->dataType == LEX_FLOAT) {
                char str[99999];
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
        char str1[99999];
        char str2[99999];
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
                PRINT_CODE(write_text, "TYPE TF@res TF@a");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(write_text, "TYPE TF@res TF@b");
                PRINT_CODE(push_operandTF, "res");
                labelID = getLabel();
                sprintf(str1, "NEQSLABEL1%ld", labelID);
                sprintf(str2, "NEQSLABEL2%ld", labelID);
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
                MAKE_VARS();
                PRINT_CODE(write_text, "LE TF@res TF@a TF@b");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(write_text, "EQ TF@res TF@a TF@b");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(put_OPERATOR, 69);
                return SUCCESS;
                break;
            case LEX_GTQ:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                MAKE_VARS();
                PRINT_CODE(write_text, "GT TF@res TF@a TF@b");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(write_text, "EQ TF@res TF@a TF@b");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(put_OPERATOR, 69);
                return SUCCESS;
                break;
            case LEX_GT:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                PRINT_CODE(put_OPERATOR, LEX_GT);
                return SUCCESS;
            case LEX_LE:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                PRINT_CODE(put_OPERATOR, LEX_LE);
                return SUCCESS;
            case LEX_EQ:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                MAKE_VARS();
                PRINT_CODE(write_text, "TYPE TF@res TF@a");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(write_text, "TYPE TF@res TF@b");
                PRINT_CODE(push_operandTF, "res");
                labelID = getLabel();
                sprintf(str1, "EQSLABEL1%ld", labelID);
                sprintf(str2, "EQSLABEL2%ld", labelID);
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
                // save operands into A and B
                MAKE_VARS();
                // Do the type check
                PRINT_CODE(write_text, "TYPE TF@res TF@a");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(push_string, "string");
                labelID = getLabel();
                sprintf(str1, "NCOMPCAT%ld", labelID);
                sprintf(str2, "COMPCAT%ld", labelID);
                PRINT_CODE(jumpIfNeqS, str1);
                // check second operand
                PRINT_CODE(write_text, "TYPE TF@res TF@b");
                PRINT_CODE(push_operandTF, "res");
                PRINT_CODE(push_string, "string");
                PRINT_CODE(jumpIfEqS, str2);
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

    int curLex = get_Token(&string);
    if (curLex == LEX_ERR) {
        stackDeconstruct(stack);
        stringDeconstruct(&string);
        return LEX_ERROR;
    }

    if (curLex == endChar && endChar == LEX_SEMICOL) {
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
                printf("Undefined Variable\n");
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return UNDEFVAR_ERROR;
            } else {
                isID = true;
                PRINT_CODE(push_operand, string.string);
                dataType = curID->dataType;
            }
        }
        if (dataType == LEX_TYPE_FLOAT) {
            dataType = LEX_FLOAT;
        } else if (dataType == LEX_TYPE_INT) {
            dataType = LEX_INT;
        } else if (dataType == LEX_TYPE_STRING) {
            dataType = LEX_STRING;
        }
        if (dataType == LEX_INT || dataType == LEX_FLOAT ||
            dataType == LEX_STRING || dataType == LEX_BOOL ||
            dataType == LEX_EXPONENT || dataType == LEX_NULL) {
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

            curLex = get_Token(&string);
            if (curLex == LEX_ERR) {
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return LEX_ERROR;
            }
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
            curLex = get_Token(&string);
            if (curLex == LEX_ERR) {
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return LEX_ERROR;
            }
            dataType = curLex;
        }
    } while (endAnalysis == false);

    *resDataType = stack->items[1]->dataType;
    stackDeconstruct(stack);
    stringDeconstruct(&string);
    return SUCCESS;
}