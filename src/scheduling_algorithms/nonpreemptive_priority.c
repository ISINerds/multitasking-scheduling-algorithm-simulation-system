#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"
#include "../includes/utils/ProcessesTable.h"
#include "../includes/utils/algo_result.h"

AlgoResult nonpreemptive_priority(Queue* processesQ, int processNumber, int quantum) {
    PriorityQueue *pq = init_priority_queue(processNumber, sizeof(Process), compare_process_priority);

    int currentTime = 0;
    int rotationTime = 0;
    int waitingTime = 0;

    AlgoResult algoResult;
    algoResult.gantt = create_gantt();

    while (!is_empty_q(processesQ) || !is_empty_pq(pq)) { 
        if(!is_empty_q(processesQ)){  
            while(processesQ->front != NULL && processesQ->front->data.arrivalTime<=currentTime){
                Process process = dequeue(processesQ);
                push(pq, &process);
            }
        }

        if (!is_empty_pq(pq)) {
            Process *currentProcess = (Process*) pop(pq); //the one with highest priority to be executed
            
            int finish =0 ; 
            for (int t=currentTime; t<currentTime+currentProcess->runTime;t++){
                ReadyQueueElements readyQueueElements = getPriorityQueueElements(pq);
                char ** readyQueue = (char*) malloc(readyQueueElements.readyQueueSize * sizeof(char*));
                for(int i=0;i<readyQueueElements.readyQueueSize;i++){
                    readyQueue[i]= ((Process*)readyQueueElements.readyQueue[i])->processName;
                }
                if(t==currentTime+currentProcess->runTime-1){
                    finish =1;
                }
                enqueue_gantt(
                    algoResult.gantt, 
                    t, 
                    currentProcess->processName,
                    finish,
                    readyQueue,
                    readyQueueElements.readyQueueSize
                );
            }
            currentTime += currentProcess->runTime;
            // Calculate the rotation and waiting time
            int currentRotationTime= currentTime - currentProcess->arrivalTime;
            rotationTime+= currentRotationTime;
            waitingTime+=(currentRotationTime - currentProcess->runTime);
            free(currentProcess);
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


// int main() {

//     // int processes_number = getNbProcesses("./src/processes.txt");  
//     // Process* processes = getTableOfProcesses("./src/processes.txt");

//     Process processes[] = {
//         {"p1", 0, 5, 1},
//         {"p2", 2, 3, 1},
//         {"p3", 2, 2, 2},
//         {"p4", 3, 5, 3},
//         {"p5", 9, 2, 5},
//         {"p6", 10,1, 2}
//     };
//     int processes_number = sizeof(processes) / sizeof(processes[0]);
//     Queue* processesQ = create_queue_from_array(processes, processes_number);

//     AlgoResult AlgoResult = nonpreemptive_priority(processesQ, processes_number, 0);

//     return 0;
// }
