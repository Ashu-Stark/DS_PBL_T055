#ifndef FUNCTION1_H
#define FUNCTION1_H

#include "queue.h"
#include "functionstore.h"

void detectFunctionDefinition(char *line, int lineNum, struct func *f, struct queue *q);

void detectFunctionDeclaration(char *line, int lineNum, struct func *f, struct queue *q);

void checkFunctionCalls(char *line, int lineNum, struct func *f, struct queue *q, int inMain);

void checkFunctions(char *line, int lineNum, struct func *f, struct queue *q, int *inMain);

int extractFunctionCall(char *line, char *funcName);

int isInMainFunction(char *line);

#endif


