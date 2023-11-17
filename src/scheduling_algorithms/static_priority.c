#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"


/******************************************************************
    PURPOSE: Static Priority Scheduler
    PARAMETERS:
    - n : number of processes to be scheduled in the system
    - processes[] : a sorted array of the processes to be scheduled
******************************************************************/
void static_priority_scheduler(Process processes[], int n) {
    PriorityQueue *pq = init_priority_queue(n, sizeof(Process), compare_process_priority);

    int currentTime = 0;
    int i = 0;

    while (i < n || !is_empty(pq)) {
        while (i < n && processes[i].arrivalTime <= currentTime) {
            push(pq, &processes[i]);
            i++;
        }

        if (!is_empty(pq)) {
            Process *currentProcess = pop(pq); //the one with highest priority to be executed
            printf("Executing Process %s at time %d ...\n", currentProcess->processName, currentTime);
            currentTime += currentProcess->runTime;
            printf("Process %s has terminated his job and left the CPU at time %d\n", currentProcess->processName, currentTime);
            free(currentProcess);
        } else {
            currentTime++;
        }
    }

    free_priority_queue(pq);
}
//TODO: calculate the metrics for static priority : average turnaround time (rotation time) // average waiting time 


// Example usage
int main() {
    Process processes[] = {
        {"p1", 0, 5, 1},
        {"p2", 2, 3, 1},
        {"p3", 2, 2, 2},
        {"p4", 3, 5, 3},
        {"p5", 9, 2, 5},
        {"p6", 10,1, 2}
    };
    int numProcesses = sizeof(processes) / sizeof(processes[0]);

    static_priority_scheduler(processes, numProcesses);

    return 0;
}
