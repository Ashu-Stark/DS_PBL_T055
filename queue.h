#ifndef QUEUE_H
#define QUEUE_H

struct node {
    char error[200];
    struct node *next;
}node;

struct queue {
    struct node *front,*rear;
};

#endif