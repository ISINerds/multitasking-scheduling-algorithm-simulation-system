#include "process.h"

//------------ Define structures
struct Node {
    struct Process data;
    struct Node* next;
};

struct Queue{
    struct Node* front;
    struct Node* rear;
};

//------------ Check if the queue is empty or not
int is_empty(struct Queue* queue){
    return (queue->front == NULL);
}

//------------ Initialize an empty queue
struct Queue* create_queue(){

    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue)); 
    if(queue == NULL){
        printf("Memory Allocation has failed!\n");
        exit(EXIT_FAILURE);
    }
    else{
        queue->front=NULL;
        queue->rear = NULL;
        return queue;
    }
}
//------------ Create node for the queue
struct Node* create_node(struct Process process){
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node)); 
    if(new_node==NULL){
        printf("Memory Allocation has failed!\n");
        exit(EXIT_FAILURE);
    }
    else{
        new_node->data = process;
        new_node->next = NULL;
        return new_node;
    }

}
//------------ Add element to the queue
void enqueue(struct Queue* queue, struct Process process) {
    struct Node* new_node = create_node(process);
    if(is_empty(queue)){
        queue->front = new_node;
        queue->rear = new_node;
    }
    else{
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

//------------ Remove element from the queue
struct Process dequeue(struct Queue* queue) {
    if(is_empty(queue)){
        printf("Queue is already empty\n");
        exit(EXIT_FAILURE);
    }
    else{
        struct Node* front_to_remove = queue->front;
        queue->front = front_to_remove->next;
        struct Process process_to_return = front_to_remove->data;
        free(front_to_remove);
        return process_to_return;
    }
};

// Function to swap two processes
void swap(struct Process* a, struct Process* b){
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

//------------- Sort the array by Arrival time
void sort_by_arrival_time(struct Process* processes, int numProcesses){
    for (int i = 0; i < numProcesses - 1; i++) {
        for (int j = 0; j < numProcesses - i - 1; j++){
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                swap(&processes[j], &processes[j + 1]);
            }
        }
    }
}

//------------ create queue from an array
struct Queue* create_queue_from_array(struct Process* processes,int numProcesses){
    struct Queue* queue = create_queue();
    sort_by_arrival_time(processes,numProcesses);
    for (int i = 0; i < numProcesses; i++){
        enqueue(queue, processes[i]);
    }
    return queue;
}






