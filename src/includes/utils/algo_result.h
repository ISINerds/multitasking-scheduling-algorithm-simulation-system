#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "./logs.h"
#include "./instant_result_node.h"
#include "./metrics.h"
typedef struct Gantt{
    InstantResultNode* front;
    InstantResultNode* rear;
}Gantt;
typedef struct AlgoResult{
    Gantt* gantt;
    Metrics metrics;
}AlgoResult;
// Methods
Gantt* create_gantt();
int is_empty_gantt(Gantt* ganttQueue);
void enqueue_gantt(Gantt* ganttQueue,int t,char* processName,int quit,char** readyQueue,int readyQueueSize);
InstantResultNode dequeue_gantt(Gantt* ganttQueue);
InstantResultNode* create_instant_result_node(int t,char* processName,int quit,char** readyQueue,int readyQueueSize);
void add_metrics(AlgoResult* algoResult,float averageRotation,float averageWaiting);
int size_gantt(Gantt* ganttQueue);

Gantt* create_gantt(){
    Gantt* gantt = (Gantt*)malloc(sizeof(Gantt));
    if(!gantt){
        printf("[ERROR] : Memory Allocation has failed!\n");
        exit(EXIT_FAILURE);
    }
    gantt->front = NULL;
    gantt->rear  = NULL;
    return gantt;
}

int is_empty_gantt(Gantt* ganttQueue){
    return (ganttQueue->front==NULL);
}

InstantResultNode* create_instant_result_node(int t,char* processName,int quit,char** readyQueue,int readyQueueSize){
    InstantResultNode* newInstantResultNode = (InstantResultNode*)malloc(sizeof(InstantResultNode));
    if(!newInstantResultNode){
        printf("[ERROR] : Memory Allocation has failed!\n");
        exit(EXIT_FAILURE);
    }
    newInstantResultNode->t              = t;
    newInstantResultNode->processName    = processName;
    newInstantResultNode->quit           = quit;
    newInstantResultNode->readyQueue     = readyQueue;
    newInstantResultNode->readyQueueSize = readyQueueSize;
    newInstantResultNode->next           = NULL;
    return newInstantResultNode;

}


void enqueue_gantt(Gantt* ganttQueue, int t,char* processName,int quit,char** readyQueue,int readyQueueSize){
    InstantResultNode* newInstantResultNode = create_instant_result_node(t,processName,quit,readyQueue,readyQueueSize);
    // execution_log(*newInstantResultNode);
    if(is_empty_gantt(ganttQueue)){
        ganttQueue->front = newInstantResultNode;
        ganttQueue->rear  = newInstantResultNode;
    }else{
        ganttQueue->rear->next = newInstantResultNode;
        ganttQueue->rear       = newInstantResultNode;
    }
}

InstantResultNode dequeue_gantt(Gantt* ganttQueue){
    if(is_empty_gantt(ganttQueue)){
        printf("[ERROR] : Gantt queue is already empty\n");
        exit(EXIT_FAILURE);
    }
    InstantResultNode* front            = ganttQueue->front;
    InstantResultNode instantResultNode = *front;
    ganttQueue->front                   = front->next;
    free(front);
    return instantResultNode;
}

void add_metrics(AlgoResult* algoResult,float averageRotation,float averageWaiting){
    algoResult->metrics.averageRotation = averageRotation;
    algoResult->metrics.averageWaiting  = averageWaiting;
}

int size_gantt(Gantt* ganttQueue){
    InstantResultNode* current_node = ganttQueue->front;
    int size=0;
    while(current_node != NULL){
        size++;
        current_node = current_node->next;
    }
    return size;
}