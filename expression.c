#include "expression.h"

bool reduceExpression(Stack *stack) {
    StackElement *popArr[3];
    int cnt = 0;
    StackElement *lastPopped;
    while ((lastPopped = stackPop(stack)) &&
           lastPopped->tokenID != LEX_BEG_HANDLE) {
        popArr[cnt] = lastPopped;
        cnt++;
    }
    // i -> E
    if (cnt == 1 && popArr[0]->tokenID == LEX_I) {
        popArr[0]->tokenID = LEX_E;
        stackPushElement(stack, popArr[0]);
        return true;
    }
    if (cnt == 3 && popArr[0]->tokenID == LEX_E &&
        popArr[2]->tokenID == LEX_E) {
        // E = E + E
        if (popArr[1]->tokenID == LEX_ADD) {
            popArr[0]->tokenID = LEX_E;
            stackPushElement(stack, popArr[0]);
            elementDeconstruct(popArr[1]);
            elementDeconstruct(popArr[2]);
            return true;
            // E = E * E
        } else if (popArr[1]->tokenID == LEX_MUL) {
            popArr[0]->tokenID = LEX_E;
            stackPushElement(stack, popArr[0]);
            elementDeconstruct(popArr[1]);
            elementDeconstruct(popArr[2]);
            return true;
        } else if (popArr[1]->tokenID == LEX_SUB) {
            popArr[0]->tokenID = LEX_E;
            stackPushElement(stack, popArr[0]);
            elementDeconstruct(popArr[1]);
            elementDeconstruct(popArr[2]);
            return true;
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
    bool endAnalysis = false;
    Stack *stack = initStack(STACK_INIT_SIZE);
    if (!StringInit(&string) || stack == NULL || !stringAppend(&string, '$') ||
        !stackPush(stack, LEX_DOLLAR, string)) {
        stackDeconstruct(stack);
        stringDeconstruct(&string);
        return INTERNAL_ERROR;
    }
    int curLex = get_Token(&string);
    while (!endAnalysis && curLex != LEX_EOF) {
        printStack(stack);
        if (curLex == LEX_INT || curLex == LEX_FLOAT) {
            curLex = LEX_I;
        }
        char operation = table[stack->topNonTerm->tokenID - 1][curLex - 1];
        if (operation == 'X') {
            printf("CHYBA\n");
            return SYNTAX_ERROR;
        } else if (operation == '<') {
            stackInsertHandle(stack);
            stackPush(stack, curLex, string);
            curLex = get_Token(&string);
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
        }
        printf("\n");
    }

    stackDeconstruct(stack);
    stringDeconstruct(&string);
    return true;
}