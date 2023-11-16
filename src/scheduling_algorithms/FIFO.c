#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../includes/data_structures/queue.h"
#include "../includes/utils/algorithms_utils.h"
// ------------------ Fist In First Out Algorithm
void FIFO(Queue* queue,int processes_number){
    int t=0;
    int rotation_time=0;
    int waiting_time=0;
    while(!is_empty(queue)){
        Process current_process = dequeue(queue);
        //  In case where there is no waiting process at that current time (t) ( process in idle state) : we increment t
        if(t<current_process.arrivalTime){
            print_seperator(t,processes_number);
            idle_state(processes_number);
            while(t<current_process.arrivalTime) t++;
        }
        print_seperator(t,processes_number);
        print_process(current_process);
        // Run the current process
        t+=current_process.runTime;
        // Calculate the rotation and waiting time
        int current_rotation_time= t - current_process.arrivalTime;
        rotation_time+= current_rotation_time;
        waiting_time+=(current_rotation_time - current_process.runTime);
    }
        print_seperator(t,processes_number);
        printf("Average rotation time : %.2fs\n" , (float)rotation_time/processes_number);
        printf("Average waiting time : %.2fs\n" , (float)waiting_time/processes_number);

    }
// Example of FIFO
int main(void){

    Process processes[] = {
        {"Process1", 0, 5, 3},
        {"Process2", 1, 3, 2},
        {"Process3", 2, 1, 4},
        {"Process4", 4, 2, 1},
    };
    int processes_number = 4;
    Queue* q = create_queue_from_array(processes,processes_number);
    FIFO(q,processes_number);
    return 0;
}

