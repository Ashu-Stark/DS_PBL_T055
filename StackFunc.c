#include <stdio.h>
#include <stdlib.h>
#include "StackFunc.h"
#include "stack.h"

void push(struct stack *s, char val, int line) {
    if (s->top + 1 == 100) {
        printf("Overflow\n");
        return;
    }
    s->top++;
    s->arr[s->top].ch = val;
    s->arr[s->top].line = line;
}

struct snode top(struct stack *s) {
    if (s->top == -1) {
        printf("Underflow\n");
    }
    else{
    return s->arr[s->top];
    }
}

struct snode pop(struct stack *s) {
    if (s->top == -1) {
        printf("Underflow\n");
    }
    else{
        return s->arr[s->top--];
    }
}

int isEmpty(struct stack *s) {
    return s->top == -1;
}

struct stack *initialize() {
    struct stack *temp = (struct stack *)malloc(sizeof(struct stack));
    temp->top = -1;
    return temp;
}
