/*
 * IFJ 2022 PROJECT
 *
 * stack.c
 *
 * Andrej Mokris: xmokri01
 *
 */

#include "stack.h"

StackElement *createElement(int tokenID, String_t tokenVal) {
    StackElement *newElement = malloc(sizeof(Stack));
    String_t newString;

    if (newElement == NULL || StringInit(&newString) == false) {
        return NULL;
    }

    for (int i = 0; i < tokenVal.length; i++) {
        if (stringAppend(&newString, tokenVal.string[i]) == false) {
            return NULL;
        }
    }
    newElement->tokenVal = newString;
    newElement->tokenID = tokenID;
    newElement->dataType = tokenID;
    return newElement;
}

void elementDeconstruct(StackElement *element) {
    stringDeconstruct(&element->tokenVal);
    free(element);
}

Stack *initStack(size_t size) {
    Stack *newStack = malloc(sizeof(struct stack));
    if (newStack == NULL) {
        return NULL;
    }
    newStack->items = malloc(size * sizeof(StackElement *));
    newStack->stackSize = size;
    newStack->elementCount = 0;
    newStack->topElement = NULL;
    newStack->topNonTerm = NULL;

    return newStack;
}

bool stackPush(Stack *stack, int tokenID, String_t tokenVal) {
    StackElement *newElement = createElement(tokenID, tokenVal);
    if (newElement == NULL || stack == NULL) {
        return false;
    }
    if (stack->elementCount < stack->stackSize) {
        newElement->arrayIndex = stack->elementCount;
        stack->items[stack->elementCount] = newElement;
        stack->elementCount++;
        stack->topElement = newElement;
        // check if item is non-terminal
        if (tokenID >= 1 && tokenID <= 15 && tokenID != LEX_BEG_HANDLE) {
            stack->topNonTerm = newElement;
        }
        return true;
    }
    return false;
}

void setTopNoTerm(Stack *stack) {
    for(int i = stack->elementCount - 1; i >= 0; i--) {
        int tokenID = stack->items[i]->tokenID;
        if (tokenID >= 1 && tokenID <= 15 && tokenID != LEX_BEG_HANDLE) {
            stack->topNonTerm = stack->items[i];
            return;
        }
    }
}

bool stackPushElement(Stack *stack, StackElement *newElement) {
    if (newElement == NULL || stack == NULL) {
        return false;
    }
    if (stack->elementCount < stack->stackSize) {
        newElement->arrayIndex = stack->elementCount;
        stack->items[stack->elementCount] = newElement;
        stack->elementCount++;
        stack->topElement = newElement;
        return true;
    }
    return false;
}

bool stackInsertHandle(Stack *stack) {
    String_t string;
    StringInit(&string);
    stringAppend(&string, '<');
    StackElement *newElement = createElement(LEX_BEG_HANDLE, string);
    if (newElement == NULL || stack == NULL) {
        return false;
    }
    for (size_t i = stack->topElement->arrayIndex;
         i > stack->topNonTerm->arrayIndex; i--) {
            stack->items[i+1] = stack->items[i];
    }
    stack->items[stack->topNonTerm->arrayIndex + 1] = newElement;
    stack->elementCount++;
    stringDeconstruct(&string);
    return true;
}

StackElement *stackPop(Stack *stack) {
    if (stack == NULL || stack->elementCount == 0) {
        return NULL;
    } else {
        stack->elementCount--;
        StackElement *tmp = stack->topElement;
        stack->items[stack->elementCount] = NULL;
        stack->topElement = stack->items[(stack->elementCount) - 1];
        return tmp;
    }
}

void printStack(Stack *stack) {
    for (size_t i = 0; i < stack->elementCount; i++) {
        //printf("(ID: %d | VAL: %s | TYPE: %d) ", stack->items[i]->tokenID,
        //     stack->items[i]->tokenVal.string, stack->items[i]->dataType);
        if(stack->items[i]->tokenID == LEX_E) {
            printf("E ");
        } else if(stack->items[i]->tokenID == LEX_I) {
            printf("i ");
        } else {
            printf("%s ", stack->items[i]->tokenVal.string);
        }
    }
    printf("\n");
}

void stackDeconstruct(Stack *stack) {
    for (size_t i = 0; i < stack->elementCount; i++) {
        elementDeconstruct(stack->items[i]);
    }
    free(stack->items);
    free(stack);
}
