#include <stdio.h>
#include <stdlib.h>
#include"../includes/data_structures/queue.h"
#include "../includes/utils/ProcessesTable.h"
#include "../includes/utils/algo_result.h"

AlgoResult round_robin(Queue* queue,int processes_number,int quantum){

    if(is_empty_q(queue)){
        exit(EXIT_FAILURE);
    }

    int quantum_value = quantum;
    int t=0;
    int rotation_time=0;
    int waiting_time=0;
    int finished=0;
    AlgoResult algoResult;
    algoResult.gantt = create_gantt();
    int wasted_time = 0;
    Queue* ready_queue = create_queue();

    while(!is_empty_q(queue) || !is_empty_q(ready_queue)){

        // IDLE STATE
        while(is_empty_q(ready_queue) && t <queue->front->data.arrivalTime ){
            wasted_time++;
            enqueue_gantt(algoResult.gantt,t,NULL,0,NULL,0);
            t++;
        }

        while(!is_empty_q(queue) && t >= queue->front->data.arrivalTime){
            enqueue(ready_queue,dequeue(queue));
        }

        Process current_process = dequeue(ready_queue);

        while(quantum_value>0 && current_process.runTime>0){
            quantum_value--;
            current_process.runTime--;

            if(current_process.runTime == 0){
                finished = 1;
                int current_rotation_time= t +1 - current_process.arrivalTime;
                rotation_time+= current_rotation_time;
                waiting_time+=current_rotation_time ;
            }

            int s = size_q(ready_queue);
            enqueue_gantt(algoResult.gantt,t,current_process.processName,finished,create_array_from_queue(ready_queue,s),s);
            t++;

            while(!is_empty_q(queue) && t >= queue->front->data.arrivalTime){
                enqueue(ready_queue,dequeue(queue));
            }
        }

        if(!finished){
            enqueue(ready_queue,current_process);
        }

        quantum_value = quantum;
        finished = 0;
    }
        free_queue(ready_queue);
        waiting_time += (wasted_time -t) ;
        add_metrics(&algoResult,(float)rotation_time/processes_number,(float)waiting_time/processes_number);
        return algoResult;
}

// int main(void){
//     Process processes[10] = {
//         {"Process1", 0, 5, 3},
//         {"Process2", 1, 3, 2},
//         {"Process3", 2, 1, 4},
//         {"Process4", 4, 2, 1},
        
//     };
//     int processes_number = 4;
//     Queue* q = create_queue_from_array(processes,processes_number);
//     int quantum = 2;
//     AlgoResult result =  round_robin(q,processes_number,quantum);
//     while (!is_empty_gantt(result.gantt)){
//         InstantResultNode ins =  dequeue_gantt(result.gantt);
//         printf("t = %d \n",ins.t);
//         printf("process name = %s \n",ins.processName);
//         printf("finished = %d \n",ins.quit);
//         printf("ready queue size = %d \n",ins.readyQueueSize);
//         for(int i=0;i<ins.readyQueueSize;i++){
//             printf(ins.readyQueue[i]);
//             printf("|");
//         }
//         printf("\n");
//         printf("--------------------\n");
//     }
//     printf(" Average Waiting Time %.2f\n",result.metrics.averageWaiting);
//     printf(" Average Rotaion Time %.2f\n",result.metrics.averageRotation);
//     return 0;
// }