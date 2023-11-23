#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"

#include "../includes/utils/ProcessesTable.h"
#include "../includes/utils/processes_generator.h"

Queue* processesQ;


int compare_int_return_bigger(const void *a, const void *b) {
    return (*(int *)a > *(int *)b);
}


void multilevel(int quantum,int size){
    int t=0;
    PriorityQueue* pq = init_priority_queue(7,sizeof(int),compare_int_return_bigger);
    Queue** queues = (Queue**) malloc(7 * sizeof(Queue*));
    for(int i=0;i<7;i++) {
        queues[i] = create_queue();
    }
    while(processesQ->front != NULL || pq->size!=0){
        if(processesQ->front != NULL){
            while(processesQ->front != NULL && processesQ->front->data.arrivalTime==t){
                int priority = processesQ->front->data.priority;
                if(is_empty_q(queues[priority])) {
                    push(pq,&priority);
                }
                enqueue(queues[priority], dequeue(processesQ));
            }
        }
        if(pq->size!=0){
            int* currPriorityPointer = (int*)pop(pq);
            int currPriority = *currPriorityPointer;
            free(currPriorityPointer);
            Node* currProcessNode = queues[currPriority]->front;
            for(int i=0;i<quantum;i++){
                currProcessNode->data.runTime--;
                t++;
                printf("%s is running from t = %d to t = %d\n",currProcessNode->data.processName,t-1,t);
                int isPriorityBigger=0;
                int samePriorityPushed = 0;
                if(processesQ->front != NULL){
                    while(processesQ->front != NULL && processesQ->front->data.arrivalTime==t){

                        int priority = processesQ->front->data.priority;
                        if(is_empty_q(queues[priority])) {
                            if(priority == currPriority) {
                                samePriorityPushed = 1;
                            }
                            push(pq,&priority);
                        }
                        
                        enqueue(queues[priority], dequeue(processesQ));
                        if(priority>currPriority){
                            isPriorityBigger=1;
                        }
                    }
                }
                if(isPriorityBigger){
                    Process finishedProcess = dequeue(queues[currPriority]);
                    if((!is_empty_q(queues[currPriority]) || (finishedProcess.runTime != 0) && !samePriorityPushed)) {
                        push(pq, &currPriority);
                    }
                    if(finishedProcess.runTime != 0) {
                        enqueue(queues[currPriority], finishedProcess);
                    }
                    break;
                }else{
                    if(currProcessNode->data.runTime ==0){
                        dequeue(queues[currPriority]);
                        if(!is_empty_q(queues[currPriority]) && !samePriorityPushed) {
                            push(pq, &currPriority);
                        }
                        break;
                    }else if(i==quantum-1){
                        Process finishedProcess = dequeue(queues[currPriority]);
                        enqueue(queues[currPriority], finishedProcess);
                        if(!samePriorityPushed) {
                            push(pq, &currPriority);
                        }
                    }
                }
            }
        }else{
            t++;
            printf("idle from t=%d to t=%d\n",t-1,t);
        }
        
    }
    for(int i=0;i<7;i++) {
        free(queues[i]);
    }
    free(queues);   
}

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

void multilevel2(int quantum,int size){
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
                printf("%s is running from t = %d to t = %d\n",(p->process).processName,t-1,t);
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


int main(int argc, char* argv[]){
    int processes_number = getNbProcesses("./src/processes.txt");
    Process* processes = getTableOfProcesses("./src/processes.txt");

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