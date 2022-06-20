#include"../mythread.h"

void func1(void *arg1)
{
    sleep(2);
    int a,b,c;
    c=a*b;
    return;
}

void func2(void *arg)
{
    int x=2,y=3,z;
    z=x+y;
    return;
}


void mythread_join_test1()
{
    thread_node t[5];
    int i;
    printf("\n***********Testing Create & Join Threads***********\n");
    for(int i=0;i<5;i++)
    {
        if(mythread_create(&t[i],func1,(void *)&i)==0)
        {
            printf("Successfully created a thread with tid=%d\n",t[i].tid);
        }
        else
        {
            printf("Creation of thread failed\n");
        }
    }
    for(int i=0;i<5;i++)
    {
        mythread_join(t[i],NULL);
        printf("Thread Joining Successful\n");
    }

}

void mythread_join_test2()
{
    thread_node t;
    int i,j1;
    printf("\n***********Testing joining already joined thread***********\n");
    if(mythread_create(&t,func2,(void *)&i)==0)
        {
            j1=mythread_join(t,NULL);
            j1=mythread_join(t,NULL);
            if(j1!=0)
            {
                printf("Failed: Joining already joined threads\n");
            }
        }
        
}

void mythread_join_test3()
{
    thread_node t,s;
    int i,j1;
    printf("\n***********Testing joining invalid thread***********\n");
    if(mythread_create(&t,func2,(void *)&i)==0)
        {
            j1=mythread_join(s,NULL);
            if(j1!=0)
            {
                printf("Failed: Joining invalid thread\n");
            }
        } 
}

int main()
{
    mythread_join_test1();
    mythread_join_test2();
    mythread_join_test3();
}