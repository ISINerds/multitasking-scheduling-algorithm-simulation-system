#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/process.h"

// This C program will be :
// 1- parsing the processes file
// 2- Generate a static table of a convenient format of the Process details

struct processesTable {
    int processesNumber;
    Process* processes;
} processesTable = {-1, NULL};


int getNbProcesses(const char* file_path);
Process tokenize (char ch[255],char del[]);
Process* getTableOfProcesses(const char* file_path);
void freeProcesses();

int getNbProcesses(const char* file_path) {
    // if(processesTable.processesNumber != -1) return processesTable.processesNumber;
    FILE *fPointer= fopen(file_path,"r");
    char content[255];

    if (fPointer != NULL ) {
        //Getting the number of processes
        fgets(content,255,fPointer);
        return processesTable.processesNumber = atoi(content);
    }
    else {
        printf("Error opening the processes file");
        return 0;
    }
    fclose(fPointer);
}


//The function that tokenizes a string and return a Process struct.
Process tokenize (char ch[255],char del[]) {
    Process result;
    ch[strcspn(ch, "\n")] = '\0';

    char *token;

    //an integer to determine which Process Attribute we are parsing;
    int nb=1;

    // Tokenize the string using strtok
    token = strtok(ch, del);

    while (token != NULL) {
        if (nb==1){
            result.processName = strdup(token);
        }
        if (nb==2){
            result.arrivalTime = atoi(token);
        }
        if (nb==3){
            result.runTime = atoi(token);
        }
        if (nb==4){
            result.priority = atoi(token);
            break;
        }
        nb++;
        token = strtok(NULL, del);
    }
    return result;
}


Process *getTableOfProcesses(const char* file_path) {
    // if(processesTable.processes != NULL) return processesTable.processes;

    //Choosing the seperator character 
    char sep[] = ";";

    //The size of the processes table
    int sizeTable;
    int i=0;

    FILE *fPointer= fopen(file_path,"r");
    char content[255];

    if (fPointer != NULL ) {
            //Getting the number of processes
            fgets(content,255,fPointer);
            sizeTable = atoi(content);
            Process* processes = (Process*)malloc(sizeTable * sizeof(Process));
            //printf("Size of the table is : %d \n",sizeTable);

            while(!feof(fPointer) && i < sizeTable){
                fgets(content,255,fPointer);
                // tokenize(content, sep, &processes[i]);
                processes[i]=tokenize(content,sep);
                i++;
            }
            fclose(fPointer);
            return processesTable.processes = processes;
    }
    else {
        printf("Error opening the processes file");
        return NULL;
    }
    fclose(fPointer);
}

void freeProcesses() {
    for (int i = 0; i < processesTable.processesNumber; i++) {
        free(processesTable.processes[i].processName);
    }
    free(processesTable.processes);
}