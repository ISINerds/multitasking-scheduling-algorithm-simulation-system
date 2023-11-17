#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/utils/ProcessesTable.h"

struct Process FirstArrived(struct Process *array,int sizeOfArray,int indice,struct Process first) {
    if (sizeOfArray <= 0) {
        // Handle case when array is empty or invalid size
        // Here, returning a default Process struct
        printf("Error, the array is empty ! ");
        struct Process defaultProcess;
        defaultProcess.arrivalTime = -1; // Set an invalid execution time
        return defaultProcess;
    }
    struct Process FirstArrived = first;
    for (int i=indice;i<sizeOfArray;i++) {
        if (array[i].ExecTime < FirstArrived.arrivalTime){
            FirstArrived = array[i];
        }
    }
    return FirstArrived;
}

struct Process minExecTime(struct Process *array,int sizeOfArray,int indice,struct Process min) {
    if (sizeOfArray <= 0) {
        // Handle case when array is empty or invalid size
        // Here, returning a default Process struct
        printf("Error, the array is empty ! ");
        struct Process defaultProcess;
        defaultProcess.ExecTime = -1; // Set an invalid execution time
        return defaultProcess;
    }
    struct Process minimalProcess = min;
    for (int i=indice;i<sizeOfArray;i++) {
        if (array[i].ExecTime < minimalProcess.ExecTime){
            minimalProcess = array[i];
        }
    }
    return minimalProcess;
}

void switchProcesses(struct Process *p1, struct Process *p2) {
    // printf("\n Trying to switch pointers :\n");
    //  printf("\n to be switched %s :\n",p1->name);
    //  printf("\n The minimal : %s :\n",p2->name);
    struct Process* temp= p1;
    *p1=*p2;
    *p2=*temp;
} 

// [5,10,5,3,7,1,10,7] 
// [1,10,5,3,7,5,10,7] 
// [1,3,5,10,7,5,10,7]
// [1,3,5,*5,7,10,10,7] 
// [1,3,5,*5,*7,*7,10,10] 


struct Process * sortArrayByExecTime(struct Process * array,int sizeOfArray) {
    for (int i=0; i<sizeOfArray-1 ;i++){
        struct Process minimalProcess= minExecTime(array,sizeOfArray,i+1,array[i]);
        printf("\n Minimal ex time : %d \n",minimalProcess.ExecTime);
        if (array[i].ExecTime > minimalProcess.ExecTime){
            switchProcesses(&array[i],&minimalProcess);
            printf("\n Ancien minimal :%s \n",minimalProcess.name);
            printf("\n Ancien non min :%s \n",array[i].name);
        }
    }
    return array;
}

void SRT(struct Process* array, int sizeOfArray, PriorityQueue * PQ ) {

    int current_time = 0;
    int completed_processes = 0;
    int current_process_index ;
    // struct Process startP= FirstArrived(array,sizeOfArray,1,array[0]);
    // printf("*----------------*%s",startP.arrivalTime);
    while(completed_processes<sizeOfArray) {
        for (int i = 0; i < sizeOfArray; ++i) {
            if (array[i].arrivalTime == current_time) {
                push(pq, &processes[i]);
            }
        }



        current_time++;
    }

    free_priority_queue(pq);

}

int main(void) {
    int sizeOfArray = getNbProcesses();
    struct Process * array1=getTableOfProcesses();
    // if (array1==NULL) {
    //     printf("\n Error getting the array of processes ! \n");
    //     return 0;
    // }
    // struct Process * arraySorted = sortArrayByExecTime(array1,sizeOfArray);
    // for(int i=0;i<sizeOfArray;i++) {
    //     printf("\n element number : %d \n",i);
    //     printf("%d",arraySorted[i].ExecTime);
    // }

    PriorityQueue * PQ = init_priority_queue(sizeOfArray,sizeof(array1)/sizeof(array1[0]),compare_int);

    return 0;
}