#include "mythread.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>

int list_init(th_list *tl)
{
    if (!tl)
        return -1;
    tl->head = tl->tail = NULL;
    return 0;
}

thread_node *list_insert(th_list *tl,pid_t tid)
{
    thread_node *tn;
    tn=(thread_node *)malloc(sizeof(thread_node));
    if(tn==NULL)
    {
        perror("Linked list node can't be allocated\n");
        return NULL;
    }
    tn->tid = tid;
    tn->next = NULL;
    tn->ret_val = NULL;
    if (tl->head == NULL)
    {
        tl->head = tl->tail = tn;
    }
    else
    {
        tl->tail->next = tn;
        tl->tail = tn;
    }
    return tn;
}

int list_delete(th_list * tl, pid_t tid)
{
    thread_node *tn = tl->head;
    if (tn == NULL)
    {
        return -1;
    }
    if (tn->tid == tid)
    {
        tl->head = tl->head->next;
        free(tn);
        if (tl->head == NULL)
        {
            tl->tail = NULL;
        }
        return 0;
    }
    while (tn->next)
    {
        if (tn->next->tid == tid)
        {
            thread_node *tn_next = tn->next->next;
            if (tn->next == tl->tail)
            {
                tl->tail = tn;
            }
            
            free(tn->next);
            tn->next = tn_next;
            break;
        }
        tn = tn->next;
    }
    return 0;
}

pid_t *return_custom_node(th_list*tl, pid_t tid)
{
    thread_node *tn = tl->head;
    while (tn != NULL)
    {
        if (tn->tid == tid)
        {
            return &(tn->tid);
        }
        tn = tn->next;
    }
    return NULL;
}

thread_node *return_custom_thread_node(th_list *tl,  pid_t tid)
{
    thread_node *tn = tl->head;
    while (tn!= NULL)
    {
        if (tn->tid == tid)
        {
            return tn;
        }
        tn = tn->next;
    }
    return NULL;
}

void print_all_thread_nodes(th_list *tl)
{
    thread_node *tn = tl->head;
    while (tn)
    {
        printf("tid%d-->", tn->tid);
        fflush(stdout);
        
        tn = tn->next;
    }
    printf("\n");
    return;
}

int delete_all_threads(th_list *tl)
{
    thread_node *tn = tl->head;
    if(tn==NULL)
    {
        printf("already NULL");

    }
    if(tl->head==tl->tail){
    free(tl->head);
    tl->head=NULL;
    return 0;
    }
    while(tn!=NULL)
    {
       thread_node* temp=tn->next;
        free(tn); 
        tn=tn->next;
    }
    tl->head=NULL;
    return 0;
}