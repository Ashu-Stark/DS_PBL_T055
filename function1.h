#ifndef FUNCTION1_H
#define FUNCTION1_H

#include "queue.h"
#include "functionstore.h"

// Detect and store function definitions
void detectFunctionDefinition(char *line, int lineNum, struct func *f, struct queue *q);

// Detect and store function declarations
void detectFunctionDeclaration(char *line, int lineNum, struct func *f, struct queue *q);

// Check function calls and validate them
void checkFunctionCalls(char *line, int lineNum, struct func *f, struct queue *q, int inMain);

// Main function checker - processes a line for all function-related checks
void checkFunctions(char *line, int lineNum, struct func *f, struct queue *q, int *inMain);

// Helper function to extract function name from a call
int extractFunctionCall(char *line, char *funcName);

// Helper function to check if we're in main function
int isInMainFunction(char *line);

#endif


