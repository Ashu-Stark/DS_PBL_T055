#ifndef MAINFUNC_H
#define MAINFUNC_H

#include "QueueFunc.h"

int checknum(char );

int checkdigit(char );

int identifier(char *);

int keyword(char *, int , char *[]);

int datatype(char *);

void removespace(char *);

void removeComments(char *);

int addvar(char *, char *, int , struct queue *);

int present(char *);

int token_func(char *line,char ftoken[][50]);

#endif