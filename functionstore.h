#ifndef FUNCTIONSTORE_H
#define FUNCTIONSTORE_H

#define NUMS 500

struct func{
    char fname[100];
    char ftype[100];
    int fdefine;
};

extern struct func f[NUMS];
extern int funcCount;
#endif