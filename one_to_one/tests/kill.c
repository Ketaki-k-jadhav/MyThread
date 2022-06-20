#include"../mythread.h"

void func1(void *arg1)
{
    sleep(2);
    int a,b,c;
    c=a*b;
    return;
}

void mythread_kill_test()
{
    int i;
    thread_node t;
    printf("\n***********Testing kill threads with invalid signal***********\n");
    if (mythread_create(&t, func1, (void *)&i) == 0)
    {
        if((mythread_kill(&t,0)==-1)||(mythread_kill(&t,SIGKILL)==-1))
        {
            printf("Failed: Invalid signal number to thread kill\n");
        }
    }
}

void mythread_kill_test1()
{
    int i;
    thread_node t;
    printf("\n***********Testing kill threads with non existing signal numbers***********\n");
     if (mythread_create(&t, func1, (void *)&i) == 0)
    {
        if(mythread_kill(&t,69)== EINVAL)
        {
            printf("Failed: Invalid signal number to thread kill, Error no = %d\n",EINVAL);
        }
    }

}

void mythread_kill_test2()
{
    thread_node t[5];
    th_list tidlist;
    list_init(&tidlist);
    printf("\n***********Testing thread kill***********\n");
    for(int i=0;i<5;i++)
    {
        if(mythread_create(&t[i],func1,(void *)&i)==0)
        {
            list_insert(&tidlist,t[i].tid);
        }
        else
        {
            printf("Creation of thread failed\n");
        }
    }
    int r = mythread_kill(&t[2],SIGINT);

}

int r = 0;

void* func4(void *args) {
    while(r);
    return (void *)42;
}

void signal_handler_test1()
{
    int i;
    thread_node t;
    printf("\n***********Testing thread kill with user defined signal***********\n");
    if (mythread_create(&t, func1, (void *)&i) == 0)
    {
        mythread_kill(&t,SIGUSR2);
        mythread_join(t,NULL);
    } 

    if(r==0)
    {
        printf("Passed:Signal Handler test case \n");
    }
    else
    {
        printf("Failed:Signal Handler test case \n");
    }
}

int j=0;
void* func5(void *args)
{
    j++;
    return NULL;
}
void signal_handler_test2()
{
    printf("\n***************Testing handling of SIGCONT AND SIGSTOP signals****************\n");
    thread_node t;
    int res=mythread_create(&t,func5,NULL);
    int k1 = mythread_kill(&t, SIGTSTP);
    int k2 = mythread_kill(&t, SIGCONT);
    sleep(1);
    if(res == 0) {
        if(j == 0) {
            printf("FAILED: SIGCONT and SIGSTOP handling test\n");
        }
        else 
        {
            printf("PASSED: SIGCONT and SIGSTOP handling test\n");
        }
    }
}

int main()
{
    mythread_kill_test();
    mythread_kill_test1();
    signal_handler_test1();
    signal_handler_test2();
    mythread_kill_test2();
}