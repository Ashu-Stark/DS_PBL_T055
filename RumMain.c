#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StackFunc.h"
#include "MainFunc.h"
#include "stack.h"
#include "queue.h"
#include "QueueFunc.h"
#include "semicolon.h"
#include "Braces.h"
#include "variable.h"
#include "functionstore.h"
#include "function1.h"
struct func f[NUMS];
int funcCount = 0;
int main(int argc, char *argv[])
{
    struct queue *q = create();
    int keywordcount = 42;
    char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do",
                        "double", "else", "enum", "extern", "float", "for", "goto", "if",
                        "inline", "int", "long", "register", "restrict", "return", "short",
                        "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
                        "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary", "printf", "scanf", "strlen", "strcpy", "strcat", "strcmp"};
    struct stack *bracestack = initialize();
    char filename[500] = "input.c";
    if (argc > 1)
    {
        strcpy(filename, argv[1]);
    }
    FILE *fp = fopen(filename, "r");
    char line[500];
    int l = 0;
    int inMain = 0;

    if (!fp)
    {
        printf("File not found\n");
        return 0;
    }

    while (fgets(line, sizeof(line), fp))
    {
        l++;
        removespace(line);
        removeComments(line);

        if (line[0] == '\0' || line[0] == '\n')
        {
            continue;
        }
        if (strncmp(line, "#include", 8) == 0)
        {
            continue;
        }

        braces(line, l, q, bracestack);
        semicolon(line, l, q);
        variable(line, l, q, keywordcount, keywords);
        checkFunctions(line, l, f, q, &inMain);
    }

    checkremain(bracestack, q);

    printf("The list of Error is given below:-\n\n");
    print(q);

    fclose(fp);
    return 0;
}
