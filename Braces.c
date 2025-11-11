//This Function checks theb braces opening , closing , missmatching.
#include<stdio.h>
#include"stack.h"
#include "Braces.h"
#include"queue.h"
#include"QueueFunc.h"
#include"stackvar.h"
#include"StackFunc.h"


void braces(char *line,int l,struct queue*q,struct stack *bracestack){
    char err[200],c;
    for (int i = 0; line[i] != '\0'; i++)
        {
            c = line[i];
            if (c == '{' || c == '(' || c == '[')
                push(bracestack, c, l);
            else if (c == '}' || c == ')' || c == ']')
            {
                if (!isEmpty(bracestack))
                {
                    struct snode topnode = top(bracestack);
                    if ((topnode.ch == '{' && c == '}') || (topnode.ch == '[' && c == ']') || (topnode.ch == '(' && c == ')'))
                        pop(bracestack);
                    else
                    {
                        sprintf(err, "Mismatch: expected closing for '%c' from line %d but found '%c' in line %d\n", topnode.ch, topnode.line, c, l);
                        enque(q, err);
                    }
                }
                else
                {
                    sprintf(err, "Extra closing brace '%c' at line %d\n", c, l);
                    enque(q, err);
                }
            }
        }
}
//check the remaining stack is empty for extra braces.
void checkremain(struct stack *bracestack,struct queue *q)
{
    char err[200];
    while (!isEmpty(bracestack))
    {
        struct snode node = pop(bracestack);
        sprintf(err, "Missing closing brace for '%c' opened at line %d\n", node.ch, node.line);
        enque(q, err);
    }
} 