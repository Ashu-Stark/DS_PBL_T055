#ifndef BRACES_H
#define BRACES_H

#include"queue.h"

void braces(char *,int ,struct queue*,struct stack *);

void checkremain(struct stack *bracestack,struct queue *q);
#endif