#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"
#include "../includes/utils/ProcessesTable.h"

void SRT (int sizeOfArray,Process * processes) {

    Queue* Q = create_queue_from_array(processes,sizeOfArray);
    PriorityQueue * PQ = init_priority_queue(sizeOfArray,sizeof(Process),compare_process_runTime);

    int idle=0;
    int tRotation=0;
    int allRunTimes=0;
    int tWaiting=0;

    int current_time = 0;
    int completed_processes = 0;
    Process * electedProcess;

    while(completed_processes<sizeOfArray) {
        if (!is_empty_q(Q)) {
            Process firstP = (Q->front)->data ;
            if (firstP.arrivalTime==current_time) {
                allRunTimes += firstP.runTime;
                firstP=dequeue(Q);
                push(PQ, &firstP);
            }
        }
        if (!is_empty_pq(PQ)){
            electedProcess = (Process *)pop(PQ);
            printf("\n Process %s is executing from t: %d to t: %d \n",electedProcess->processName,current_time,current_time+1);
            electedProcess->runTime--;

            if (electedProcess->runTime==0){
                completed_processes++;
                printf("\n ! Process %s has finished executing at t: %d ! \n",electedProcess->processName,current_time+1);
                int actualRotationTime= current_time+1 - electedProcess->arrivalTime;
                tRotation+= actualRotationTime;
                tWaiting+= actualRotationTime;
            }
            else {
                push(PQ,electedProcess);
            }
        }
        else {
            printf("********** CPU is IDLE ***********\n");
            idle++;
        }

        current_time++;
    }
    tWaiting -= allRunTimes;
    float averageWaitingTime = (float) tWaiting/sizeOfArray;
    float averageRotationTime= (float) tRotation/sizeOfArray;
    printf("\n IDLE CPU Percentage : %.1f%% \n", (float)idle/current_time*100);
    printf ("************************************ \n  average rotation time - : %.2f \n", averageRotationTime);
    printf ("************************************ \n  average waiting time - : %.2f \n", averageWaitingTime);
    free_priority_queue(PQ);
    free(Q);

}

int main(void) {
    int sizeOfArray = getNbProcesses("./src/processes.txt");
    Process* processes = getTableOfProcesses("./src/processes.txt");
    SRT(sizeOfArray,processes);
    return 0;
}