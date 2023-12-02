#include <stdio.h>
#include <stdlib.h>
#include "./includes/utils/processes_generator.h"
#include "./includes/utils/ProcessesTable.h"
#include "./includes/utils/dynamic_menu.h"
#include "./includes/data_structures/queue.h"

int main(void) {
    int nbAlgorithms = get_nb_algorithms("./build/algorithms");
    printf("%d \n",nbAlgorithms);
    Algorithm* algorithms = load_all_algorithms("./build/algorithms");
    display_menu(algorithms,nbAlgorithms);
    int choice;
    do{
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= nbAlgorithms) {
                Process processes[] = {
        {"Process1", 0, 5, 3},
        {"Process2", 1, 3, 2},
        {"Process3", 2, 1, 4},
        {"Process4", 4, 2, 1},
        
                };
            int processes_number = 4;
            Queue* q = create_queue_from_array(processes,processes_number);
            algorithms[choice-1].run(q,processes_number,0);
        } else {
            printf("Invalid choice\n");
        }
    } while(choice !=0 );

    unload_all_algorithms(algorithms, nbAlgorithms);
    return EXIT_SUCCESS;
}

