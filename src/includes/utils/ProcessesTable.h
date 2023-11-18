#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

// This C program will be :
// 1- parsing the processes file
// 2- Generate a static table of a convenient format of the Process details


int getNbProcesses() {
    FILE *fPointer= fopen("test.txt","r");
    char content[255];

    if (fPointer != NULL ) {
            //Getting the number of processes
            fgets(content,255,fPointer);
            return atoi(content);
    }
    else {
        printf("Error opening the processes file");
        return 0;
    }
}

// struct Process {
//     char name[10];
//     int arrivalTime ;
//     int ExecTime;
//     int priority;
// };

//The function that tokenizes a string and return a Process struct.
void tokenize (char ch[255],char del[], Process *P) {
    ch[strcspn(ch, "\n")] = '\0';

    char *token;

    //an integer to determine which Process Attribute we are parsing;
    int nb=1;

    // Tokenize the string using strtok
    token = strtok(ch, del);

    while (token != NULL) {
        if (nb==1){
            strcpy(P->processName,token);
        }
        if (nb==2){
            P->arrivalTime=atoi(token);
        }
        if (nb==3){
            P->runTime=atoi(token);
        }
        if (nb==4){
            P->priority=atoi(token);
            break;
        }
        nb++;
        token = strtok(NULL, del);
    }

}


Process *getTableOfProcesses() {
    //Choosing the seperator character 
    char sep[] = ";";

    //The size of the processes table
    int sizeTable;
    int i=0;

    FILE *fPointer= fopen("test.txt","r");
    char content[255];

    if (fPointer != NULL ) {
            printf("Lecture of the processes file is successfull ! \n");
            //Getting the number of processes
            fgets(content,255,fPointer);
            sizeTable = atoi(content);
            Process* processes = (Process*)malloc(sizeTable * sizeof(Process));
            //printf("Size of the table is : %d \n",sizeTable);

            while(!feof(fPointer) && i < sizeTable){
                fgets(content,255,fPointer);
                tokenize(content, sep, &processes[i]);
                //processes[i]=p;
                i++;
            }
            fclose(fPointer);

            //Testinng part
            //printf("%d",processes[0].priority);
            return processes;
    }
    else {
        printf("Error opening the processes file");
        return NULL;
    }
}

