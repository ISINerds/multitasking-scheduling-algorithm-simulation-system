#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"

#include "../includes/utils/ProcessesTable.h"
#include<time.h>
Queue* processesQ;

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

void multilevel(int quantum,int size){
    int t=0;
    PriorityQueue* pq = init_priority_queue(size,sizeof(MultitaskProcess),compare_process_priority_multilevel);
    while(processesQ->front != NULL || pq->size!=0){
        if(processesQ->front != NULL){
            while(processesQ->front != NULL && processesQ->front->data.arrivalTime==t){
                Process process=dequeue(processesQ);
                MultitaskProcess pp= {0};
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
                printf("%s is running form t = %d to t = %d \n",(p->process).processName,t-1,t);
                int isPriorityBigger=0;
                if(processesQ->front != NULL){
                    while(processesQ->front != NULL && processesQ->front->data.arrivalTime==t){
                        Process process=dequeue(processesQ);
                        MultitaskProcess pp= {0};
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
                    if((p->process).runTime!=0){
                        p->readyFrom=t;
                        p->new=0;
                        push(pq,p);
                    }
                    break;
                }else{
                    if((p->process).runTime==0){
                        break;
                    }else if(i==quantum-1){
                        p->readyFrom=t;
                        p->new=0;
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
    // int processes_number = getNbProcesses("./src/processes.txt");
    // Process* processes = getTableOfProcesses("./src/processes.txt");

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

    processesQ = create_queue_from_array(processes,8);
    clock_t start,end;
    double cpu_time_used;

    start=clock();
    multilevel(2,8);
    end=clock();
    cpu_time_used=((double)(end-start))/CLOCKS_PER_SEC;

    printf("time = %f ",cpu_time_used);

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