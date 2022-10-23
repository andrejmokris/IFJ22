#include "expression.h"

bool addRule(Stack *stack, StackElement *op1, StackElement *op2) {
    if (op1->dataType == op2->dataType) {
        op1->tokenID = LEX_E;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return true;
    } else if (op1->dataType == LEX_FLOAT && op2->dataType == LEX_INT) {
        op1->tokenID = LEX_E;
        op1->dataType = LEX_FLOAT;
        stackPushElement(stack, op1);
        elementDeconstruct(op2);
        return true;
    } else if (op1->dataType == LEX_INT && op2->dataType == LEX_FLOAT) {
        op1->tokenID = LEX_E;
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
                    //elementDeconstruct(popArr[1]);
                    return true;
                }
                printf("semantic error\n");
                return SEMANTIC_ERROR;
            case LEX_SUB:
                // E-E -> E
                popArr[0]->tokenID = LEX_E;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                return true;
            case LEX_MUL:
                // E*E -> E
                popArr[0]->tokenID = LEX_E;
                stackPushElement(stack, popArr[0]);
                elementDeconstruct(popArr[1]);
                elementDeconstruct(popArr[2]);
                return true;
            case LEX_DIV:
                // E/E -> E
                popArr[0]->tokenID = LEX_E;
                popArr[0]->dataType = LEX_FLOAT;
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

int parseExpression() {
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
        if (curLex == LEX_INT || curLex == LEX_FLOAT) {
            curLex = LEX_I;
        }
        char operation = table[stack->topNonTerm->tokenID - 1][curLex - 1];
        if (operation == 'A' && stack->elementCount == 2) {
            break;
        }
        if (operation == 'X') {
            printf("CHYBA\n");
            return SYNTAX_ERROR;
        } else if (operation == '<') {
            stackInsertHandle(stack);
            stackPush(stack, curLex, string);
            stack->topElement->dataType = dataType;
            
            curLex = get_Token(&string);
            dataType = curLex;
            if (curLex == LEX_ERR) {
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return LEX_ERROR;
            }
            if (curLex == LEX_EOF || curLex == LEX_SEMICOL) {
                curLex = LEX_DOLLAR;
            }
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
            if (curLex == LEX_ERR) {
                stackDeconstruct(stack);
                stringDeconstruct(&string);
                return LEX_ERROR;
            }
            if (curLex == LEX_EOF || curLex == LEX_SEMICOL) {
                curLex = LEX_DOLLAR;
            }
        }
        printf("\n");
    } while (!endAnalysis);

    printf("Vysledny datovy typ: %d\n", stack->items[1]->dataType);
    stackDeconstruct(stack);
    stringDeconstruct(&string);
    return NO_ERROR;
}