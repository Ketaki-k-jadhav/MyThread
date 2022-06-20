#include "../mythread.h"
#include <stdio.h>

#define PRESENT 1
#define ABSENT 0
#define MAX 16
#define NThreads 5
int arr[] = { 1, 6, 8, 11, 13, 14, 15, 19, 21, 23, 26, 28, 31, 65, 108, 220 };

int key = 26;
int is_found = 0;
int current_thread= 0;

int expected_result = PRESENT; 

void* linear_search(void *arg) {
  
  int num = current_thread++;
   for (int i = num * (MAX / NThreads); i < ((num + 1) * (MAX / NThreads)); i++){
      if (arr[i] == key)
         is_found = 1; 
   }
}

int main() {
   printf("\n**************Testing linear search using multiple thereads******************\n");
   thread_node threads[NThreads];
   for (int i = 0; i < NThreads; i++)
      mythread_create(&threads[i],linear_search, NULL);
   for (int i = 0; i < NThreads; i++)
      mythread_join(threads[i], NULL); 

   if (is_found == expected_result)
		printf("\nPASSED: Linear search test with %d threads\n",NThreads);
   else
		printf("\n**FAILED**: linear search test with %d threads\n",NThreads);
}