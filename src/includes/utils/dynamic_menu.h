#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>
#include "algo_result.h"


typedef struct Algorithm {
    void* handle;
    AlgoResult (*run)(Queue* queue, int processNumber, int quantum);
    char name[50];
} Algorithm;


int count_nb_algorithms(const char *dirPath){
    int nb_algorithms=0;
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        printf("[ERROR]: Could not open directory");
        return EXIT_FAILURE;
    }
    while ((struct dirent *)(readdir(dir))!=NULL){
        nb_algorithms++;
    }
    return nb_algorithms;

}

int get_nb_algorithms(const char *dirPath){

    char command[100];
    snprintf(command,sizeof(command),"ls -1 %s | wc -l > temp_file.txt", dirPath);
    system(command);

    int nbAlgorithms;
    FILE *fp = fopen("temp_file.txt", "r");
    if (fp == NULL) {
        printf("[ERROR]: opening temporary file ");
        return EXIT_FAILURE;
    }

    fscanf(fp, "%d", &nbAlgorithms);
    fclose(fp);

    remove("temp_file.txt"); 

    return nbAlgorithms;
}

Algorithm* load_all_algorithms(const char *dirPath){

    int nbAlgorithms = get_nb_algorithms(dirPath);
    Algorithm* algorithms = malloc(nbAlgorithms * sizeof(Algorithm));
    DIR *dir = opendir(dirPath);
    struct dirent *entry;
    int i=0;
    
    while ((entry = readdir(dir)) != NULL && i<nbAlgorithms) {
        
        char filename[100];
        snprintf(filename, sizeof(filename), "%s/%s", dirPath, entry->d_name);
        
        algorithms[i].handle = dlopen(filename, RTLD_LAZY);  

        if (algorithms[i].handle != NULL) {
            // Load the function to execute 
            algorithms[i].run = dlsym(algorithms[i].handle, entry->d_name);

            // Extract algorithm name from filename
            strcpy(algorithms[i].name, entry->d_name);
            
            i++;
        }
    }
    closedir(dir);
    return algorithms;
    
}


void display_menu(Algorithm* algorithms, int nbAlgorithms){

    if (nbAlgorithms == 0) {
        printf("[ERROR]: No algorithms found.\n");
        return EXIT_FAILURE;
    }

    // Display menu based on loaded algorithms
    printf("Available algorithms:\n");
    for (int i = 0; i < nbAlgorithms; i++) {
        printf("%d- %s\n", i + 1, algorithms[i].name);
    }
    printf("0- exit\n");

}

void unload_all_algorithms(Algorithm* algorithms, int nbAlgorithms){
    for (int i = 0; i < nbAlgorithms; i++) {
        dlclose(algorithms[i].handle);
    }
    free(algorithms);
}

int main() {
    
    int nbAlgorithms = get_nb_algorithms("./build/algorithms");
    Algorithm* algorithms = load_all_algorithms("./build/algorithms");
    display_menu(algorithms,nbAlgorithms);

    int choice;
    do{
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= nbAlgorithms) {
            //TODO: call the run fct accordingly 
            
        } else {
            printf("Invalid choice\n");
        }
    } while(choice ==0 );

    unload_all_algorithms(algorithms, nbAlgorithms);
    return EXIT_SUCCESS;
}
