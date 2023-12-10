#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"
#include "../includes/utils/ProcessesTable.h"
#include "../includes/utils/algo_result.h"

typedef struct MultitaskProcess{
    Process process;
    int readyFrom;
    int new;
    int remainingRunTime;
}MultitaskProcess;



int compare_process_priority_multilevel(const void *a, const void *b) {
    if((((MultitaskProcess*)a)->process).priority < (((MultitaskProcess*)b)->process).priority){
        return -1;
    }else if((((MultitaskProcess*)a)->process).priority > (((MultitaskProcess*)b)->process).priority){
        return 1;
    }else{
        if((((MultitaskProcess*)a)->process).arrivalTime > (((MultitaskProcess*)b)->process).arrivalTime){
            return -1;
        }else if((((MultitaskProcess*)a)->process).arrivalTime < (((MultitaskProcess*)b)->process).arrivalTime){
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

AlgoResult preemptive_priority(Queue* processesQ, int processNumber, int quantum) {
    PriorityQueue *pq = init_priority_queue(processNumber, sizeof(MultitaskProcess), compare_process_priority_multilevel);

    int currentTime = 0;
    int terminated =0 ;
    int rotationTime = 0;
    int waitingTime = 0;

    AlgoResult algoResult;
    algoResult.gantt = create_gantt();

    while (terminated < processNumber && ( !is_empty_q(processesQ) || !is_empty_pq(pq))) {
        
        if(!is_empty_q(processesQ)){  
            while(processesQ->front != NULL && processesQ->front->data.arrivalTime==currentTime){
                Process newProcess = dequeue(processesQ);
                MultitaskProcess mProcess;
                mProcess.process=newProcess;
                mProcess.readyFrom=currentTime;
                mProcess.new=0;
                mProcess.remainingRunTime=newProcess.runTime;
                push(pq, &mProcess);
            }
        }

        if (!is_empty_pq(pq)) {
            int finish = 0;
            MultitaskProcess *highestPriorityProcess = (MultitaskProcess*)pop(pq);
            highestPriorityProcess->remainingRunTime--;

            ReadyQueueElements readyQueueElements = getPriorityQueueElements(pq);
            char ** readyQueue = (char*) malloc(readyQueueElements.readyQueueSize * sizeof(char*));
            for(int i=0;i<readyQueueElements.readyQueueSize;i++){
                readyQueue[i]= ((MultitaskProcess*)readyQueueElements.readyQueue[i])->process.processName;
                free(readyQueueElements.readyQueue[i]);
            }
            free(readyQueueElements.readyQueue);
            
            currentTime++;
            int isPriorityBigger=0;
            if(!is_empty_q(processesQ)){
                while(processesQ->front != NULL && processesQ->front->data.arrivalTime==currentTime){
                    Process newProcess=dequeue(processesQ);
                    MultitaskProcess mProcess;
                    mProcess.process=newProcess;
                    mProcess.readyFrom=currentTime;
                    mProcess.remainingRunTime=newProcess.runTime;
                    // mProcess.new= newProcess.priority==(highestPriorityProcess->process).priority ? 1 : 0 ;
                    mProcess.new = 0;
                    if(newProcess.priority > (highestPriorityProcess->process).priority){
                        isPriorityBigger=1;
                    }
                    push(pq,&mProcess);
                }
            }
            if(isPriorityBigger){
                if(highestPriorityProcess->remainingRunTime!=0){
                    highestPriorityProcess->readyFrom=currentTime;
                    highestPriorityProcess->new=1;
                    push(pq,highestPriorityProcess);
                }
            }else{
                if(highestPriorityProcess->remainingRunTime==0){
                    finish =1;
                    terminated++;
                    // Calculate the rotation and waiting time
                    int currentRotationTime= currentTime - (highestPriorityProcess->process).arrivalTime;
                    rotationTime+= currentRotationTime;
                    waitingTime+=(currentRotationTime - (highestPriorityProcess->process).runTime);
                }
                else{
                    highestPriorityProcess->readyFrom=currentTime;
                    highestPriorityProcess->new=1;
                    push(pq,highestPriorityProcess);
                }
            }
            enqueue_gantt(
                    algoResult.gantt, 
                    currentTime-1, 
                    highestPriorityProcess->process.processName,
                    finish,
                    readyQueue,
                    readyQueueElements.readyQueueSize
                );
        } else {
            currentTime++;
            enqueue_gantt(
                    algoResult.gantt, 
                    currentTime-1, 
                    NULL,
                    0,
                    NULL,
                    0
                );
        }
    } 

    float averageRotationTime = (float)rotationTime/processNumber;
    float averageWaitingTime = (float)waitingTime/processNumber;
    add_metrics(&algoResult, averageRotationTime, averageWaitingTime);

    free_priority_queue(pq);
    return algoResult;
}


//Usage Example
// int main() {

//     // int processes_number = getNbProcesses("./src/processes.txt");  
//     // Process* processes = getTableOfProcesses("./src/processes.txt");
//     //Queue* processesQ = create_queue_from_array(processes, processNumber);

//     Process processes[] = {
//         {"p1", 0, 5, 1},
//         {"p2", 2, 3, 4},
//         {"p3", 5, 10, 2},
//         {"p4", 12, 4, 3}
//     };
//     int processes_number = sizeof(processes) / sizeof(processes[0]);
//     Queue* processesQ = create_queue_from_array(processes, processes_number);
//     AlgoResult algoResult = preemptive_priority(processesQ, processes_number,0);

//     return 0;
// }
