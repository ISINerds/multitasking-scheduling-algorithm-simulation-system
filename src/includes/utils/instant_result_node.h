#pragma once
typedef struct InstantResultNode{
    int t;
    char* processName;
    char** readyQueue;
    int readyQueueSize;
    int quit;
    struct InstantResultNode* next;
}InstantResultNode;