#include "expression.h"

// E = E + E
int addRule(Stack *stack, StackElement *op1, StackElement *op2) {
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
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

// E = E - E
int subRule(Stack *stack, StackElement *op1, StackElement *op2) {
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
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

// E = E * E
int mulRule(Stack *stack, StackElement *op1, StackElement *op2) {
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
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

// E = E / E
int divRule(Stack *stack, StackElement *op1, StackElement *op2) {
    if ((op1->dataType == op2->dataType) &&
        (op1->dataType == LEX_INT || op1->dataType == LEX_FLOAT)) {
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
        stackPushElement(stack, popArr[0]);
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
    int dataType = curLex;

    do {
        // printStack(stack);
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
                dataType = curID->dataType;
            }
        }
        if (dataType == LEX_INT || dataType == LEX_FLOAT ||
            dataType == LEX_STRING || dataType == LEX_BOOL) {
            curLex = LEX_I;
        }
        if (curLex == LEX_EOF) {
            stackDeconstruct(stack);
            stringDeconstruct(&string);
            return SYNTAX_ERROR;
        }
        // Condition for ending the precendence analysis
        if (curLex > 14) {
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

            curLex = get_Token(&string);
            dataType = curLex;
        } else if (operation == '>') {
            // printf("REDUCE\n");
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
            dataType = curLex;
        }
    } while (endAnalysis == false);

    // printf("EXPRESSION PARSED SUCCESSFULLY\n");
    *resDataType = stack->items[1]->dataType;
    stackDeconstruct(stack);
    stringDeconstruct(&string);
    return SUCCESS;
}