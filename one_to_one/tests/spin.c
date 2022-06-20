#include"../mythread.h"
#include <stdio.h>

spinlock lock;
long c = 0, c1 = 0, c2 = 0, run = 1;
void *thread1(void *arg) {
	while(run == 1) {
		spinlock_lock(&lock);
		c++;
		spinlock_unlock(&lock);
		c1++;
	}
}
void *thread2(void *arg) {
	while(run == 1) {
		spinlock_lock(&lock);
		c++;
		spinlock_unlock(&lock);
		c2++;
	}
}
int main() {
	printf("***********Testing Synchronization using locks**************\n");
	thread_node th1, th2;
	spinlock_init(&lock); 
	mythread_create(&th1, thread1, NULL);
	mythread_create(&th2, thread2, NULL);
	sleep(2);
	run = 0;
	mythread_join(th1, NULL);
	mythread_join(th2, NULL);
	fprintf(stdout, "c = %ld c1+c2 = %ld c1 = %ld c2 = %ld \n", c, c1+c2, c1, c2);
	if(c==c1+c2)
	{
		printf("PASSED:Race test using locks\n");
	}
	else
	{
		printf("Failed:Race test using locks\n");
	}

	fflush(stdout);
}