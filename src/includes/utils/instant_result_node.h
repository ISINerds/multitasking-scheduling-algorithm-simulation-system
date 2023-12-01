#pragma once
typedef struct InstantResultNode{
    int t;
    char* processName; // null <=> idle state
    char** readyQueue;
    int readyQueueSize;
    int quit; // 1 = finished
    struct InstantResultNode* next;
}InstantResultNode;