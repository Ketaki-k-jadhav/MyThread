#include"../mythread.h"

static int k = 0;
void func1(void *arg1)
{
    sleep(2);
    int a,b,c;
    c=a*b;
    return;
}

void mythread_create_test1()
{
    thread_node t[5];
    th_list tidlist;
    list_init(&tidlist);
    printf("\n***********Testing thread creation***********\n");
    for(int i=0;i<5;i++)
    {
        if(mythread_create(&t[i],func1,(void *)&i)==0)
        {
            printf("\nSuccessfully created a thread with tid = %d\n",t[i].tid);
            list_insert(&tidlist,t[i].tid);
        }
        else
        {
            printf("Creation of thread failed\n");
        }
    }
}

void mythread_create_test2()
{
    thread_node t;
    int i,j;
    printf("\n***********Testing thread creation with multiple arguments***********\n");
    if(mythread_create(&t,func1,(void *)&i,(void *)&j)==0)
    {
        printf("Creation of thread successful : Multiple arguments to start routine function\n");
    }
}

int main()
{
    mythread_create_test1();
    mythread_create_test2();
}