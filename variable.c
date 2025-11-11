//This function is check the variables,keywords,datatypes etc.
#include <stdio.h>
#include <string.h>
#include "variable.h"
#include "queue.h"
#include"MainFunc.h"


void variable(char *line, int l, struct queue *q, int keywordcount, char *keywords[])
{
    char tline[500];
    char err[200];
    strcpy(tline, line);
    removespace(tline);
    
    if (datatype(tline))   
    {
        // Check if this is a function definition/declaration (has '(' after identifier)
        char *openParen = strchr(tline, '(');
        if (openParen != NULL)
        {
            // This is a function definition/declaration
            // Extract function parameters and add them as variables
            // Format: "datatype funcName(type1 param1, type2 param2)"
            
            // Find the opening parenthesis
            char *paramStart = openParen + 1;
            char *paramEnd = strchr(paramStart, ')');
            
            if (paramEnd != NULL && paramEnd > paramStart)
            {
                // Extract parameter section
                int paramLen = paramEnd - paramStart;
                char paramSection[500];
                strncpy(paramSection, paramStart, paramLen);
                paramSection[paramLen] = '\0';
                
                // Parse parameters: "float a, float b" or "int x"
                char *paramToken = strtok(paramSection, ",");
                while (paramToken != NULL)
                {
                    // Remove leading/trailing spaces
                    while (*paramToken == ' ' || *paramToken == '\t')
                        paramToken++;
                    
                    // Extract type and parameter name
                    char paramType[50];
                    char paramName[50];
                    int typeIdx = 0, nameIdx = 0;
                    int foundSpace = 0;
                    
                    for (int i = 0; paramToken[i] != '\0'; i++)
                    {
                        if (paramToken[i] == ' ' || paramToken[i] == '\t')
                        {
                            if (typeIdx > 0)
                                foundSpace = 1;
                            continue;
                        }
                        
                        if (!foundSpace)
                        {
                            if (typeIdx < 49)
                                paramType[typeIdx++] = paramToken[i];
                        }
                        else
                        {
                            if (nameIdx < 49)
                                paramName[nameIdx++] = paramToken[i];
                        }
                    }
                    
                    paramType[typeIdx] = '\0';
                    paramName[nameIdx] = '\0';
                    
                    // Add parameter as variable if valid
                    // Note: Skip if already exists (allows same parameter names in different functions)
                    if (strlen(paramType) > 0 && strlen(paramName) > 0 && identifier(paramName))
                    {
                        // Check if variable already exists - if so, skip (function-scoped parameters)
                        if (!present(paramName))
                        {
                            addvar(paramType, paramName, l, q);
                        }
                        // If it exists, it's likely from another function, which is okay
                    }
                    
                    paramToken = strtok(NULL, ",");
                }
            }
            // Function parameters handled, return (don't check function body variables here)
            return;
        }
        
        // Regular variable declaration (not a function)
        char *token = strtok(tline, " ,;{}()/*-+=");
        char type[20];
        strcpy(type, token);

        while ((token = strtok(NULL, " ,;{}()/*-+=")) != NULL)
        {
            if (strlen(token) > 0 && identifier(token))
            {
                addvar(type, token, l, q);
            }
        }
    }
    else
    {
        int len = strlen(tline);
        char token[50];
        int idx = 0;
        int flag1 = 0;

        for (int i = 0; i <= len; i++)
        {
            if (tline[i] == '"' && (i == 0 || tline[i - 1] != '\\'))
            {
                flag1 = !flag1;
            }
            if ((checknum(tline[i]) || tline[i] == '_') && !flag1)
                token[idx++] = tline[i];
            else if (idx > 0)
            {
                token[idx] = '\0';
                idx = 0;

                // Check if this token is followed by '(' - if so, it's a function call, skip it
                // Function calls are handled by function1.c, not variable checker
                int isFunctionCall = 0;
                int j = i;
                while (j < len && (tline[j] == ' ' || tline[j] == '\t'))
                    j++;
                if (j < len && tline[j] == '(')
                {
                    isFunctionCall = 1;
                }

                if (!isFunctionCall && !keyword(token, keywordcount, keywords))
                {
                    int isNumber = 1;
                    for (int j = 0; token[j]; j++)
                    {
                        if (!checkdigit(token[j]))
                        {
                            isNumber = 0;
                            break;
                        }
                    }
                    if (!isNumber)
                    {
                        if (!identifier(token))
                        {
                            sprintf(err, "Invalid identifier: %s at line %d\n", token, l);
                            enque(q, err);
                        }
                        else if (!present(token))
                        {
                            sprintf(err, "Undeclared variable: %s at line %d\n", token, l);
                            enque(q, err);
                        }
                    }
                }
            }
        }
    }
}