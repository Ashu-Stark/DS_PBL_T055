#ifndef VARIABLESTORE_H
#define VARIABLESTORE_H

#define LIMIT 500

typedef struct Variable
{
    char name[50];
    char type[20];
    int line;
} Variable;
Variable varList[LIMIT];
int varCount = 0;

#endif