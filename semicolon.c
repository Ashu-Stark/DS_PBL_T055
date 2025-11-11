// // This function checks the required statement ending with or wihtout ";"
// #include<stdio.h>
// #include <string.h>
// #include "QueueFunc.h"
// #include "semicolon.h"
// #include"queue.h"

// void semicolon(char *line,int l,struct queue *q)
// {
//     int llen = strlen(line);
//         if (line[llen - 1] == '\n')
//             line[--llen] = '\0';
//         if (!(line[llen - 1] == '{' || line[llen - 1] == ':' || line[llen - 1] == ','|| line[llen - 1] == '}' || line[llen - 1] == ')'|| line[0]=='#'))
//         {
//             if (line[llen - 1] != ';' )
//             {
//                 char err[200];
//                 sprintf(err, "Missing ';' at line %d: %s\n", l, line);
//                 enque(q, err);
//             }
//         }
// }



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
    if (line[len - 1] == '\n')                              // remove newline
    {                                  
        line[len - 1] = '\0';
        len--;
    }

    int start = 0;                                          // skip leading spaces
    while (line[start] == ' ' || line[start] == '\t')
    start++;
    if (line[start] == '#')                                // ignore preprocessor
    return;
    if (line[start] == '\0')                                // blank line
    return;
    if (strstr(line, "while") && line[start] != 'w')       //for do while check
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
        return;                                          // valid condition of do-while
    }

    if (strncmp(&line[start], "while", 5) == 0)          // for while check
    {
        if (!strchr(line, '(') || !strchr(line, ')'))    // must have '(' and ')'
        {
            char err[200];
            sprintf(err, "Invalid while-loop syntax at line %d: %s", l, line);
            enque(q, err);
            return;
        }

        char *closing = strrchr( line, ')');              // detect wrong while(condition);
        if (closing && closing[1] == ';')
        {
            char err[200];
            sprintf(err, "Unexpected ';' after while-loop at line %d: %s", l, line);
            enque(q, err);
            return;
        }
        return;                                           // valid while()
    }

    if (strncmp (&line[start], "for", 3) == 0)             // for loop check 
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
        return;                                      // valid for loop
    }

    char temp[300];                                  // remove comments
    int k = 0;
    for (i = start; line[i] != '\0'; i++)
    {
        if(line[i] == '/' && line[i+1] == '/')
            break;
        temp[k++] = line[i];
    }
    temp[k] = '\0';

    while (k > 0 && (temp[k-1] == ' ' || temp[k-1] == '\t'))     // trim trailing spaces
    {
        temp[k-1] = '\0';
        k--;
    }
    if (k == 0)
    return;

    char last = temp[k-1];
    if (last == '{' || last == '}' || last == ':' || last == ')') //no need for ;
    return;
    if (last == ';')                                             // already has a semicolon
    return;

    char err[200];                                               //missing semicolon
    sprintf(err, "Missing ';' at line %d: %s", l, line);
    enque(q, err);
}