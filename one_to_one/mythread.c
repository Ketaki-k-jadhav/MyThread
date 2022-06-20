#include "mythread.h"


th_list tidlist;

spinlock global_lock;

static int SYS_KILL(int tid, int signal)
{
    return syscall(SYS_tgkill,getpid(),tid,signal);
}

static int SYS_FUTEX(void *addr)
{
    return syscall(SYS_futex, addr, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
}

void cleanup()
{
    delete_all_threads(&tidlist);
}

static void init_threads()
{
    spinlock_init(&global_lock);
    list_init(&tidlist);
    thread_node *new_node = list_insert(&tidlist,getpid());
    new_node->function = NULL;
    new_node->ret_val=NULL;
    new_node->stack=NULL;
    new_node->arg=NULL;
    atexit(cleanup);
    return;
}

static void *allocate_stack(size_t stack_size, size_t guard_size)
{
    void *stack = NULL;
    stack = mmap(NULL, stack_size + guard_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED)
    {
        perror("Error in Stack Allocation");
        return NULL;
    }
    if (mprotect(stack, guard_size, PROT_NONE))
    {
        munmap(stack, stack_size + guard_size);
        perror("Error in Stack Allocation");
        return NULL;
    }
    return stack;
}

static int start_routine(void *fa)
{
    thread_node * temp;
    temp=(thread_node *)fa;
    temp->ret_val=temp->function(temp->arg);
    mythread_exit(NULL);
}

int mythread_create(thread_node *t, void *start_routine, void *arg,...)
{
    spinlock_lock(&global_lock);
    static int init_state = 0;

    if (init_state == 0)
    {
        init_state = 1;
        init_threads();
    }
    thread_node *new_node = list_insert(&tidlist,0);
    if (new_node == NULL)
    {
        printf("Thread not found\n");
        spinlock_unlock(&global_lock);

        return -1;
    }
    t->function=(void *)malloc(sizeof(t->function));
    t->arg=(void *)malloc(sizeof(t->arg));
    t->ret_val=(void *)malloc(sizeof(t->ret_val));
    t->stack=(void *)malloc(sizeof(t->stack));
    t->function = start_routine;
    t->arg = arg;
        t->stack = allocate_stack(STACK_SIZE, GUARD_SIZE);
        if (!t->stack)
        {
            perror("thread stack allocation error\n");
            spinlock_unlock(&global_lock);
            return errno;
        }
        t->tid = clone(start_routine,
                    t->stack + STACK_SIZE + GUARD_SIZE,
                    CLONE_FLAGS,
                    t,
                    &(new_node->tid),
                    NULL,
                    &(new_node->tid));
            
        new_node->arg = t->arg;
        new_node->function=t->function;
        new_node->stack=t->stack;

    if (t->tid == -1)
    {
        perror("thread create\n");
        spinlock_unlock(&global_lock);
        free(t->stack);
        return errno;
    }
    spinlock_unlock(&global_lock);
    return 0;
}

int mythread_join(thread_node t, void **ret_val)
{
    int result;
    spinlock_lock(&global_lock);
    void *addr=return_custom_node(&tidlist,t.tid);
    if (addr == NULL)
    {
        spinlock_unlock(&global_lock);
        return ESRCH;
    }
    if (*((pid_t *)addr) == 0)
    {
        spinlock_unlock(&global_lock);
        return EINVAL;
    }
    while (*((pid_t *)addr) == t.tid)
    {
        spinlock_unlock(&global_lock);
        result = syscall(SYS_futex, addr, FUTEX_WAIT, t.tid, NULL, NULL, 0);
        spinlock_lock(&global_lock);
    }
    syscall(SYS_futex, addr, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
    if (ret_val)
    {
        thread_node *new_node = return_custom_thread_node(&tidlist, t.tid);
        *ret_val = new_node->ret_val;
    }
    spinlock_unlock(&global_lock);
    return result;
}

void mythread_exit(void *retval)
{
    spinlock_lock(&global_lock);
    void* addr;
    addr=(void *)return_custom_thread_node(&tidlist, gettid());
    if (addr == NULL)
    {
        spinlock_unlock(&global_lock);
        return;
    }
    if (retval)
    {
        thread_node *node = return_custom_thread_node(&tidlist, gettid());
        node->ret_val = retval;
        printf("retval=%p\n",retval);
    }
    
    spinlock_unlock(&global_lock);
    // kill(gettid(),SIGINT);
   exit(0);

    return;
}

int mythread_kill(thread_node* t, int signum)
{
    int result;
    if(t == NULL){
        return -1;
    }
    if (signum==0)
    {
        return -1;
    }
    if(signum < 0 || signum > 64) 
    {
        return EINVAL;
    }

    if (signum == SIGSTOP|| signum == SIGINT || signum == SIGCONT)
    {   printf("\nThread kill1 successful\n");
        result =syscall(SYS_tgkill,getpid(),t->tid,signum);
    
        if(result == -1)
        {
            perror("thread kill error\n");
            return errno;
        }
    }
    return result;
}