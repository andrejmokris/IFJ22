#ifndef _STACK
#define _STACK

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"
#include "scanner.h"

#define DOLLAR_SIGN 0

typedef struct stackElement {
    int tokenID;
    int dataType;
    size_t arrayIndex;
    String_t tokenVal;
} StackElement;

typedef struct stack {
    size_t stackSize;
    size_t elementCount;
    StackElement *topElement;
    StackElement *topNonTerm;
    struct stackElement **items;
} Stack;

bool stackPush(Stack *stack, int tokenID, String_t tokenVal);
bool stackPushElement(Stack *stack, StackElement *newElement);
StackElement *stackPop(Stack *stack);
Stack *initStack(size_t size);
void printStack(Stack *stack);
void setTopNoTerm(Stack *stack);
void stackDeconstruct(Stack *stack);
void elementDeconstruct(StackElement *element);
bool stackInsertHandle(Stack *stack);

#endif