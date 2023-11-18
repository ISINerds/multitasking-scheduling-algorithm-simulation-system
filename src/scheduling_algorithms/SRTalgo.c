#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/data_structures/priority_queue.h"
#include "../includes/utils/ProcessesTable.h"

Process FirstArrived(Process *array,int sizeOfArray,int indice,Process first) {
    if (sizeOfArray <= 0) {
        // Handle case when array is empty or invalid size
        // Here, returning a default Process struct
        printf("Error, the array is empty ! ");
        Process defaultProcess;
        defaultProcess.arrivalTime = -1; // Set an invalid execution time
        return defaultProcess;
    }
    Process FirstArrived = first;
    for (int i=indice;i<sizeOfArray;i++) {
        if (array[i].runTime < FirstArrived.arrivalTime){
            FirstArrived = array[i];
        }
    }
    return FirstArrived;
}

Process minExecTime(Process *array,int sizeOfArray,int indice,Process min) {
    if (sizeOfArray <= 0) {
        // Handle case when array is empty or invalid size
        // Here, returning a default Process struct
        printf("Error, the array is empty ! ");
        Process defaultProcess;
        defaultProcess.runTime = -1; // Set an invalid execution time
        return defaultProcess;
    }
    Process minimalProcess = min;
    for (int i=indice;i<sizeOfArray;i++) {
        if (array[i].runTime < minimalProcess.runTime){
            minimalProcess = array[i];
        }
    }
    return minimalProcess;
}

void switchProcesses(Process *p1, Process *p2) {
    // printf("\n Trying to switch pointers :\n");
    //  printf("\n to be switched %s :\n",p1->name);
    //  printf("\n The minimal : %s :\n",p2->name);
    Process* temp= p1;
    *p1=*p2;
    *p2=*temp;
} 

// [5,10,5,3,7,1,10,7] 
// [1,10,5,3,7,5,10,7] 
// [1,3,5,10,7,5,10,7]
// [1,3,5,*5,7,10,10,7] 
// [1,3,5,*5,*7,*7,10,10] 


Process * sortArrayByExecTime(Process * array,int sizeOfArray) {
    for (int i=0; i<sizeOfArray-1 ;i++){
        Process minimalProcess= minExecTime(array,sizeOfArray,i+1,array[i]);
        printf("\n Minimal ex time : %d \n",minimalProcess.runTime);
        if (array[i].runTime > minimalProcess.runTime){
            switchProcesses(&array[i],&minimalProcess);
            printf("\n Ancien minimal :%s \n",minimalProcess.processName);
            printf("\n Ancien non min :%s \n",array[i].processName);
        }
    }
    return array;
}

void SRT(Process* array, int sizeOfArray, PriorityQueue * pq ) {

    int current_time = 0;
    int completed_processes = 0;
    int current_process_index ;
    // Process startP= FirstArrived(array,sizeOfArray,1,array[0]);
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
    int sizeOfArray = getNbProcesses("./src/processes.txt");
    Process * array1=getTableOfProcesses("./src/processes.txt");
    // if (array1==NULL) {
    //     printf("\n Error getting the array of processes ! \n");
    //     return 0;
    // }
    // Process * arraySorted = sortArrayByExecTime(array1,sizeOfArray);
    // for(int i=0;i<sizeOfArray;i++) {
    //     printf("\n element number : %d \n",i);
    //     printf("%d",arraySorted[i].ExecTime);
    // }

    PriorityQueue * PQ = init_priority_queue(sizeOfArray,sizeof(array1)/sizeof(array1[0]),compare_int);

    return 0;
}