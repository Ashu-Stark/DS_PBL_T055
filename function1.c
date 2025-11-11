#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "function1.h"
#include "queue.h"
#include "MainFunc.h"
#include "functionstore.h"

// Track function definitions
void detectFunctionDefinition(char *line, int lineNum, struct func *f, struct queue *q)
{
    char ftoken[50][50];
    int count = token_func(line, ftoken);
    
    if (count < 3) return;
    
    // Check if it's a function definition: datatype identifier ( ... ) {
    // Check if ftoken[0] is a datatype (without requiring space)
    int isDatatype = (strcmp(ftoken[0], "int") == 0 || strcmp(ftoken[0], "float") == 0 || 
                      strcmp(ftoken[0], "char") == 0 || strcmp(ftoken[0], "double") == 0 ||
                      strcmp(ftoken[0], "void") == 0);
    if (isDatatype && identifier(ftoken[1]) && strcmp(ftoken[2], "(") == 0)
    {
        // Find the ending - should be '{' for definition
        char *ending = ftoken[count - 1];
        
        if (strcmp(ending, "{") == 0)
        {
            // Check if function already exists
            int found = 0;
            int foundIndex = -1;
            
            for (int i = 0; i < funcCount; i++)
            {
                if (strcmp(f[i].fname, ftoken[1]) == 0)
                {
                    found = 1;
                    foundIndex = i;
                    break;
                }
            }
            
            if (found)
            {
                // Check if already defined
                if (f[foundIndex].fdefine == 1)
                {
                    char err[200];
                    sprintf(err, "Function redefined at line %d: %s\n", lineNum, ftoken[1]);
                    enque(q, err);
                }
                else
                {
                    // Update declaration to definition
                    f[foundIndex].fdefine = 1;
                }
            }
            else
            {
                // New function definition
                strcpy(f[funcCount].fname, ftoken[1]);
                strcpy(f[funcCount].ftype, ftoken[0]);
                f[funcCount].fdefine = 1;
                funcCount++;
            }
        }
    }
}

// Track function declarations
void detectFunctionDeclaration(char *line, int lineNum, struct func *f, struct queue *q)
{
    char ftoken[50][50];
    int count = token_func(line, ftoken);
    
    if (count < 3) return;
    
    // Check if it's a function declaration: datatype identifier ( ... ) ;
    // Check if ftoken[0] is a datatype (without requiring space)
    int isDatatype = (strcmp(ftoken[0], "int") == 0 || strcmp(ftoken[0], "float") == 0 || 
                      strcmp(ftoken[0], "char") == 0 || strcmp(ftoken[0], "double") == 0 ||
                      strcmp(ftoken[0], "void") == 0);
    if (isDatatype && identifier(ftoken[1]) && strcmp(ftoken[2], "(") == 0)
    {
        char *ending = ftoken[count - 1];
        
        if (strcmp(ending, ";") == 0)
        {
            // Check if function already exists
            int found = 0;
            int foundIndex = -1;
            
            for (int i = 0; i < funcCount; i++)
            {
                if (strcmp(f[i].fname, ftoken[1]) == 0)
                {
                    found = 1;
                    foundIndex = i;
                    break;
                }
            }
            
            if (found)
            {
                // Check for multiple declarations
                if (f[foundIndex].fdefine == 0)
                {
                    char err[200];
                    sprintf(err, "Multiple declarations of function at line %d: %s\n", lineNum, ftoken[1]);
                    enque(q, err);
                }
                // If already defined, declaration is fine (forward declaration)
            }
            else
            {
                // New function declaration
                strcpy(f[funcCount].fname, ftoken[1]);
                strcpy(f[funcCount].ftype, ftoken[0]);
                f[funcCount].fdefine = 0; // 0 = declared, 1 = defined
                funcCount++;
            }
        }
    }
}

