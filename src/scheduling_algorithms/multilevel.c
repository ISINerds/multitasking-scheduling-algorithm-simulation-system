#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"

#include "../includes/utils/ProcessesTable.h"

Queue* processesQ;


// int compare_process_priority(const void *a, const void *b) {
//     return ((Process*)a)->priority > ((Process*)b)->priority;
// }
int compare_process_priority_arrivalTime(const void *a, const void *b) {
    return ((Process*)a)->arrivalTime < ((Process*)b)->arrivalTime;
}
int compare_process_priority_multilevel(const void *a, const void *b) {
    if(((Process*)a)->priority < ((Process*)b)->priority){
        return -1;
    }else if(((Process*)a)->priority > ((Process*)b)->priority){
        return 1;
    }else{
        if(((Process*)a)->arrivalTime > ((Process*)b)->arrivalTime){
            return -1;
        }else if(((Process*)a)->arrivalTime < ((Process*)b)->arrivalTime){
            return 1;
        }
        return 1;
    }
}

void multilevel(int quantum,int size){
    int t=0;
    PriorityQueue* pq = init_priority_queue(size,sizeof(Process),compare_process_priority_multilevel);
    while(processesQ->front != NULL || pq->size!=0){
        if(processesQ->front != NULL){
            while(processesQ->front != NULL && processesQ->front->data.arrivalTime==t){
                Process pp=dequeue(processesQ);
                push(pq,&pp);
            }
        }
        if(pq->size!=0){
            Process* p = (Process *)pop(pq);

            for(int i=0;i<quantum;i++){
                p->runTime--;
                t++;
                printf("%s is running form t = %d to t = %d \n",p->processName,t-1,t);
                int isPriorityBigger=0;
                if(processesQ->front != NULL){
                    while(processesQ->front != NULL && processesQ->front->data.arrivalTime==t){
                        Process pp = dequeue(processesQ);
                        if(pp.priority>p->priority){
                            isPriorityBigger=1;
                        }
                        push(pq,&pp);
                    }
                }
                if(isPriorityBigger){
                    if(p->runTime!=0){
                        p->arrivalTime=t;
                        push(pq,p);
                    }
                    break;
                }else{
                    if(p->runTime==0){
                        break;
                    }else if(i==quantum-1){
                        p->arrivalTime=t;
                        push(pq,p);
                    }
                }
            }
        }else{
            t++;
            printf("idle from t=%d to t=%d\n",t-1,t);
        }
        
    }
    
}

int main(void){
    // generate_processes_file("../config.conf","./test.txt",';');
    // struct Process *proc = getTableOfProcesses();

    // generate_processes_file("./src/config.conf", "./src/processes.txt", ';');
    int processes_number = getNbProcesses("./src/processes.txt");
    Process* processes = getTableOfProcesses("./src/processes.txt");

    // Process processes[8] = {
    //     {"P1", 0, 6, 2},
    //     {"P2", 1, 4, 3},
    //     {"P3", 2, 5, 3},
    //     {"P4", 4, 2, 1},
    //     {"P5", 5, 4, 1},
    //     {"P6", 6, 5, 2},
    //     {"P7", 12, 5, 4},
    //     {"P8", 13, 2, 4},
    // };

    processesQ = create_queue_from_array(processes,processes_number);
    

    multilevel(2,processes_number);
    // for(int i=0;i<8;i++){
    //     push(processesQ,&processes[i]);
    // }
    // char name[10];
    // int arrivalTime ;
    // int ExecTime;
    // int priority;
    // PriorityQueue *pq = init_priority_queue(8,sizeof(struct Process),compare_process_priority);
    // for(int i=0;i<8;i++){
    //     push(pq,&processes[i]);
    //     printf("%s %d %d %d\n",processes[i].name,processes[i].arrivalTime,processes[i].ExecTime,processes[i].priority);
    // }
    // printf("-------------------\n");
    // while(processesQ->size) {
	// 	void* e = pop(processesQ); // pop element from the priority_queue
   	//     printf("Popped: value=%s %d %d %d\n", ((struct Process *)e)->name, ((struct Process *)e)->arrivalTime, ((struct Process *)e)->ExecTime, ((struct Process *)e)->priority);	
   	//     free(e); // don't forget to free the popped element
	// }
	
    // free_priority_queue(pq); // free the priority_queue

    return 0;
}