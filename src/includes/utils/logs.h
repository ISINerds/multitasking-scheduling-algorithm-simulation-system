#pragma once
#include <stdio.h>
#include "./instant_result_node.h"
#include "./metrics.h"
// ANSI escape codes for text colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void print_ready_queue(const char* readyQueue[], int size) {
    printf(ANSI_COLOR_CYAN "Ready Queue:" ANSI_COLOR_RESET);
    if (size == 0) {
        printf(" Empty");
    } else {
        int i = 0;
        for (; i < size - 1; i++) {
            printf(" %s |", readyQueue[i]);
        }
        if(i < size) printf(" %s", readyQueue[i]);
    }
    printf("\n");
}

void execution_log(InstantResultNode state) {
    if (state.processName == NULL) {
        printf(ANSI_COLOR_RED "CPU IS IDLE from %d to %d\n" ANSI_COLOR_RESET, state.t, state.t + 1);
    } else {
        printf(ANSI_COLOR_GREEN "%s is executing from %d to %d\n" ANSI_COLOR_RESET, state.processName, state.t, state.t + 1);
        print_ready_queue(state.readyQueue, state.readyQueueSize);

        if (state.quit) {
            printf(ANSI_COLOR_YELLOW "%s has terminated its job and left the CPU at %d\n" ANSI_COLOR_RESET, state.processName, state.t + 1);
        }
    }
    printf("--------------------\n");
    // #ifdef _WIN32
    //     Sleep(1000);  // Sleep function in Windows takes milliseconds
    // #else
    //     sleep(1);     // sleep function in Unix-like systems takes seconds
    // #endif
}

void metrics_log(Metrics metrics) {
    // Print Average Waiting Time in green
    printf(ANSI_COLOR_GREEN "Average Waiting Time: %.2f\n" ANSI_COLOR_RESET, metrics.averageWaiting);

    // Print Average Rotation Time in yellow
    printf(ANSI_COLOR_YELLOW "Average Rotation Time: %.2f\n" ANSI_COLOR_RESET, metrics.averageRotation);
    printf("--------------------\n");
    // #ifdef _WIN32
    //     Sleep(1000);  // Sleep function in Windows takes milliseconds
    // #else
    //     sleep(1);     // sleep function in Unix-like systems takes seconds
    // #endif
    
}