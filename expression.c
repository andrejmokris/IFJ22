#include "expression.h"

bool addRule(Stack *stack, StackElement *op1, StackElement *op2) {
    if (op1->dataType == op2->dataType) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return true;
    } else if ((op1->dataType == LEX_FLOAT && op2->dataType == LEX_INT) ||
               (op1->dataType == LEX_INT && op2->dataType == LEX_FLOAT)) {
        op1->dataType = LEX_FLOAT;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return true;
    }
    elementDeconstruct(op1);
    elementDeconstruct(op2);
    return false;
}

bool mulRule(Stack *stack, StackElement *op1, StackElement *op2) {
    if (op1->dataType == op2->dataType) {
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return true;
    } else if ((op1->dataType == LEX_FLOAT && op2->dataType == LEX_INT) ||
               (op1->dataType == LEX_INT && op2->dataType == LEX_FLOAT)) {
        op1->dataType = LEX_FLOAT;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return true;
    }
    elementDeconstruct(op1);
    elementDeconstruct(op2);
    return false;
}

bool reduceExpression(Stack *stack) {
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
        return true;
    }

    if (cnt == 3 && popArr[0]->tokenID == LEX_E &&
        popArr[2]->tokenID == LEX_E) {
        switch (popArr[1]->tokenID) {
            case LEX_ADD:
                // E+E -> E
                elementDeconstruct(popArr[1]);
                if (addRule(stack, popArr[0], popArr[2])) {
                    return true;
                }
                printf("semantic error\n");
                return SEMANTIC_ERROR;
            case LEX_SUB:
                // E-E -> E
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                return true;
            case LEX_MUL:
                // E*E -> E
                elementDeconstruct(popArr[1]);
                if (mulRule(stack, popArr[0], popArr[2])) {
                    return true;
                }
                printf("semantic error\n");
                return SEMANTIC_ERROR;
                return true;
            case LEX_DIV:
                // E/E -> E
                popArr[0]->dataType = LEX_FLOAT;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                return true;
            case LEX_NEQ:
            case LEX_LEQ:
            case LEX_GTQ:
            case LEX_GT:
            case LEX_LE:
            case LEX_EQ:
                popArr[0]->dataType = LEX_BOOL;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                return true;
            default:
                return false;
        }
    }
    if (cnt == 3 && popArr[0]->tokenID == LEX_RPAR &&
        popArr[2]->tokenID == LEX_LPAR) {
        if (popArr[1]->tokenID == LEX_E) {
            stackPushElement(stack, popArr[1]);
            elementDeconstruct(popArr[0]);
            elementDeconstruct(popArr[2]);
            return true;
        }
    }
    return false;
}

bool finishReducing(Stack *stack) {
    bool res = true;
    while (res == true && stack->elementCount > 2) {
        printStack(stack);
        res = reduceExpression(stack);
    }
    return res;
}

int parseExpression(int endChar, int *resDataType) {
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
    int dataType = curLex;

    if (curLex == LEX_ERR) {
        stackDeconstruct(stack);
        stringDeconstruct(&string);
        return LEX_ERROR;
    }
    do {
        printStack(stack);
        printf("CUR CHAR: %d\n", curLex);
        printf("\n");
        if (curLex == LEX_EOF) {
            stackDeconstruct(stack);
            stringDeconstruct(&string);
            return SYNTAX_ERROR;
        }
        if (dataType == LEX_INT || dataType == LEX_FLOAT ||
            dataType == LEX_STRING) {
            curLex = LEX_I;
        }
        if (curLex == LEX_EOF) {
            stackDeconstruct(stack);
            stringDeconstruct(&string);
            return SYNTAX_ERROR;
        }
        if (curLex > 14) {
            if (curLex == endChar) {
                printf("finish reduce\n");
                if (finishReducing(stack)) {
                    printStack(stack);
                    break;
                } else {
                    stackDeconstruct(stack);
                    stringDeconstruct(&string);
                    return SYNTAX_ERROR;
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
            printf("REDUCE\n");
            if (!reduceExpression(stack)) {
                printf("CHYBA PRI REDUKCII\n");
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return SYNTAX_ERROR;
            }
            setTopNoTerm(stack);
        } else if (operation == '=') {
            stackPush(stack, curLex, string);
            curLex = get_Token(&string);
            dataType = curLex;
        }
    } while (endAnalysis == false);

    printf("Vysledny datovy typ: %d\n", stack->items[1]->dataType);
    *resDataType = stack->items[1]->dataType;
    stackDeconstruct(stack);
    stringDeconstruct(&string);
    return SUCCESS;
}