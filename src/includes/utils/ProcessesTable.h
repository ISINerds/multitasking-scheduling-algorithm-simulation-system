#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/process.h"


struct processesTable {
    int processesNumber;
    Process* processes;
} processesTable = {-1, NULL};


int getNbProcesses(const char* file_path);
Process tokenize (char ch[255],char del[]);
Process* getTableOfProcesses(const char* file_path);
void freeProcesses();

int getNbProcesses(const char* file_path) {
    FILE *fPointer= fopen(file_path,"r");
    char content[255];

    if (fPointer != NULL ) {
        fgets(content,255,fPointer);
        return processesTable.processesNumber = atoi(content);
    }
    else {
        printf("Error opening the processes file");
        return 0;
    }
    fclose(fPointer);
}


Process tokenize (char ch[255],char del[]) {
    Process result;
    ch[strcspn(ch, "\n")] = '\0';

    char *token;

    int nb=1;

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
    char sep[] = ";";

    int sizeTable;
    int i=0;

    FILE *fPointer= fopen(file_path,"r");
    char content[255];

    if (fPointer != NULL ) {
            fgets(content,255,fPointer);
            sizeTable = atoi(content);
            Process* processes = (Process*)malloc(sizeTable * sizeof(Process));

            while(!feof(fPointer) && i < sizeTable){
                fgets(content,255,fPointer);
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

void freeProcesses(Process* processes, int nbProcesses) {
    for (int i = 0; i < nbProcesses; i++) {
        free(processes[i].processName);
    }
    free(processes);
}