// This is the main fucntion.
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
// NEW CHANGE: Added function1.h for new function declaration and definition checker
#include "function1.h"
struct func f[NUMS];
int funcCount = 0;
int main()
{
    struct queue *q = create();
    int keywordcount = 42;
    char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do",
                        "double", "else", "enum", "extern", "float", "for", "goto", "if",
                        "inline", "int", "long", "register", "restrict", "return", "short",
                        "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
                        "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary", "printf", "scanf", "strlen", "strcpy", "strcat", "strcmp"};
    struct stack *bracestack = initialize();
    FILE *fp = fopen("input.c", "r");
    char line[500];
    int l = 0;
    // NEW CHANGE: Added variable to track if we're inside main function for function checking
    int inMain = 0; // Track if we're inside main function

    if (!fp)
    {
        printf("File not found\n");
        return 0;
    }

    while (fgets(line, sizeof(line), fp))
    {
        l++;
        // Removing space

        removespace(line);

        // Remove comments
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

        // Semicolon check
        semicolon(line, l, q);

        // VARIABLE CHECK
        variable(line, l, q, keywordcount, keywords);
        
        // NEW CHANGE: Added function checker to detect function definitions, declarations, and calls
        // This checks for: function definitions, declarations, calls, undefined functions, 
        // functions called in main without definition, and multiple declarations
        checkFunctions(line, l, f, q, &inMain);
        // printf("%s\n",line);
    }

    //  REMAINING OPEN BRACES
    checkremain(bracestack, q);

    printf("The list of Error is given below:-\n\n");
    print(q);

    fclose(fp);
    return 0;
}
