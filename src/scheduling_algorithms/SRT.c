#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"
#include "../includes/utils/algo_result.h"
#include "../includes/utils/ProcessesTable.h"

AlgoResult SRT (Queue * Q,int sizeOfArray, int quantum) {

    PriorityQueue * PQ = init_priority_queue(sizeOfArray,sizeof(Process),compare_process_runTime);
    // int idle=0;
    int tRotation=0;
    int allRunTimes=0;
    int tWaiting=0;

    int current_time = 0;
    int completed_processes = 0;
    

    //algo result
    Gantt * gantt= create_gantt();
    while(completed_processes<sizeOfArray) {
        if (!is_empty_q(Q)) {
            Process firstP = {0} ;
            while (!is_empty_q(Q)&& Q->front->data.arrivalTime<=current_time) {
                firstP=dequeue(Q);
                allRunTimes += firstP.runTime;
                push(PQ, &firstP);
                // Process firstP = (Q->front)->data ;
            }
        }
        if (!is_empty_pq(PQ)){
            Process * electedProcess = (Process *)pop(PQ);
            electedProcess->runTime--;
            
            ReadyQueueElements RQElements = getPriorityQueueElements(PQ);
            char** tableofNames=(char**)malloc(RQElements.readyQueueSize*sizeof(char*));
            //printf(" At instant t= %d ready queue is : ",current_time);
            for (int k=0;k<RQElements.readyQueueSize;k++) {
                void* e= RQElements.readyQueue[k];
                tableofNames[k]=((Process*)e)->processName;
                free(RQElements.readyQueue[k]);
                //printf(" %s ", ((Process*)e)->processName);
            }
            free(RQElements.readyQueue);
            // for (int k=0;k<RQElements.readyQueueSize;k++) {
            //     printf(" %s ",tableofNames[k]);
            // }
            //printf("\n");

            

            if (electedProcess->runTime==0){
                completed_processes++;

                enqueue_gantt(gantt,current_time,electedProcess->processName,1,tableofNames,RQElements.readyQueueSize);

                //printf("\n ! Process %s has finished executing at t: %d ! \n",electedProcess->processName,current_time+1);
                int actualRotationTime= current_time+1 - electedProcess->arrivalTime;
                tRotation+= actualRotationTime;
                tWaiting+= actualRotationTime;

            }
            else {
                enqueue_gantt(gantt,current_time,electedProcess->processName,0,tableofNames,RQElements.readyQueueSize);
                push(PQ,electedProcess);
            }
            free(electedProcess);
        }
        else {
            enqueue_gantt(gantt,current_time,NULL,0,NULL,0);
            //printf("********** CPU is IDLE ***********\n");
            // idle++;
        }        

        current_time++;
    }
    tWaiting -= allRunTimes;
    float averageWaitingTime = (float) tWaiting/sizeOfArray;
    float averageRotationTime= (float) tRotation/sizeOfArray;

    AlgoResult algoResult;
    add_metrics(&algoResult,averageRotationTime,averageWaitingTime);
    algoResult.gantt=gantt;

    // printf("\n IDLE CPU Percentage : %.1f%% \n", (float)idle/current_time*100);
    // printf ("************************************ \n  average rotation time - : %.2f \n", averageRotationTime);
    // printf ("************************************ \n  average waiting time - : %.2f \n", averageWaitingTime);
    
    free_priority_queue(PQ);
    // free(Q);

    return algoResult;

}

// int main(void) {
//     // int sizeOfArray = getNbProcesses("./src/processes.txt");
//     // Process* processes = getTableOfProcesses("./src/processes.txt");
//     int sizeOfArray=4;
//     Process processes[4] = {
//         {"P1",0,5,3},
//         {"P2",1,3,2},
//         {"P3",2,1,4},
//         {"P4",4,2,1},
//     };
//     AlgoResult algoResult;
//     algoResult=SRT(sizeOfArray,&processes[0]);
//      while(!is_empty_gantt(algoResult.gantt)){
//         InstantResultNode i = dequeue_gantt(algoResult.gantt);
//         printf("\nfrom instant t=%d to t=%d, Process executing is : %s , and finished? : %d ,ready queue size : %d ",i.t,i.t+1,i.processName,i.quit,i.readyQueueSize);
//         printf("\n ready Processes :",i.t);
//         for(int k=0;k<i.readyQueueSize;k++){
//             printf("%s  ", (char*)i.readyQueue[k]);
//         }
//         // printf("from instant t=%d to t=%d, Process executing is : %s , and finished? : %d ,ready queue size : %d \n",i.t,i.t+1,i.processName,i.quit,i.readyQueueSize);
//     }
//     return 0;
// }