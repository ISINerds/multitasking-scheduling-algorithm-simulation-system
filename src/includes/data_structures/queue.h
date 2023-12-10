#pragma once
#include "../utils/process.h"


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
void free_queue(Queue* q);

int is_empty_q(Queue* queue){
    return (queue->front == NULL);
}

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

void swap_q( Process* a,  Process* b){
    Process temp = *a;
    *a = *b;
    *b = temp;
}

void sort_by_arrival_time(Process* processes, int numProcesses){
    for (int i = 0; i < numProcesses - 1; i++) {
        for (int j = 0; j < numProcesses - i - 1; j++){
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                swap_q(&processes[j], &processes[j + 1]);
            }
        }
    }
}

Queue* create_queue_from_array( Process* processes,int numProcesses){
    Process* copy_from_processes = (Process*)malloc(numProcesses * sizeof(Process));
    memcpy(copy_from_processes, processes, numProcesses * sizeof(Process));
    Queue* queue = create_queue();
    sort_by_arrival_time(copy_from_processes,numProcesses);
    for (int i = 0; i < numProcesses; i++){
        enqueue(queue, copy_from_processes[i]);
    }
    free(copy_from_processes);
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
void free_queue(Queue* q) {
    Queue* tmp = NULL;
    while(!is_empty_q(q)) {
        dequeue(q);
    }
    free(q);
}