#ifndef STACKFUNC_H
#define STACKFUNC_H

#include "stack.h"

void push(struct stack *, char, int);
struct snode top(struct stack *);
struct snode pop(struct stack *);
int isEmpty(struct stack *);
struct stack *initialize();

#endif
