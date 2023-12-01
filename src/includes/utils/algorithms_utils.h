#pragma once
void print_seperator(int t,int processes_number);
void print_process(Process process);
void idle_state(int processes_number);

// ------------------ Print the current time in seconds and seperate between the processes
void print_seperator(int t,int processes_number){
    char string[10];  
    sprintf(string, "%d", processes_number);
    for(int i=0;i<9+strlen(string);i++) printf("-");
    printf("%ds\n",t);
}
// ------------------ Print the name of the current process 
void print_process(Process process){
        printf("|%s\n",process.processName);
}
// ------------------ Print nothing when the processer in idle state
void idle_state(int processes_number){
    char string[10];  
    sprintf(string, "%d", processes_number);
    for(int i=0;i<9+strlen(string);i++) printf("|");
    printf("\n");
}