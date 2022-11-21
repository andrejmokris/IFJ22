#include "expression.h"

#include "generator.h"
#include "parser.h"
#include "print_inst.h"

extern String_t code;
extern Tinstruction_list list;

// E = E + E
int addRule(Stack *stack, StackElement *op1, StackElement *op2) {
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        PRINT_CODE(put_OPERATOR, LEX_ADD);
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
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        PRINT_CODE(put_OPERATOR, LEX_SUB);
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
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        PRINT_CODE(put_OPERATOR, LEX_MUL);
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
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        PRINT_CODE(put_OPERATOR, LEX_DIV);
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
                // printf("PUSHS int@%s\n", popArr[0]->tokenVal.string);
            } else if (popArr[0]->dataType == LEX_FLOAT) {
                PRINT_CODE(push_float,
                           popArr[0]->tokenVal.string);  // TODO float hodnota
                // printf("PUSHS float@%a\n", popArr[0]->tokenVal.string);
            } else if (popArr[0]->dataType == LEX_STRING) {
                PRINT_CODE(push_string, popArr[0]->tokenVal.string);
                // printf("PUSHS float@%a\n", popArr[0]->tokenVal.string);
            } else if (popArr[0]->dataType == LEX_NULL) {
                PRINT_CODE(push_null, );
                // printf("PUSHS float@%a\n", popArr[0]->tokenVal.string);
            }
        }
        stackPushElement(stack, popArr[0]);
        // To generate code from it, push this to dataStack: popArr[0]->tokenVal
        return SUCCESS;
    }

    if (cnt == 3 && popArr[0]->tokenID == LEX_E &&
        popArr[2]->tokenID == LEX_E) {
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
                if (popArr[0]->dataType == popArr[2]->dataType) {
                    popArr[0]->dataType = LEX_BOOL;
                    stackPushElement(stack, popArr[0]);
                    elementDeconstruct(popArr[1]);
                    elementDeconstruct(popArr[2]);
                    PRINT_CODE(put_OPERATOR, LEX_EQ);
                    PRINT_CODE(put_OPERATOR, 0);

                    return SUCCESS;
                } else {
                    elementDeconstruct(popArr[1]);
                    elementDeconstruct(popArr[2]);
                    return TYPECOMP_ERORR;
                }
                break;
            case LEX_LEQ:
            case LEX_GTQ:
            case LEX_GT:
            case LEX_LE:
            case LEX_EQ:
                if (popArr[0]->dataType == popArr[2]->dataType) {
                    popArr[0]->dataType = LEX_BOOL;
                    stackPushElement(stack, popArr[0]);
                    elementDeconstruct(popArr[1]);
                    elementDeconstruct(popArr[2]);
                    PRINT_CODE(put_OPERATOR, LEX_EQ);

                    return SUCCESS;
                } else {
                    elementDeconstruct(popArr[1]);
                    elementDeconstruct(popArr[2]);
                    return TYPECOMP_ERORR;
                }
                break;
            case LEX_DOT:
                // CONCAT
                if (((popArr[0]->dataType == popArr[2]->dataType) &&
                     (popArr[0]->dataType == LEX_STRING ||
                      popArr[0]->dataType == LEX_NULL))) {
                    popArr[0]->dataType = LEX_STRING;
                    stackPushElement(stack, popArr[0]);
                    elementDeconstruct(popArr[1]);
                    elementDeconstruct(popArr[2]);
                    //PRINT_CODE(put_OPERATOR, LEX_EQ);
                    return SUCCESS;
                } else {
                    elementDeconstruct(popArr[1]);
                    elementDeconstruct(popArr[2]);
                    return TYPECOMP_ERORR;
                }
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

int parseExpression(int endChar, int *resDataType, node_t symTable) {
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
        if (functionCall(&string, resDataType, 'a')) {
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
        //printStack(stack);
        if (curLex == LEX_EOF) {
            stackDeconstruct(stack);
            stringDeconstruct(&string);
            return SYNTAX_ERROR;
        } else if (curLex == LEX_ID) {
            node_t curID = TreeFind(symTable, string.string);
            if (curID == NULL) {
                printf("Undefined Variable\n");
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return UNDEFVAR_ERROR;
            } else {
                isID = true;
                PRINT_CODE(push_operand, string.string);
                // printf("PUSHS GF@%s\n", string.string);
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
        // printf("OPERATION: %c\n", operation);
        // printf("CURLEX: %s\n", string.string);
        // printf("STLPEC: %d\n", curLex - 1);
        // printf("RIADOK: %d\n", stack->topNonTerm->tokenID - 1);
        // printf("TOP NONTERM: %s\n\n", stack->topNonTerm->tokenVal.string);
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
            //printf("REDUCE\n");
            int reduceRes = reduceExpression(stack);
            if (reduceRes != SUCCESS) {
                printf("CHYBA PRI REDUKCII\n");
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

    // printf("EXPRESSION PARSED SUCCESSFULLY\n");
    *resDataType = stack->items[1]->dataType;
    stackDeconstruct(stack);
    stringDeconstruct(&string);
    return SUCCESS;
}