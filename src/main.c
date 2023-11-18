#include <stdio.h>
#include <stdlib.h>
#include "./includes/utils/processes_generator.h"
#include "./includes/utils/ProcessesTable.h"
int main(void) {
	generate_processes_file("./src/config.conf", "./src/processes.txt", ';');
    int nb = getNbProcesses("./src/processes.txt");
    // printf("%d\n", nb);
    Process* processes = getTableOfProcesses("./src/processes.txt");
    // for(int i=0;i<nb;i++) {
    //     printf("%s %d %d %d\n", processes[i].processName, processes[i].arrivalTime, processes[i].runTime, processes[i].priority);
    // }
    freeProcesses();
   return 0;
}

