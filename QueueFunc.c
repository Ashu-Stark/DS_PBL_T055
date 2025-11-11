#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "QueueFunc.h"
#include"queue.h"

struct queue* create()
{
    struct queue *q = (struct queue*)malloc(sizeof(struct queue));
    q->front=q->rear=NULL;
    return q;
}

void enque(struct queue*q,char *str)
{
    struct node *temp=(struct node*)malloc(sizeof(struct node));
    strcpy(temp->error,str);
    temp->next=NULL;
    if(q->rear==NULL){
        q->front=q->rear=temp;
        return ;
    }
    q->rear->next=temp;
    q->rear=temp;
}

void print(struct queue*q)
{
    int i=1;
    if(q->front==NULL)
    {
        printf("No error\n");
        return ;
    }
    struct node *temp=q->front;
    while(temp!=NULL)
    {
        printf("%d. =>  %s\n",i,temp->error);
        i++;
        temp=temp->next;
    }
}