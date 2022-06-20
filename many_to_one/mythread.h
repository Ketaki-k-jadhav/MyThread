#ifndef MYTHREAD_H
#define MYTHREAD_H
#define _GNU_SOURCE
#include<stdio.h>
#include<assert.h>
#include<sys/time.h>
#include<errno.h>
#include<string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<setjmp.h>
#include<sys/syscall.h>
#include"locks.h"

#define STACK_SIZE 65536

#define GUARD_SIZE getpagesize()

#define alarm 1000

#define gettid() syscall(SYS_gettid)

struct itimerval timer; 

typedef struct Thread thread_node;


struct Thread{
    pid_t tid;
    void *ret_val;
    void *stack;
    size_t stack_size;
    sigjmp_buf *context;
    void *(*function)(void *);
    void *arg;
    int signal;
    int state;
    int status;
};

typedef enum state
{
    JOINABLE,
    JOINED

}state;

typedef enum status
{
    READY,
    RUNNING,
    TERMINATED

}status;

void enable_timer(struct itimerval *timer);
void disable_timer(struct itimerval *timer);

void start_routine();
void cleanup();
static long int mangle(long int p);
void scheduler(int sig);
void setContext(sigjmp_buf *context);
static void *allocStack(size_t stack_size, size_t guard_size);
void createContext(sigjmp_buf *context, void *routine, void *stack)

static void init_threads();
int mythread_create(thread_node *t, void *start_routine, void *arg,...);
int mythread_join(thread_node t, void **ret_val);
void mythread_exit(void *retval);
int mythread_kill(thread_node *t, int signum);



#endif