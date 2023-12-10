#include <stdio.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/data_structures/queue.h"

#include "../includes/utils/ProcessesTable.h"
#include "../includes/utils/processes_generator.h"
#include "../includes/utils/algo_result.h"
Queue* processesQ;


int compare_int_return_bigger(const void *a, const void *b) {
    return (*(int *)a > *(int *)b);
}

void getInstantResult(Queue** queues, int* readyQueueSize, char*** list, int maximumPriorityClass) {
    (*readyQueueSize) = -1;
    for(int i=maximumPriorityClass;i;i--) {
        Node* q = queues[i]->front;
        while(q) {
            (*readyQueueSize)++;
            q = q->next;
        }
    }
    if(!readyQueueSize) {
        *readyQueueSize = 0;
        (*list)=NULL;
        return;
    }
    (*list) = (char**)malloc((*readyQueueSize)*sizeof(char*));
    int idx = 0;
    bool isFirstNode = false;
    for(int i=maximumPriorityClass;i;i--) {
        Node* q = queues[i]->front;
        if(!isFirstNode && q) {
            isFirstNode = true;
            q = q->next;
        }
        while(q) {
            (*list)[idx++] = q->data.processName;
            q = q->next;
        }
    }
    // for(int j=0;j<(*readyQueueSize);j++){
    //     printf("%s ", (*list)[j]);
    // }
    // printf("here\n");
    // printf("\n");
}

AlgoResult multilevel_v2(Queue* processesQ,int processNumber,int quantum){
    int maximumPriorityClass = maximumPriorityLevel("./config.conf");
    // printf("max prio = %d\n", maximumPriorityClass);
    int t=0;
    float totalRotationTime = 0, totalExecutionTime = 0;
    PriorityQueue* pq = init_priority_queue(maximumPriorityClass,sizeof(int),compare_int_return_bigger);
    AlgoResult result;
    result.gantt=create_gantt();
    Queue** queues = (Queue**) malloc((maximumPriorityClass + 1) * sizeof(Queue*));
    for(int i=0;i<=maximumPriorityClass;i++) {
        queues[i] = create_queue();
    }
    while(processesQ->front != NULL || pq->size!=0){
    // printf("here\n");
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
                totalExecutionTime++;
                t++;
                printf("%s is running from t = %d to t = %d\n",currProcessNode->data.processName,t-1,t);

                int readyQueueSize = 0;
                char** list;
                getInstantResult(queues, &readyQueueSize, &list, maximumPriorityClass);
                enqueue_gantt(result.gantt,t-1,currProcessNode->data.processName,currProcessNode->data.runTime==0?1:0,list,readyQueueSize);

                if(currProcessNode->data.runTime == 0) {
                    totalRotationTime += t - currProcessNode->data.arrivalTime;
                }
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
                    // printf("here\n");

                    // ReadyQueueElements existingPriorities = getPriorityQueueElements(pq);
                    // int* existingPrioritiesList = (int*)malloc(existingPriorities.readyQueueSize * sizeof(int));
                    // for(int i=0;i<existingPriorities.readyQueueSize;i++) {
                    //     existingPrioritiesList[i] = *(int*)existingPriorities.readyQueue[i];
                    //     printf("%d ", existingPrioritiesList[i]);
                    // }
                    // printf("\n");
                    // for(int i=0;i<existingPriorities.readyQueueSize;i++) {
                    //     int priorityLevel = existingPrioritiesList[i];
                    // }
                    

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
            enqueue_gantt(result.gantt,t,NULL,0,NULL,0);
            t++;
            // printf("idle from t=%d to t=%d\n",t-1,t);
        }
        
    }
    free_priority_queue(pq);
    for(int i=0;i<maximumPriorityClass+1;i++) {
        free_queue(queues[i]);
    }
    free(queues);   
    float averageRotationTime = (float)totalRotationTime / processNumber;
    float averageWaitingTime = (float)(totalRotationTime - totalExecutionTime) / processNumber;
    // totalWaitingTime=(averageRotationTime-runTimeSum)/(float)processNumber;
    // averageRotationTime/=(float)processNumber;
    add_metrics(&result,averageRotationTime,averageWaitingTime);
    // printf("final result: %d\n", size_gantt(result.gantt));
    return result;
}

int main(int argc, char* argv[]){
    int processes_number = getNbProcesses("./processes.txt");
    Process* processes = getTableOfProcesses("./processes.txt");

    processesQ = create_queue_from_array(processes,processes_number);
    
    printf("%d\n", size_gantt(multilevel_v2(processesQ, processes_number, 2).gantt));
    multilevel_v2(processesQ, processes_number, 2);
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