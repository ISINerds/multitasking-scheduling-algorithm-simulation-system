#pragma once
#include "../utils/process.h"


//------------ Define structures
typedef struct Node {
    Process data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

int is_empty_q(Queue* queue);
Queue* create_queue();
Node* create_node(Process process);
void enqueue(Queue* queue, Process process);
Process dequeue(Queue* queue);
void swap_q( Process* a,  Process* b);
void sort_by_arrival_time(Process* processes, int numProcesses);
Queue* create_queue_from_array( Process* processes,int numProcesses);
int size_q(Queue* queue);
char** create_array_from_queue(Queue* queue, int size);


//------------ Check if the queue is empty or not
int is_empty_q(Queue* queue){
    return (queue->front == NULL);
}

//------------ Initialize an empty queue
Queue* create_queue(){

    Queue* queue = (Queue*)malloc(sizeof(Queue)); 
    if(!queue){
        printf("[ERROR] : Memory Allocation has failed!\n");
        exit(EXIT_FAILURE);
    }
    else{
        queue->front = queue->rear = NULL;
        return queue;
    }
}
//------------ Create a node for the queue
Node* create_node(Process process){
    Node* new_node = (Node*)malloc(sizeof(Node)); 
    if(!new_node){
        printf("[ERROR] : Memory Allocation has failed!\n");
        exit(EXIT_FAILURE);
    }
    else{
        new_node->data = process;
        new_node->next = NULL;
        return new_node;
    }
}
//------------ Add an element to the queue
void enqueue(Queue* queue, Process process) {
    Node* new_node = create_node(process);
    if(is_empty_q(queue)){
        queue->front = new_node;
        queue->rear = new_node;
    }
    else{
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

//------------ Remove an element from the queue and return its data ( whish is a process)
Process dequeue(Queue* queue) {
    if(is_empty_q(queue)){
        printf("[ERROR] : Queue is already empty\n");
        exit(EXIT_FAILURE);
    }
    else{
        Node* front_to_remove = queue->front;
        queue->front = front_to_remove->next;
        Process process_to_return = front_to_remove->data;
        free(front_to_remove);
        return process_to_return;
    }
};

// Function to swap two processes
void swap_q( Process* a,  Process* b){
    Process temp = *a;
    *a = *b;
    *b = temp;
}

//------------- Sort the array by Arrival time
void sort_by_arrival_time(Process* processes, int numProcesses){
    for (int i = 0; i < numProcesses - 1; i++) {
        for (int j = 0; j < numProcesses - i - 1; j++){
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                swap_q(&processes[j], &processes[j + 1]);
            }
        }
    }
}

//------------ Create a queue from an array
Queue* create_queue_from_array( Process* processes,int numProcesses){
    Queue* queue = create_queue();
    sort_by_arrival_time(processes,numProcesses);
    for (int i = 0; i < numProcesses; i++){
        enqueue(queue, processes[i]);
    }
    return queue;
}

int size_q(Queue* queue){
    Node* current_node = queue->front;
    int size=0;
    while(current_node != NULL){
        size++;
        current_node = current_node->next;
    }
    return size;
} 
char** create_array_from_queue(Queue* queue,int size){
    char** array_of_processes = (char**)malloc(size*sizeof(char*));
    int i=0;
    Node* current_node = queue->front;
    while(current_node){
        i[array_of_processes] = current_node->data.processName;
        i++;
        current_node = current_node->next;
    }
    return array_of_processes;
}
// An example where we define a static table of processes and then we create a queue from it. We add( enqueue)
// another process and then we dequeue them all while reading the processes names
// #include<stdio.h>
// #include<stdlib.h>
// #include "./queue.h"
// int main(void){
//     Process processes[] = {
//         {"Process1", 2, 10, 1},
//         {"Process2", 5, 8, 2},
//         {"Process3", 1, 15, 3},
//         {"Process4", 10, 15, 3},
//         {"Process5", 7, 5, 13},
//         {"Process6", 5, 4, 3},
//         {"Process7", 3, 7, 2},
//         {"Process8", 9, 4, 3},
//     };
//     Queue* q = create_queue_from_array(processes,8);
//     enqueue(q,(Process){"Process9",14, 4, 3});
//     char**t = create_array_from_queue(q,size_q(q));
//     for(int i=0;i<size_q(q);i++){
//         printf(t[i]);
//     }
//     while(!is_empty_q(q)){
//         Process p = dequeue(q);
//         printf("%s arrives at %d \n",p.processName , p.arrivalTime);
//     }
//     return 0;
// }


