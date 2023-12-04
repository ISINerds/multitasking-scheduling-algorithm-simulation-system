#include "./includes/utils/dynamic_menu.h"
#include "./includes/utils/processes_generator.h"
#include "./includes/utils/ProcessesTable.h"
#include "./includes/utils/algo_result.h"

#define RAYGUI_IMPLEMENTATION
#include "../build_raylib/_deps/raygui-src/src/raygui.h"
#include "raylib.h"

#define textPadding 10
#define borderRadius 0.1
#define gap 5
#define padding 5
#define textSize 30
#define containerColor (Color){ 220, 220, 220, 255 }
#define backgroundColor (Color){255,255,255,1}
Font font;

Algorithm* algorithms = NULL;
Process* processes = NULL;
AlgoResult algoResult;
InstantResultNode currNode;
int numberOfAlgo = 0;
int started = 0;
int currFrame = 0;
Rectangle* ganttRectangles = NULL;
int ganttRectanglesSize = 0;

void render_job_pool(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Job pool",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
}
void render_gantt(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Gantt",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
    if(started && currFrame%60==0){
        currNode = dequeue_gantt(algoResult.gantt);
        ganttRectanglesSize++;
        ganttRectangles[ganttRectanglesSize-1]=(Rectangle){
            .x=boundry.x+100+60*(ganttRectanglesSize-1),
            .y=boundry.y+50,
            .width=50,
            .height=100,
        };
        if(is_empty_gantt(algoResult.gantt)){
            started = 0;
        }
    }
    for(int i=0;i<ganttRectanglesSize;i++){
        DrawRectangleRounded(ganttRectangles[i],0.1,20,RED);
    }

}
void render_stats(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Stats",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
    DrawTextEx(font,currNode.readyQueueSize!=0?currNode.readyQueue[0]:"",(Vector2){boundry.x+boundry.width/2,boundry.y+boundry.height/2},textSize,0,RED);
}
void render_menu(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Menu",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
    int isStartButtonPressed = GuiButton((Rectangle){boundry.x+(boundry.width)/2,boundry.y+(boundry.height)/2,100,100},"Start");
    if(isStartButtonPressed && !started){
        Process processes[] = {
            {"Process1", 0, 5, 3},
            {"Process2", 1, 3, 2},
            {"Process3", 2, 1, 4},
            {"Process4", 4, 2, 1},
        };
        int processes_number = 4;
        Queue* q = create_queue_from_array(processes,processes_number);
        printf("%s\n",algorithms[1].name);
        algoResult = algorithms[1].run(q,processes_number,1);
        started = 1;
        int queueSize = size_gantt(algoResult.gantt);
        printf("%d\n",queueSize);
        ganttRectangles = (Rectangle*) malloc(queueSize*sizeof(Rectangle));
    }
}
void preview_screen(void){
    int w = GetRenderWidth();
    int h = GetRenderHeight();
    ClearBackground(backgroundColor);
    BeginDrawing(); 
    Rectangle jobPoolRect = {
        .x = 0+padding,
        .y = 0+padding,
        .width = (w-2*padding-2*gap)/3,
        .height = 2*(h-gap)/3,
    };
    render_job_pool(jobPoolRect);
    Rectangle menuRect = {
        .x = 2*gap+padding+2*(w-2*padding-2*gap)/3,
        .y = 0+padding,
        .width = (w-2*padding-2*gap)/3,
        .height = 2*(h-gap)/3,
    };
    render_menu(menuRect);
    Rectangle statsRect = {
        .x = gap+padding+(w-2*padding-2*gap)/3,
        .y = 0+padding,
        .width = (w-2*padding-2*gap)/3,
        .height = 2*(h-gap)/3,
    };
    render_stats(statsRect);
    Rectangle ganttRect = {
        .x = padding,
        .y = 2*h/3 + gap,
        .width = w-2*padding,
        .height = (h-gap)/3 - padding,
    };
    render_gantt(ganttRect);
    EndDrawing();

}
int main(void){
    const int screenWidth = 900;
    const int screenHeight = 500;
    SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Multitasking Scheduling Algorithm Simulation System");
    font = LoadFont("../assets/fonts/DelaGothicOne-Regular.ttf");
    SetTargetFPS(60);

    numberOfAlgo = get_nb_algorithms("../build/algorithms");
    algorithms = load_all_algorithms("../build/algorithms");
    while (!WindowShouldClose()){
        // printf("%d\n",started);
        preview_screen();
        currFrame++;
        currFrame%=60;
    }
    CloseWindow();
    return 0;
}