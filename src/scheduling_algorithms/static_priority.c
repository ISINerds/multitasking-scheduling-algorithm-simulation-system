#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"
#include "../includes/utils/ProcessesTable.h"


void static_priority_scheduler(Process* processes, int processNumber) {
    Queue* processesQ = create_queue_from_array(processes, processNumber);
    PriorityQueue *pq = init_priority_queue(processNumber, sizeof(Process), compare_process_priority);

    int currentTime = 0;
    int rotationTime = 0;
    int waitingTime = 0;

    while (!is_empty_q(processesQ) || !is_empty_pq(pq)) { 
        if(!is_empty_q(processesQ)){  
            while(processesQ->front != NULL && processesQ->front->data.arrivalTime<=currentTime){
                Process process = dequeue(processesQ);
                push(pq, &process);
            }
        }

        if (!is_empty_pq(pq)) {
            Process *currentProcess = (Process*) pop(pq); //the one with highest priority to be executed
            printf("Executing %s at time %d ...\n", currentProcess->processName, currentTime);
            currentTime += currentProcess->runTime;
            printf("%s has terminated his job and left the CPU at time %d\n", currentProcess->processName, currentTime);
            // Calculate the rotation and waiting time
            int currentRotationTime= currentTime - currentProcess->arrivalTime;
            rotationTime+= currentRotationTime;
            waitingTime+=(currentRotationTime - currentProcess->runTime);
            free(currentProcess);
        } else {
            currentTime++;
            printf("idle from time %d to %d \n", currentTime-1, currentTime);
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
    //     {"p2", 2, 3, 1},
    //     {"p3", 2, 2, 2},
    //     {"p4", 3, 5, 3},
    //     {"p5", 9, 2, 5},
    //     {"p6", 10,1, 2}
    // };
    // int processes_number = sizeof(processes) / sizeof(processes[0]);

    static_priority_scheduler(processes, processes_number);

    return 0;
}
