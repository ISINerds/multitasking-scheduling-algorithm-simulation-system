#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"
#include "../includes/utils/ProcessesTable.h"


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

void preemptive_priority_scheduler(Process* processes, int processNumber) {
    Queue* processesQ = create_queue_from_array(processes, processNumber);
    PriorityQueue *pq = init_priority_queue(processNumber, sizeof(MultitaskProcess), compare_process_priority_multilevel);

    int currentTime = 0;
    int terminated =0 ;
    int rotationTime = 0;
    int waitingTime = 0;

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
            MultitaskProcess *highestPriorityProcess = (MultitaskProcess*)pop(pq);
            highestPriorityProcess->remainingRunTime--;
            currentTime++;
            printf("%s is running form t = %d to t = %d \n",(highestPriorityProcess->process).processName,currentTime-1,currentTime);
            int isPriorityBigger=0;
            if(!is_empty_q(processesQ)){
                while(processesQ->front != NULL && processesQ->front->data.arrivalTime==currentTime){
                    Process newProcess=dequeue(processesQ);
                    MultitaskProcess mProcess;
                    mProcess.process=newProcess;
                    mProcess.readyFrom=currentTime;
                    mProcess.remainingRunTime=newProcess.runTime;
                    mProcess.new= newProcess.priority==(highestPriorityProcess->process).priority ? 1 : 0 ;
                    if(newProcess.priority > (highestPriorityProcess->process).priority){
                        isPriorityBigger=1;
                    }
                    push(pq,&mProcess);
                }
            }
            if(isPriorityBigger){
                if(highestPriorityProcess->remainingRunTime!=0){
                    highestPriorityProcess->readyFrom=currentTime;
                    highestPriorityProcess->new=0;
                    push(pq,highestPriorityProcess);
                }
            }else{
                if(highestPriorityProcess->remainingRunTime==0){
                    terminated++;
                    printf("%s terminated his job at t=%d \n",(highestPriorityProcess->process).processName,currentTime);
                    // Calculate the rotation and waiting time
                    int currentRotationTime= currentTime - (highestPriorityProcess->process).arrivalTime;
                    rotationTime+= currentRotationTime;
                    waitingTime+=(currentRotationTime - (highestPriorityProcess->process).runTime);
                }
                else{
                    highestPriorityProcess->readyFrom=currentTime;
                    highestPriorityProcess->new=0;
                    push(pq,highestPriorityProcess);
                }
            }
        } else {
            currentTime++;
            printf("idle from t=%d to t=%d \n", currentTime-1, currentTime);
        }
    } 

    printf("Average rotation time : %.2fs\n" , (float)rotationTime/processNumber);
    printf("Average waiting time : %.2fs\n" , (float)waitingTime/processNumber);

    free_priority_queue(pq);
}


int main() {

    int processes_number = getNbProcesses("./src/processes.txt");  
    Process* processes = getTableOfProcesses("./src/processes.txt");

    // Process processes[] = {
    //     {"p1", 0, 5, 1},
    //     {"p2", 2, 3, 4},
    //     {"p3", 5, 10, 2},
    //     {"p4", 12, 4, 3}
    // };
    // int processes_number = sizeof(processes) / sizeof(processes[0]);

    preemptive_priority_scheduler(processes, processes_number);

    return 0;
}
