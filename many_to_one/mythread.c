#include"mythread.h"
#include"myqueue.h"


void enable_timer(struct itimerval *timer) 
{
    timer->it_value.tv_sec = 0;
    timer->it_value.tv_usec = alarm;
    timer->it_interval.tv_sec = 0;
    timer->it_interval.tv_usec = alarm;
    setitimer(ITIMER_VIRTUAL, timer, 0);
}

void disable_timer(struct itimerval *timer) 
{
    timer->it_value.tv_sec = 0;
    timer->it_value.tv_usec = 0;
    timer->it_interval.tv_sec = 0;
    timer->it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, timer, 0);
}

void start_routine() 
{
    thread_node *t;
    t->ret_val = t->function(t->arg);
    t->status = TERMINATED;
    raise(SIGVTALRM);
    return;
}

void cleanup() 
{
    thread_node *t;
    queue *q;
    int len = q->len;
    for(int i = 0; i < len; i++)
    {
        t = dequeue(q);
        free(t);
    }
    free(q);
}

static long int mangle(long int p) 
{
    long int ret;
    asm(" xor %%fs:0x30, %0\n;"
        " rol $0x11, %0\n;"
        : "=r"(ret)
        : "0"(p)
    );
    return ret;
}

void scheduler(int sig)
{
    disable_timer(&timer);
    queue *q;
    thread_node *t;
    pid_t temp_tid = t->tid;
    if(sigsetjump(t->context, 1) == 1)
    {
        return;
    }
    enqueue(q,t,t->tid);

    if(t->status == RUNNING)
    {
        t->status = READY;
    }

    thread_node *new;
    for(int i = 0; i < q->len; i++)
    {
        new = dequeue(q);
        if(new->status == READY)
        {
            t = new;
            t->status = RUNNING;
            break;
        }
        else if(new->status == TERMINATED) 
        {
            enqueue(q, new, new->tid);
        }
    }

    if(t->tid == temp_tid) 
    {
        exit(0);
    }
    
    if(t->signal != -1) 
    {
        raise(t->signal);
        t->signal = -1;
    }

    enable_timer(&timer);
    setContext(new->context);

}

void setContext(sigjmp_buf *context)
{
    siglongjmp(*context, 1);
}

static void init_threads() 
{
    queue *q;
    thread_node *t;
    q = init_queue();
    pid_t tid = 0;
    
    t->tid = tid++;
    t->ret_val = NULL;
    t->function = NULL;
    t->arg = NULL;
    t->signal = -1;
    t->state = JOINABLE;
    t->status = RUNNING;
    sigsetjmp(t->context, 1);

    struct sigaction sa_usr;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGVTALRM);
    sa_usr.sa_handler = &scheduler;
    sa_usr.sa_flags = 0;
    sa_usr.sa_mask = mask;
    if(sigaction (SIGVTALRM, &sa_usr, NULL) == -1)
    {
        printf("Error in sigaction");
    }
    atexit(cleanup);
    start_timer(&timer);

}

static void *allocStack(size_t stack_size, size_t guard_size)
{
    void *stack = NULL;
    stack = mmap(NULL,stack_size + guard_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK,-1, 0);

    if(stack == MAP_FAILED) 
    {
        perror("Stack allocation error");
        return NULL;
    }
    return stack;
}

void createContext(sigjmp_buf *context, void *routine, void *stack)
{
    sigsetjmp(*context, 1);
    context[0]->__jmpbuf[6] = mangle((long int)(STACK_SIZE - sizeof(long int)));
	context[0]->__jmpbuf[6] = context[0]->__jmpbuf[1];
    if(start_routine)
    {
        context[0]->__jmpbuf[7] = mangle((long int) start_routine);
    }
}

int mythread_create(thread_node *th, void *start_routine, void *arg,...)
{
    queue *q;
    thread_node *t;
    t = (thread_node *) malloc(sizeof(thread_node));
    pid_t tid = 0;

    t->tid = tid++;
	t->arg = arg;
    t->status = READY;
	t->function = start_routine;
    t->signal = -1;
    t->state = JOINABLE;

    sigjmp_buf *context = (sigjmp_buf *)malloc(sizeof(sigjmp_buf));

    t->stack = allocStack(t->stack_size, 0);

    th->tid = t->tid;

    createContext(context, start_routine, t->stack);

    enqueue(q, t, t->tid);

    return 0;
}

int mythread_join(thread_node thread, void **retval) 
{
    disable_timer(&timer);
    queue *q;
    thread_node *t;
    thread_node *temp;
    temp = get_custom_node(q,thread.tid);
    if(temp == NULL) 
    {
        if(retval)
            *retval = (void *)ESRCH;
        enable_timer(&timer);
        return ESRCH;
    }

    if(temp->state == JOINED) 
    {
        if(retval)
            *retval = (void *)EINVAL;
        enable_timer(&timer);
        return EINVAL;
    }
    temp->state = JOINED;

    while(1) 
    {
        if (temp->status == TERMINATED) {
            break;
        }
    }

    if(retval) 
    {
        *retval = temp->ret_val;
    }
    return 0;
}

void mythread_exit(void *retval) 
{
    thread_node *t;
    t->ret_val = retval;
    t->status = TERMINATED;
    raise(SIGVTALRM);
    
}

int mythread_kill(thread_node *thread, int signum) 
{
    queue *q;
    thread_node *t;
    int status = -1;
    if(signum == 0) 
    {
        return 0;
    }

    if(signum < 0 || signum > 64) 
    {
        return EINVAL;
    }

    if (signum == SIGINT || signum == SIGCONT || signum == SIGSTOP)
    {
        kill(getpid(), signum);
    }
    else
    {
        if(thread->tid == t->tid) 
        {
            status = raise(signum);
        }
        else 
        {
            thread_node *temp = get_custom_node(q, thread->tid);
            if(temp != NULL) 
            {
                temp->signal = signum;
                status = 0;
            }
            else 
            {
                status = ESRCH;
            }
        }
    }
    return status;
}




