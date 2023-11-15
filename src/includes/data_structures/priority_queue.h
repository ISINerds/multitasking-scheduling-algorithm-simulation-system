#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

#ifndef PROCESS
#define PROCESS
#include "../utils/process.h"
#endif

// structure to represent the priority queue
typedef struct {
	void** data; // table of void* elements pointing to void in order to make a generic pirority_queue
	int size; // size of the priority_queue
	int capacity; // maximum size of the priority_queue
	int dataSize; // size of each element to be added to the priority_queue
	int (*compare)(const void *, const void *); // function pointer for custom comparison function
} PriorityQueue;

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
void swap(void **a, void **b) {
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
        swap(&pq->data[i], &pq->data[(i-1) / 2]);
        i = (i-1) / 2;
    }
    pq->size++; // incrementing the size of the priority_queue
}

// function to check if the priority_queue is empty
bool is_empty(PriorityQueue* pq) {
	return !pq->size;
}

// function to pop the element with the highest priority from the priority_queue
void* pop(PriorityQueue *pq) {
    if (is_empty(pq)) {
        printf("Priority queue underflow\n");
		exit(0);
    }
    
    void *poppedData = malloc(pq->dataSize); // allocating memory for the element to be popped
    memcpy(poppedData, pq->data[0], pq->dataSize); // copying the data into the element to be popped
	pq->size--; // decrementing the size of the priority_queue
    swap(&pq->data[0], &pq->data[pq->size]); // swap the root with the last element
    free(pq->data[pq->size]); // freeing the memory of the deleted element
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
            swap(&pq->data[i], &pq->data[maxIndex]);
            i = maxIndex;
        }
		else {
            break;
        }
    }
	return poppedData;
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
    return ((process*)a)->priority > ((process*)b)->priority;
}
