#ifndef MYTHREAD_H
#define MYTHREAD_H
#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/futex.h>
#include <linux/unistd.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <limits.h>
#include <stdlib.h>
#include<unistd.h>
#include<signal.h>
#include"locks.h"

#define STACK_SIZE (1024*64)
#define GUARD_SIZE getpagesize()
#define CLONE_FLAGS CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD |CLONE_SYSVSEM|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID
#define gettid() syscall(SYS_gettid)

typedef struct Thread thread_node;
struct Thread{
    pid_t tid;
    void *(*function)(void *);
    void *ret_val;
    void *arg;
    void *stack;
    struct Thread *next;
};

typedef struct Thread_list th_list;
struct Thread_list{
    struct Thread *head;
    struct Thread *tail;
};

int list_init(th_list *tl);
thread_node *list_insert(th_list *tl, pid_t tid);
int list_delete(th_list *,  pid_t tid);
pid_t *return_custom_node(th_list*tl, pid_t tid);
void print_all_thread_nodes(th_list *tl);
thread_node *return_custom_thread_node(th_list *tl,  pid_t tid);
int delete_all_threads(th_list *tl);
void cleanup();
static void init_threads();
static void *allocate_stack(size_t stack_size, size_t guard_size);
static int start_routine(void *fa);
int mythread_create(thread_node *t,void *start_routine, void *arg,...);
int mythread_join(thread_node t, void **ret_val);
void mythread_exit(void *retval);
int mythread_kill(thread_node* t, int signum);

#endif