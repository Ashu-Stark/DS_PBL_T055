#include <stdio.h>
#include <string.h> 
#include "QueueFunc.h" 
#include "semicolon.h" 
#include "queue.h"

void semicolon(char *line, int l, struct queue *q)
{
    int i, len = strlen(line);
    if (len == 0)
        return;
    if (line[len - 1] == '\n')
    {                                  
        line[len - 1] = '\0';
        len--;
    }

    int start = 0;
    while (line[start] == ' ' || line[start] == '\t')
    start++;
    if (line[start] == '#')
    return;
    if (line[start] == '\0')
    return;
    if (strstr(line, "while") && line[start] != 'w')
    {
        if (!strchr(line, '(') || !strchr(line, ')'))
        {
            char err[200];
            sprintf(err, "Invalid do-while syntax at line %d: %s", l, line);
            enque(q, err);
            return;
        }

        int last = strlen(line) - 1;
        if (line[last] != ';')
        {
            char err[200];
            sprintf(err, "Missing ';' in do-while at line %d: %s", l, line);
            enque(q, err);
            return;
        }
        return;
    }

    if (strncmp(&line[start], "while", 5) == 0)
    {
        if (!strchr(line, '(') || !strchr(line, ')'))
        {
            char err[200];
            sprintf(err, "Invalid while-loop syntax at line %d: %s", l, line);
            enque(q, err);
            return;
        }

        char *closing = strrchr( line, ')');
        if (closing && closing[1] == ';')
        {
            char err[200];
            sprintf(err, "Unexpected ';' after while-loop at line %d: %s", l, line);
            enque(q, err);
            return;
        }
        return;
    }

    if (strncmp (&line[start], "for", 3) == 0) 
    {
        int sc = 0;
        for (i = start; line[i] != '\0'; i++)
        {
            if (line[i] == ';')
                sc++;
        }
        if (sc != 2)
        {
            char err[200];
            sprintf(err, "Missing ';' inside for-loop at line %d: %s", l, line);
            enque(q, err);
            return;
        }
        return;
    }

    char temp[300];
    int k = 0;
    for (i = start; line[i] != '\0'; i++)
    {
        if(line[i] == '/' && line[i+1] == '/')
            break;
        temp[k++] = line[i];
    }
    temp[k] = '\0';

    while (k > 0 && (temp[k-1] == ' ' || temp[k-1] == '\t'))
    {
        temp[k-1] = '\0';
        k--;
    }
    if (k == 0)
    return;

    char last = temp[k-1];
    if (last == '{' || last == '}' || last == ':' || last == ')')
    return;
    if (last == ';')
    return;

    char err[200];
    sprintf(err, "Missing ';' at line %d: %s", l, line);
    enque(q, err);
}