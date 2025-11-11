#ifndef STACK_H
#define STACK_H

struct snode {
    char ch;
    int line;
};

struct stack {
    struct snode arr[100];
    int top;
};

#endif
