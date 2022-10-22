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
    return newElement;
}

void elementDeconstruct(StackElement *element) {
    stringDeconstruct(&element->tokenVal);
    free(element);
}

Stack *initStack(size_t size) {
    Stack *newStack = malloc(sizeof(Stack) + size * (sizeof(StackElement *)));
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
        stack->items[stack->elementCount] = newElement;
        stack->elementCount++;
        stack->topElement = newElement;
        // for debug purpose - true to be deleted, when we know element
        if (true) {
            stack->topNonTerm = newElement;
        }
        return true;
    }
    return false;
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
        printf("ID: %d  |   VAL: %s\n", stack->items[i]->tokenID,
               stack->items[i]->tokenVal.string);
    }
}

void stackDeconstruct(Stack *stack) {
    for (size_t i = 0; i < stack->elementCount; i++) {
        elementDeconstruct(stack->items[i]);
    }
    free(stack->items);
    free(stack);
}
