#include <stdio.h>
#include <stdlib.h>
#define QUEUE
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"

#include "../includes/utils/ProcessesTable.h"
#include "../includes/utils/algo_result.h"
#include<time.h>


typedef struct MultitaskProcess{
    Process process;
    int readyFrom;
    int new;
}MultitaskProcess;



int compare_process_priority_multilevel(const void *a, const void *b) {
    if((((MultitaskProcess*)a)->process).priority < (((MultitaskProcess*)b)->process).priority){
        return -1;
    }else if((((MultitaskProcess*)a)->process).priority > (((MultitaskProcess*)b)->process).priority){
        return 1;
    }else{
        if(((MultitaskProcess*)a)->readyFrom > ((MultitaskProcess*)b)->readyFrom){
            return -1;
        }else if(((MultitaskProcess*)a)->readyFrom < ((MultitaskProcess*)b)->readyFrom){
            return 1;
        }else{
            if(((MultitaskProcess*)a)->new > ((MultitaskProcess*)b)->new){
                return 1;
            }else{
                return -1;
            }
        }
    }
}

AlgoResult multilevel(Queue* queue,int processNumber,int quantum){
    int t=0,runTimeSum=0;
    float averageRotationTime=0.0,averageWaitingTime=0.0;
    PriorityQueue* pq = init_priority_queue(processNumber,sizeof(MultitaskProcess),compare_process_priority_multilevel);
    AlgoResult result;
    result.gantt=create_gantt();
    while(queue->front != NULL || pq->size!=0){
        if(queue->front != NULL){
            while(queue->front != NULL && queue->front->data.arrivalTime==t){
                Process process=dequeue(queue);
                MultitaskProcess pp = {0};
                runTimeSum+=process.runTime;
                pp.process=process;
                pp.readyFrom=t;
                pp.new=0;
                push(pq,&pp);
            }
        }
        if(pq->size!=0){
            MultitaskProcess* p = (MultitaskProcess *)pop(pq);

            for(int i=0;i<quantum;i++){
                (p->process).runTime--;
                t++;
                int isPriorityBigger=0;
                if(queue->front != NULL){
                    while(queue->front != NULL && queue->front->data.arrivalTime==t){
                        Process process=dequeue(queue);
                        MultitaskProcess pp= {0};
                        runTimeSum+=process.runTime;
                        pp.process=process;
                        pp.readyFrom=t;
                        pp.new=process.priority==(p->process).priority?1:0;
                        if(process.priority>(p->process).priority){
                            isPriorityBigger=1;
                        }
                        push(pq,&pp);
                    }
                }
                if(isPriorityBigger){
                    ReadyQueueElements elements = getPriorityQueueElements(pq);
                    char** list = (char**)malloc(elements.readyQueueSize*sizeof(char*));
                    for(int j=0;j<elements.readyQueueSize;j++){
                        list[j]=((MultitaskProcess*)elements.readyQueue[j])->process.processName;
                    }
                    enqueue_gantt(result.gantt,t-1,(p->process).processName,(p->process).runTime==0?1:0,list,elements.readyQueueSize);
                    if((p->process).runTime!=0){
                        p->readyFrom=t;
                        p->new=0;
                        push(pq,p);
                    }else{
                        averageRotationTime+=t-(p->process).arrivalTime;
                    }
                    break;
                }else{
                    if((p->process).runTime==0){
                        ReadyQueueElements elements = getPriorityQueueElements(pq);
                        char** list = (char**)malloc(elements.readyQueueSize*sizeof(char*));
                        for(int j=0;j<elements.readyQueueSize;j++){
                            list[j]=((MultitaskProcess*)elements.readyQueue[j])->process.processName;
                        }
                        enqueue_gantt(result.gantt,t-1,(p->process).processName,1,list,elements.readyQueueSize);
                        averageRotationTime+=t-(p->process).arrivalTime;
                        break;
                    }else if(i==quantum-1){
                        ReadyQueueElements elements = getPriorityQueueElements(pq);
                        char** list = (char**)malloc(elements.readyQueueSize*sizeof(char*));
                        for(int j=0;j<elements.readyQueueSize;j++){
                            list[j]=((MultitaskProcess*)elements.readyQueue[j])->process.processName;
                        }
                        enqueue_gantt(result.gantt,t-1,(p->process).processName,0,list,elements.readyQueueSize);

                        p->readyFrom=t;
                        p->new=0;
                        push(pq,p);
                    }else{
                        ReadyQueueElements elements = getPriorityQueueElements(pq);
                        char** list = (char**)malloc(elements.readyQueueSize*sizeof(char*));
                        for(int j=0;j<elements.readyQueueSize;j++){
                            list[j]=((MultitaskProcess*)elements.readyQueue[j])->process.processName;
                        }
                        enqueue_gantt(result.gantt,t-1,(p->process).processName,0,list,elements.readyQueueSize);
                    }
                }
            }
        }else{
            enqueue_gantt(result.gantt,t,NULL,0,NULL,0);
            t++;
        }
    }
    averageWaitingTime=(averageRotationTime-runTimeSum)/(float)processNumber;
    averageRotationTime/=(float)processNumber;
    add_metrics(&result,averageRotationTime,averageWaitingTime);
    return result;
}

int main(void){
    Process processes[8] = {
        {"P1", 0, 6, 2},
        {"P2", 1, 4, 3},
        {"P3", 2, 5, 3},
        {"P4", 4, 2, 1},
        {"P5", 5, 4, 1},
        {"P6", 6, 5, 2},
        {"P7", 12, 5, 4},
        {"P8", 13, 2, 4},
    };
    Queue* processesQ;
    processesQ = create_queue_from_array(processes,8);
    AlgoResult res = multilevel(processesQ,8,2);

    return 0;
}