// Extract function name from a function call
int extractFunctionCall(char *line, char *funcName)
{
    int len = strlen(line);
    int i = 0, j = 0;
    int inString = 0;
    int startIdx = -1;
    
    // Find any identifier followed by '(' - this indicates a function call
    for (i = 0; i < len; i++)
    {
        if (line[i] == '"' && (i == 0 || line[i - 1] != '\\'))
        {
            inString = !inString;
            continue;
        }
        
        if (inString)
        {
            continue;
        }
        
        // Check if we found an identifier character
        if ((line[i] >= 'a' && line[i] <= 'z') || 
            (line[i] >= 'A' && line[i] <= 'Z') || 
            line[i] == '_')
        {
            if (startIdx == -1)
            {
                startIdx = i; // Start of potential function name
            }
        }
        else if (line[i] >= '0' && line[i] <= '9' && startIdx != -1)
        {
            // Continue building identifier (numbers allowed after first char)
        }
        else
        {
            // We hit a non-identifier character
            if (startIdx != -1)
            {
                // Check if current or next non-space character is '('
                int k = i;
                while (k < len && (line[k] == ' ' || line[k] == '\t'))
                    k++;
                
                if (k < len && line[k] == '(')
                {
                    // Found function call! Extract the name
                    for (int m = startIdx; m < i && j < 99; m++)
                    {
                        funcName[j++] = line[m];
                    }
                    funcName[j] = '\0';
                    return 1; // Found function call
                }
                // Not a function call, reset
                startIdx = -1;
            }
        }
    }
    
    // Check end of line case
    if (startIdx != -1)
    {
        int k = i;
        while (k < len && (line[k] == ' ' || line[k] == '\t'))
            k++;
        if (k < len && line[k] == '(')
        {
            for (int m = startIdx; m < i && j < 99; m++)
            {
                funcName[j++] = line[m];
            }
            funcName[j] = '\0';
            return 1;
        }
    }
    
    funcName[0] = '\0';
    return 0;
}

// Check if we're inside main function
int isInMainFunction(char *line)
{
    // Simple check: if line contains "int main" or we're tracking main
    // This is a simplified version - you might want to track main scope more carefully
    if (strstr(line, "int main") != NULL || strstr(line, "void main") != NULL)
    {
        return 1;
    }
    return 0;
}

// Check function calls
void checkFunctionCalls(char *line, int lineNum, struct func *f, struct queue *q, int inMain)
{
    char funcName[100];
    char tline[500];
    strcpy(tline, line);
    
    // Remove spaces for easier parsing
    removespace(tline);
    
    // Skip if it's a function definition or declaration
    if (datatype(tline))
    {
        return;
    }
    
    // Extract function call
    if (extractFunctionCall(tline, funcName))
    {
        // Skip keywords that look like function calls
        char *keywords[] = {"if", "while", "for", "switch", "return", "printf", "scanf", 
                           "strlen", "strcpy", "strcat", "strcmp", "malloc", "free", 
                           "sizeof", "main"};
        int keywordCount = 15;
        
        int isKeyword = 0;
        for (int i = 0; i < keywordCount; i++)
        {
            if (strcmp(funcName, keywords[i]) == 0)
            {
                isKeyword = 1;
                break;
            }
        }
        
        if (isKeyword)
        {
            return;
        }
        
        // Check if function exists in our list
        int found = 0;
        int isDefined = 0;
        int isDeclared = 0;
        
        for (int i = 0; i < funcCount; i++)
        {
            if (strcmp(f[i].fname, funcName) == 0)
            {
                found = 1;
                if (f[i].fdefine == 1)
                {
                    isDefined = 1;
                }
                else
                {
                    isDeclared = 1;
                }
                break;
            }
        }
        
        if (!found)
        {
            // Function not declared or defined at all
            char err[200];
            sprintf(err, "Function not declared/defined at line %d: %s\n", lineNum, funcName);
            enque(q, err);
        }
        else if (!isDefined && inMain)
        {
            // Function called in main but only declared, not defined
            char err[200];
            sprintf(err, "Function called in main but not defined at line %d: %s\n", lineNum, funcName);
            enque(q, err);
        }
        // If function is declared but not defined and called outside main,
        // we don't flag it as it might be defined later in the file
    }
}

// Main function checker - processes a line for function definitions, declarations, and calls
void checkFunctions(char *line, int lineNum, struct func *f, struct queue *q, int *inMain)
{
    char tline[500];
    strcpy(tline, line);
    removespace(tline);
    
    // Check if entering main function
    if (strstr(tline, "int main") != NULL || strstr(tline, "void main") != NULL)
    {
        *inMain = 1;
    }
    
    // Check if exiting main function (simplified - looks for closing brace)
    // This is a basic implementation - you might want to track braces more carefully
    
    // First check for function definition
    detectFunctionDefinition(tline, lineNum, f, q);
    
    // Then check for function declaration
    detectFunctionDeclaration(tline, lineNum, f, q);
    
    // Finally check for function calls
    checkFunctionCalls(tline, lineNum, f, q, *inMain);
}

