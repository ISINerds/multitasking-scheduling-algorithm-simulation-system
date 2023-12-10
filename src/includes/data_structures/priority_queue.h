#pragma once
#include<string.h>
#include <stdbool.h>
#include "../utils/process.h"
#include "../utils/ReadyQueueElements.h"
typedef struct {
	void** data;
	int size;
	int capacity;
	int dataSize;
	int (*compare)(const void *, const void *);
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

PriorityQueue *init_priority_queue(int capacity, int dataSize, int (*compare)(const void *, const void *)) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue)); // allocating the priority_queue
    pq->data = (void **)malloc(sizeof(void*) * (capacity)); // allocating the data table
    pq->size = 0;
    pq->capacity = capacity;
    pq->dataSize = dataSize;
    pq->compare = compare;
    return pq;
}

void swap_pq(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

void push(PriorityQueue *pq, void *data) {
    if (pq->size == pq->capacity) {
        printf("Priority queue overflow\n");
        return;
    }
	pq->data[pq->size] = malloc(pq->dataSize);
    memcpy(pq->data[pq->size], data, pq->dataSize);
    int i = pq->size;
    while (i > 0 && pq->compare(pq->data[i], pq->data[(i-1) / 2]) > 0) {
        swap_pq(&pq->data[i], &pq->data[(i-1) / 2]);
        i = (i-1) / 2;
    }
    pq->size++;
}
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
    }
    return (ReadyQueueElements){array, pq->size};
}
void free_created_array_from_pq(ReadyQueueElements readyqueue) {
    for(int i=0;i<readyqueue.readyQueueSize;i++) {
        free(readyqueue.readyQueue[i]);
    }
    free(readyqueue.readyQueue);
}
void* pop(PriorityQueue *pq) {
    if (is_empty_pq(pq)) {
        printf("Priority queue underflow\n");
		exit(0);
    }
	pq->size--;
    swap_pq(&pq->data[0], &pq->data[pq->size]);
    void* poppedData = pq->data[pq->size];
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

void free_priority_queue(PriorityQueue *pq) {
    for (int i = 0; i < pq->size; ++i) {
        free(pq->data[i]);
    }
    free(pq->data);
    free(pq);
}

int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int compare_process_priority(const void *a, const void *b) {
    return ((Process*)a)->priority > ((Process*)b)->priority;
}