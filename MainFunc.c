//Here is the main functions used in Program.
#include<stdio.h>
#include <string.h>
#include <ctype.h>
#include "MainFunc.h"
#include"variablestore.h"
#include"queue.h"

#include"QueueFunc.h"

int checknum(char str)
{
    if (str >= 'a' && str <= 'z' || str >= 'A' && str <= 'Z' || str >= '0' && str <= '9')
    {
        return 1;
    }
    return 0;
}

int checkdigit(char str)
{
    if (str >= '0' && str <= '9')
    {
        return 1;
    }
    return 0;
}

// ---------------- IDENTIFIER / KEYWORD CHECK ----------------
int identifier(char *token)
{
    if (!((token[0] >= 'a' && token[0] <= 'z') ||
          (token[0] >= 'A' && token[0] <= 'Z') ||
          (token[0] == '_')))
        return 0;
    for (int i = 1; token[i] != '\0'; i++)
    {
        if (!((token[i] >= 'a' && token[i] <= 'z') ||
              (token[i] >= 'A' && token[i] <= 'Z') ||
              (token[i] == '_') ||
              (token[i] >= '0' && token[i] <= '9')))
            return 0;
    }
    return 1;
}

int keyword(char *name, int keywordcount, char *keywords[])
{
    for (int i = 0; i < keywordcount; i++)
    {
        if (strcmp(keywords[i], name) == 0)
            return 1;
    }
    return 0;
}

int datatype(char *tline)
{
    if (strncmp(tline, "int ", 4) == 0 || strncmp(tline, "float ", 6) == 0 ||
        strncmp(tline, "char ", 5) == 0 || strncmp(tline, "double ", 7) == 0)
        return 1;
    return 0;
}

// ---------------- REMOVE EXTRA SPACES ----------------
// void removespace(char *str)
// {
//     int i = 0, j = 0;
//     int flag2 = 0;
//     while (str[i] == ' ' || str[i] == '\t')
//         i++;
//     for (; str[i] != '\0'; i++)
//     {
//         if (str[i] == ' ' || str[i] == '\t')
//         {
//             if (!flag2)
//             {
//                 str[j++] = ' ';
//                 flag2 = 1;
//             }
//         }
//         else if (str[i] == '\n')
//         {
//             continue;
//         }
//         else
//         {
//             str[j++] = str[i];
//             flag2 = 0;
//         }
//     }
//     if (j > 0 && str[j - 1] == ' ')
//         j--;
//     str[j] = '\0';
// }

void removespace(char *str)
{
    int i = 0, j = 0;
    int flag2 = 0; // for space control
    int newline_flag = 0; // to detect blank lines

    // Skip leading spaces/tabs/newlines
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
        i++;

    for (; str[i] != '\0'; i++)
    {
        // Handle spaces and tabs
        if (str[i] == ' ' || str[i] == '\t')
        {
            if (!flag2)
            {
                str[j++] = ' ';
                flag2 = 1;
            }
        }
        else if (str[i] == '\n')
        {
            // Check if upcoming line is blank (only spaces/tabs/newlines)
            int k = i + 1;
            while (str[k] == ' ' || str[k] == '\t' || str[k] == '\n')
                k++;

            // If next non-space is '\0', stop (end of string)
            if (str[k] == '\0')
                break;

            // If next line is NOT blank, keep one newline
            if (str[k] != '\n' && str[k] != '\0')
                str[j++] = '\n';

            flag2 = 0;
            newline_flag = 0;
            i = k - 1; // skip the blank lines
        }
        else
        {
            str[j++] = str[i];
            flag2 = 0;
            newline_flag = 0;
        }
    }

    // Remove trailing space if any
    if (j > 0 && (str[j - 1] == ' ' || str[j - 1] == '\n'))
        j--;

    str[j] = '\0';
}


// ---------------- REMOVE COMMENTS ----------------
void removeComments(char *line)
{
    int i = 0, j = 0;
    int flag1 = 0;
    while (line[i])
    {
        if (line[i] == '"' && (i == 0 || line[i - 1] != '\\'))
        {
            flag1 = !flag1;
        }

        if (!flag1 && line[i] == '/' && line[i + 1] == '/')
        {
            break; // skip rest of line (single-line comment)
        }
        else if (!flag1 && line[i] == '/' && line[i + 1] == '*')
        {
            i += 2;
            while (line[i] && !(line[i] == '*' && line[i + 1] == '/'))
                i++;
            if (line[i])
                i += 2;
            continue;
        }
        line[j++] = line[i++];
    }
    line[j] = '\0';
}


int addvar(char *type, char *name, int line, struct queue *q)
{
    // Check redeclaration
    for (int i = 0; i < varCount; i++)
    {
        if (strcmp(varList[i].name, name) == 0)
        {
            char err[200];
            sprintf(err, "Redeclaration error: %s at line %d\n", name, line);
            enque(q, err);
            return 0;
        }
    }

    // Add new variable
    strcpy(varList[varCount].name, name);
    strcpy(varList[varCount].type, type);
    varList[varCount].line = line;
    varCount++;
    return 1;
}

int present(char *name)
{
    for (int i = 0; i < varCount; i++)
    {
        if (strcmp(varList[i].name, name) == 0)
            return 1;
    }
    return 0;
}

int token_func(char *line,char ftoken[][50])
{
    int i,j=0,count=0;
    for(i=0;line[i]!='\0';i++)
    {
        char ch=line[i];
        if(isspace(ch) || ch=='{' || ch=='}'||ch==';'||ch=='('||ch==')'||ch==',')
        {
            if(j>0)
            {
                ftoken[count++][j]='\0';
                j=0;
            }

            if(ch=='{' || ch=='}'||ch==';'||ch=='('||ch==')'||ch==',')
            {
                ftoken[count][0]=ch;
                ftoken[count][1]='\0';
                count++;
            }
        }
        else{
            ftoken[count][j++]=ch;
        }
    }
    if(j>0)
    {
        ftoken[count++][j]='\0';
        j=0;
    }

    return count;
}