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
        char *openParen = strchr(tline, '(');
        if (openParen != NULL)
        {
            char *paramStart = openParen + 1;
            char *paramEnd = strchr(paramStart, ')');
            
            if (paramEnd != NULL && paramEnd > paramStart)
            {
                int paramLen = paramEnd - paramStart;
                char paramSection[500];
                strncpy(paramSection, paramStart, paramLen);
                paramSection[paramLen] = '\0';
                
                char *paramToken = strtok(paramSection, ",");
                while (paramToken != NULL)
                {
                    while (*paramToken == ' ' || *paramToken == '\t')
                        paramToken++;
                    
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
                    
                    if (strlen(paramType) > 0 && strlen(paramName) > 0 && identifier(paramName))
                    {
                        if (!present(paramName))
                        {
                            addvar(paramType, paramName, l, q);
                        }
                    }
                    
                    paramToken = strtok(NULL, ",");
                }
            }
            return;
        }
        
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