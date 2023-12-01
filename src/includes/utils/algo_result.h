#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "./logs.h"
#include "./instant_result_node.h"
typedef struct Metrics{
    float averageRotation;
    float averageWaiting;
}Metrics;
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
    execution_log(*newInstantResultNode);
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



// example :
// #include<stdio.h>
// #include "algo_result.h"

// int main(void){
//     AlgoResult result;
//     result.gantt=create_gantt();
//     add_metrics(&result,1.4,1.5);
//     char** list = (char**)malloc(3*sizeof(char*));
//     list[0]="hi";
//     list[1]="hello";
//     list[2]="by";
//     for(int i=0;i<10;i++){
//         enqueue_gantt(result.gantt,i,"hello",0,list,3);
//     }
//     while(!is_empty_gantt(result.gantt)){
//         InstantResultNode node = dequeue_gantt(result.gantt);
//         printf("%d\n",node.t);
//     }
//     printf("%f %f\n",result.metrics.averageRotation,result.metrics.averageWaiting);
//     return 1;
// }