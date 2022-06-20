#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include<stdlib.h>
#include<limits.h>
#include "mythread.h"

typedef struct Queue queue;
typedef struct Node node;

struct Node{
    thread_node *t_node;
    struct Node *next;
};

struct Queue{
    node *front;
    node *rear;
    int len;
};

thread_node *init_thread(thread_node *t,pid_t tid);
node *new_node(thread_node *t,pid_t tid);
queue * init_queue();
int enqueue(queue *q, thread_node *t,pid_t tid);
thread_node *dequeue(queue *q);
void print_queue(queue *q);
thread_node *get_custom_node(queue* q, pid_t tid);

#endif