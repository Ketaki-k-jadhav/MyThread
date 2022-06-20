#include "myqueue.h"

thread_node *init_thread(thread_node *t,pid_t tid)
{
	t=(thread_node *)malloc(sizeof(thread_node));
	t->tid=tid;
	return t;
}
node *new_node(thread_node *t,pid_t tid)
{
    node *n=(node *)malloc(sizeof(node));
	t=(thread_node *)malloc(sizeof(thread_node));
	t->tid=tid;
    n->t_node=t;
    n->next=NULL;
	printf("new node created,node->tnode->tid=%d\n",n->t_node->tid);
    return n;
}

queue *init_queue()
{
    queue *q=(queue *)malloc(sizeof(queue));
    q->front = q->rear = NULL;
	q->len = 0;
	printf("queue initialised\n");
    return q;
}
   
int enqueue(queue *q, thread_node *t,pid_t tid)
{
    node *n=new_node(t,tid);
    // If queue is empty, then new node is front and rear both 
	if (q->rear == NULL) { 
		q->front = q->rear = n;
		q->len++;
		return 0; 
	}
	// Add the new node at the end of queue and change rear 
	q->rear->next = n; 
	q->rear = n;
	q->len++;
	printf("enqueue new thread with tid=%d\n",n->t_node->tid);
	return 0; 
}

thread_node *dequeue(queue *q)
{
   // If queue is empty, return NULL.
    if (q->front == NULL)
        return NULL;
	
    thread_node *t_node = q->front->t_node;
    q->front = q->front->next;
    q->len = q->len - 1;
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;
 
    return(t_node);
   
}

void print_queue(queue *q)
{
	node *temp;
	if(q->front==NULL)
	{
		printf("Queue is empty\n");
		return;
	}
	temp=q->front;
	while(temp!=NULL)
	{
		printf("tid=%d\t",temp->t_node->tid);
		temp=temp->next;
	}
		
	return;

}


thread_node *get_custom_node(queue* q, pid_t tid)
{
	node *tmp_node;
	thread_node *tn;
	if(q->front==NULL)
	{
		return NULL;
	}
	tmp_node=q->front;
	while(tmp_node!=NULL)
	{
		if(tmp_node->t_node->tid==tid)
		{
			return tmp_node->t_node;
		}
	}
	return NULL;
}

// //testing queue functions

// int main() {
//     thread_node *t1,*t2,*t3,*t4;
//     queue *threads;
//     threads=init_queue();
//     enqueue(threads, t1,23);
// 	 enqueue(threads,t2,24);
//      enqueue(threads,t3,25);
// 	printf("after enqueue\n");
//     print_queue(threads);
//     dequeue(threads);
// 	printf("deque:\n");
//     print_queue(threads);
// 	t4=init_thread(t4,0);
//     t4 = get_custom_node(threads,24);
//     printf("\n\n%d\n",t4->tid);

// }