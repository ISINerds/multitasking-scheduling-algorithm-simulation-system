#pragma once
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


typedef struct Conf {
    int number_of_processes;          // Number of processes configuration
    int arrival_time_lower_bound;     // Lower bound for arrival time configuration
    int arrival_time_upper_bound;     // Upper bound for arrival time configuration
    int runtime_lower_bound;          // Lower bound for runtime configuration
    int runtime_upper_bound;          // Upper bound for runtime configuration
    int priority_classes;             // Priority classes configuration
} Conf;


int random_between(int min, int max); // Generates a random number within the given interval. Before calling this function, ensure to call srand(time(NULL)) to change the seed.
Conf read_configuration_file(const char* file_path); // Reads the configuration file and returns a struct containing the configuration.
void generate_processes_file(const char* configuration_file_path,const char*file_name, const char seperator); // Generates the processes file.


int random_between(int min, int max){
    if(max>RAND_MAX){
        fprintf(stderr,"[Error]: maximum value exceeds RAND_MAX = %d\n",RAND_MAX);
        return -1;
    }
    return min+rand()%(max-min+1);
}


Conf read_configuration_file(const char* file_path){

    FILE *file=fopen(file_path,"r");

    Conf configuration = {0};

    if(file==NULL){
        fprintf(stderr,"[Error]: error opening the configuration file\n");
        return configuration;
    }

    char line[100];
    while(fgets(line,sizeof(line),file)!=NULL){
        char key[50];
        int val;
        sscanf(line,"%[^=]=%d",key,&val);
        if(strcmp(key,"number_of_processes")==0){
            configuration.number_of_processes=val;
        }else if(strcmp(key,"arrival_time_lower_bound")==0){
            configuration.arrival_time_lower_bound=val;
        }else if(strcmp(key,"arrival_time_upper_bound")==0){
            configuration.arrival_time_upper_bound=val;
        }else if(strcmp(key,"runtime_lower_bound")==0){
            configuration.runtime_lower_bound=val;
        }else if(strcmp(key,"runtime_upper_bound")==0){
            configuration.runtime_upper_bound=val;
        }else if(strcmp(key,"priority_classes")==0){
            configuration.priority_classes=val;
        }else{
            fprintf(stderr,"[Error]: Invalid key : %s",key);
        }
    }
    fclose(file);
    return configuration;
}

void generate_processes_file(const char* configuration_file_path,const char*file_name, const char seperator){
    srand(time(NULL));
    Conf configuration=read_configuration_file(configuration_file_path);
    FILE * file = fopen(file_name,"w");
    fprintf(file,"%d\n",configuration.number_of_processes);
    for(size_t i=0;i<configuration.number_of_processes;i++){
        const int arrival_time=random_between(configuration.arrival_time_lower_bound,configuration.arrival_time_upper_bound);
        const int run_time=random_between(configuration.runtime_lower_bound,configuration.runtime_upper_bound);
        const int priority=random_between(1,configuration.priority_classes);
        if(i<configuration.number_of_processes-1)fprintf(file,"processus%ld%c%d%c%d%c%d\n",i,seperator,arrival_time,seperator,run_time,seperator,priority);
        else fprintf(file,"processus%ld%c%d%c%d%c%d",i,seperator,arrival_time,seperator,run_time,seperator,priority);
    }
    fclose(file);
}

