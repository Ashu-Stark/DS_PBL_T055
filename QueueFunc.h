#ifndef QUEUEUFUNC_H
#define QUEUEUFUNC_H

struct queue *create();

void enque(struct queue * , char *);

void print(struct queue*);


// void enque(struct queue *q, char *error);

#endif