#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "function1.h"
#include "queue.h"
#include "MainFunc.h"
#include "functionstore.h"

void detectFunctionDefinition(char *line, int lineNum, struct func *f, struct queue *q)
{
    char ftoken[50][50];
    int count = token_func(line, ftoken);
    
    if (count < 3) return;
    
    int isDatatype = (strcmp(ftoken[0], "int") == 0 || strcmp(ftoken[0], "float") == 0 || 
                      strcmp(ftoken[0], "char") == 0 || strcmp(ftoken[0], "double") == 0 ||
                      strcmp(ftoken[0], "void") == 0);
    if (isDatatype && identifier(ftoken[1]) && strcmp(ftoken[2], "(") == 0)
    {
        char *ending = ftoken[count - 1];
        
        if (strcmp(ending, "{") == 0)
        {
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
                if (f[foundIndex].fdefine == 1)
                {
                    char err[200];
                    sprintf(err, "Function redefined at line %d: %s\n", lineNum, ftoken[1]);
                    enque(q, err);
                }
                else
                {
                    f[foundIndex].fdefine = 1;
                }
            }
            else
            {
                strcpy(f[funcCount].fname, ftoken[1]);
                strcpy(f[funcCount].ftype, ftoken[0]);
                f[funcCount].fdefine = 1;
                funcCount++;
            }
        }
    }
}

void detectFunctionDeclaration(char *line, int lineNum, struct func *f, struct queue *q)
{
    char ftoken[50][50];
    int count = token_func(line, ftoken);
    
    if (count < 3) return;
    
    int isDatatype = (strcmp(ftoken[0], "int") == 0 || strcmp(ftoken[0], "float") == 0 || 
                      strcmp(ftoken[0], "char") == 0 || strcmp(ftoken[0], "double") == 0 ||
                      strcmp(ftoken[0], "void") == 0);
    if (isDatatype && identifier(ftoken[1]) && strcmp(ftoken[2], "(") == 0)
    {
        char *ending = ftoken[count - 1];
        
        if (strcmp(ending, ";") == 0)
        {
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
                if (f[foundIndex].fdefine == 0)
                {
                    char err[200];
                    sprintf(err, "Multiple declarations of function at line %d: %s\n", lineNum, ftoken[1]);
                    enque(q, err);
                }
            }
            else
            {
                strcpy(f[funcCount].fname, ftoken[1]);
                strcpy(f[funcCount].ftype, ftoken[0]);
                f[funcCount].fdefine = 0;
                funcCount++;
            }
        }
    }
}

int extractFunctionCall(char *line, char *funcName)
{
    int len = strlen(line);
    int i = 0, j = 0;
    int inString = 0;
    int startIdx = -1;
    
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
        
        if ((line[i] >= 'a' && line[i] <= 'z') || 
            (line[i] >= 'A' && line[i] <= 'Z') || 
            line[i] == '_')
        {
            if (startIdx == -1)
            {
                startIdx = i;
            }
        }
        else if (line[i] >= '0' && line[i] <= '9' && startIdx != -1)
        {
        }
        else
        {
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
                startIdx = -1;
            }
        }
    }
    
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

int isInMainFunction(char *line)
{
    if (strstr(line, "int main") != NULL || strstr(line, "void main") != NULL)
    {
        return 1;
    }
    return 0;
}

void checkFunctionCalls(char *line, int lineNum, struct func *f, struct queue *q, int inMain)
{
    char funcName[100];
    char tline[500];
    strcpy(tline, line);
    
    removespace(tline);
    
    if (datatype(tline))
    {
        return;
    }
    
    if (extractFunctionCall(tline, funcName))
    {
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
            char err[200];
            sprintf(err, "Function not declared/defined at line %d: %s\n", lineNum, funcName);
            enque(q, err);
        }
        else if (!isDefined && inMain)
        {
            char err[200];
            sprintf(err, "Function called in main but not defined at line %d: %s\n", lineNum, funcName);
            enque(q, err);
        }
    }
}

void checkFunctions(char *line, int lineNum, struct func *f, struct queue *q, int *inMain)
{
    char tline[500];
    strcpy(tline, line);
    removespace(tline);
    
    if (strstr(tline, "int main") != NULL || strstr(tline, "void main") != NULL)
    {
        *inMain = 1;
    }
    
    detectFunctionDefinition(tline, lineNum, f, q);
    
    detectFunctionDeclaration(tline, lineNum, f, q);
    
    checkFunctionCalls(tline, lineNum, f, q, *inMain);
}

