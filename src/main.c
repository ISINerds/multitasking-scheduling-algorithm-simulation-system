#include <stdio.h>
#include "./includes/data_structures/priority_queue.h"

#ifndef PROCESS
#define PROCESS
#include "./includes/utils/process.h"
#endif

int main(void) {
//	PriorityQueue *pq = init_priority_queue(11, sizeof(int), compare_int);
    PriorityQueue *pq = init_priority_queue(11, sizeof(process), compare_process_priority);

//    int values[] = {50, 19, 0, 14, 6, 3, 8, 16};
	process values[] = {{'p', 0, 7, 8}, {'a', 5, 4, 2}, {'b', 12, 15, 88}, {'c', 8, 14, 3}, {'d', 10, 17, 18}, {'e', 5, 4, 0}};
    
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        push(pq, &values[i]); // push elements into the priority_queue
    }
    
	while(pq->size) {
		void* e = pop(pq); // pop element from the priority_queue
    	printf("Popped: value=%c %d %d %d\n", ((process *)e)->name, ((process *)e)->arrivalTime, ((process *)e)->executionTime, ((process *)e)->priority);	
//		printf("Popped: value=%d\n", *((int*)e));
    	free(e); // don't forget to free the popped element
	}
	
    free_priority_queue(pq); // free the priority_queue

    return 0;
}
