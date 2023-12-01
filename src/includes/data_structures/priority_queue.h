#pragma once
#include<string.h>
#include <stdbool.h>
#include "../utils/process.h"
#include "./queue.h"
#include "../utils/ReadyQueueElements.h"
// structure to represent the priority queue
typedef struct {
	void** data; // table of void* elements in order to make a generic pirority_queue
	int size; // size of the priority_queue
	int capacity; // maximum size of the priority_queue
	int dataSize; // size of each element to be added to the priority_queue
	int (*compare)(const void *, const void *); // function pointer for custom comparison function
} PriorityQueue;

int compare_process_runTime(const void *a, const void *b) {
    return ((Process*)a)->runTime < ((Process*)b)->runTime;
}

PriorityQueue *init_priority_queue(int capacity, int dataSize, int (*compare)(const void *, const void *));
void swap_pq(void **a, void **b);
void push(PriorityQueue *pq, void *data);
bool is_empty_pq(PriorityQueue* pq);
void* pop(PriorityQueue *pq);
void* top(PriorityQueue* pq);
void free_priority_queue(PriorityQueue *pq);
int compare_int(const void *a, const void *b);
int compare_process_priority(const void *a, const void *b);
ReadyQueueElements getPriorityQueueElements(PriorityQueue* pq);
void free_created_array_from_pq(ReadyQueueElements readyqueue);

// function to initialize an empty priority queue
PriorityQueue *init_priority_queue(int capacity, int dataSize, int (*compare)(const void *, const void *)) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue)); // allocating the priority_queue
    pq->data = (void **)malloc(sizeof(void*) * (capacity)); // allocating the data table
    pq->size = 0;
    pq->capacity = capacity;
    pq->dataSize = dataSize;
    pq->compare = compare;
    return pq;
}

// function to swap two elements in the priority queue
void swap_pq(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

// function to push an element into the priority queue
void push(PriorityQueue *pq, void *data) {
    if (pq->size == pq->capacity) {
        printf("Priority queue overflow\n");
        return;
    }
	pq->data[pq->size] = malloc(pq->dataSize); // allocating needed memory for the new element to be pushed
    memcpy(pq->data[pq->size], data, pq->dataSize); // copy the data into the priority_queue
    // perform heapify-up to maintain the heap property
    int i = pq->size;
    while (i > 0 && pq->compare(pq->data[i], pq->data[(i-1) / 2]) > 0) {
        swap_pq(&pq->data[i], &pq->data[(i-1) / 2]);
        i = (i-1) / 2;
    }
    pq->size++; // incrementing the size of the priority_queue
}

// function to check if the priority_queue is empty
bool is_empty_pq(PriorityQueue* pq) {
	return !pq->size;
}

ReadyQueueElements getPriorityQueueElements(PriorityQueue* pq) {
    void** array = (void**) malloc(pq->size * sizeof(void*));
    int idx = 0;
    while(pq->size) {
        array[idx] = malloc(pq->dataSize);
        memcpy(array[idx++], top(pq), pq->dataSize);
        free(pop(pq));
    }
    for(int i=0;i<idx;i++) {
        push(pq, i[array]);
        // push(pq, array[i]);
    }
    return (ReadyQueueElements){array, pq->size};
}
void free_created_array_from_pq(ReadyQueueElements readyqueue) {
    for(int i=0;i<readyqueue.readyQueueSize;i++) {
        free(readyqueue.readyQueue[i]);
    }
    free(readyqueue.readyQueue);
}
// function to pop the element with the highest priority from the priority_queue
void* pop(PriorityQueue *pq) {
    if (is_empty_pq(pq)) {
        printf("Priority queue underflow\n");
		exit(0);
    }
	pq->size--; // decrementing the size of the priority_queue
    swap_pq(&pq->data[0], &pq->data[pq->size]); // swap the root with the last element
    void* poppedData = pq->data[pq->size];  // store the poppedData
    // perform heapify-down to maintain the heap property
    int i = 0;
    while (1) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        int maxIndex = i;

        if (leftChild < pq->size && pq->compare(pq->data[leftChild], pq->data[maxIndex]) > 0) {
            maxIndex = leftChild;
        }

        if (rightChild < pq->size && pq->compare(pq->data[rightChild], pq->data[maxIndex]) > 0) {
            maxIndex = rightChild;
        }

        if (maxIndex != i) {
            swap_pq(&pq->data[i], &pq->data[maxIndex]);
            i = maxIndex;
        }
		else {
            break;
        }
    }
	return poppedData;
}

void* top(PriorityQueue* pq) {
    if(is_empty_pq(pq)) {
        exit(EXIT_FAILURE);
    }
    return pq->data[0];
}

// function to free the memory allocated for the priority_queue
void free_priority_queue(PriorityQueue *pq) {
    for (int i = 0; i < pq->size; ++i) {
        free(pq->data[i]);
    }
    free(pq->data);
    free(pq);
}

// example custom comparison function for integers
int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// example custom comparison function for process priority
int compare_process_priority(const void *a, const void *b) {
    return ((Process*)a)->priority > ((Process*)b)->priority;
}





// int main(void) {
// 	// PriorityQueue *pq = init_priority_queue(11, sizeof(int), compare_int);
//    PriorityQueue *pq = init_priority_queue(11, sizeof(Process), compare_process_priority);

//    // int values[] = {50, 19, 0, 14, 6, 3, 8, 16};
// 	Process values[] = {{"pqrs", 0, 7, 8}, {"a", 5, 4, 2}, {"b", 12, 15, 88}, {"c", 8, 14, 3}, {"d", 10, 17, 18}, {"e", 5, 4, 0}};
    
//    for (int i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
//        push(pq, &values[i]); // push elements into the priority_queue
//    }
//    ReadyQueueElements readyqueue = getPriorityQueueElements(pq);
//    for(int i=0;i<readyqueue.readyQueueSize;i++) {
//       void* e = readyqueue.readyQueue[i];
//       printf("Popped: value=%s %d %d %d\n", ((Process *)e)->processName, ((Process *)e)->arrivalTime, ((Process *)e)->runTime, ((Process *)e)->priority);	
//       // printf("Popped: value=%d\n", *((int*)e));
//    }
//    printf("\n");
// 	while(pq->size) {
// 		void* e = pop(pq); // pop element from the priority_queue
//    	printf("Popped: value=%s %d %d %d\n", ((Process *)e)->processName, ((Process *)e)->arrivalTime, ((Process *)e)->runTime, ((Process *)e)->priority);	
// 		// printf("Popped: value=%d\n", *((int*)e));
//    	free(e); // don't forget to free the popped element
// 	}
	
//    free_priority_queue(pq); // free the priority_queue
//    free_created_array_from_pq(readyqueue); // free the created array from pq
//    return 0;
// }