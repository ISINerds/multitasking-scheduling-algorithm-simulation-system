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
AlgoResult algoResult = {NULL, {0, 0}};
InstantResultNode currNode;
int numberOfAlgo = 0;
int started = 0;
int currFrame = 0;
InstantResultNode* ganttRectangles = NULL;
int ganttRectanglesSize = 0;
int ganttSize = 0;

//----------------
//algosdropdown
int selectedAlgoIndex = 0;
bool algosDropDown1EditMode = false;
//-----------
//quantumslider
int quantumValue = 0;
bool quantumSpinnerEditMode = false;
//----------
//startbutton
//----------
int isStartButtonPressed = false;
//---------------
//TODO return the names of all algorithms like this "algo1;algo2;algo3;algo4"
char* algosList;

Color colors[10] = {RED, MAROON, GREEN, BLUE, ORANGE, GRAY, SKYBLUE, MAGENTA, GOLD, YELLOW};

void render_job_pool(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Job pool",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
}
void render_gantt(Rectangle boundry){
    // printf("from gantt\n");
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Gantt",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
    if(isStartButtonPressed && algoResult.gantt &&  is_empty_gantt(algoResult.gantt)) {
        isStartButtonPressed = false;
        free(algoResult.gantt);
        algoResult.gantt = NULL;
        printf("not pressed\n");
    }
    if(isStartButtonPressed && algoResult.gantt && currFrame%5==0){
        currNode = dequeue_gantt(algoResult.gantt);
        ganttRectanglesSize++;
        ganttRectangles[ganttRectanglesSize-1]=currNode;
    }
    for(int i=0;i<ganttRectanglesSize;i++){
        char name[50];
        snprintf(name, sizeof(name), ganttRectangles[i].processName);
        // printf("%d\n", name[1]-'0');
        Color c = BLACK;
        if(name[1] - '0' != -46) {
            // != null (idle state)
            c = colors[name[1] - '0'];
        }
        DrawRectangleRounded((Rectangle){
            .x=boundry.x+textPadding+((boundry.width - 20 - ganttSize + 1) / ganttSize + 1)*i,
            .y=boundry.y+50,
            .width=(boundry.width - 20 - ganttSize + 1)/ganttSize,
            .height=100,
        },0.1,20,c);
    }

}
void render_stats(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Stats",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
    DrawTextEx(font,currNode.readyQueueSize!=0?currNode.readyQueue[0]:"",(Vector2){boundry.x+boundry.width/2,boundry.y+boundry.height/2},textSize,0,RED);
}
void render_menu(Rectangle boundry){
    // printf("from menu\n");
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Menu",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
    DrawTextEx(font,"Scheduling Algorithms",(Vector2){boundry.x+textPadding, boundry.y+3*textPadding},textSize,0,RED);
    if(GuiButton((Rectangle){boundry.x+textPadding,boundry.y+18*textPadding,boundry.width - 20,30},"Start")) {
        if(!isStartButtonPressed) {
            ganttSize = 0;
            ganttRectanglesSize = 0;
            if(ganttRectangles) {
                printf("ganttrectangles are being freed\n");
                free(ganttRectangles);
                ganttRectangles = NULL;
                printf("----------------\n");
            }
            printf("pressed\n");
            printf("selected algo: %d his name is %s quantum is %d\n", selectedAlgoIndex, algorithms[selectedAlgoIndex].name, quantumValue);
            int processes_number = getNbProcesses("./processes.txt");
            Process* processes = getTableOfProcesses("./processes.txt");
            Queue* q = create_queue_from_array(processes,processes_number);
            printf("I about to run the algo \n");

            algoResult = algorithms[selectedAlgoIndex].run(q, processes_number, quantumValue);
            printf("%f %f\n", algoResult.metrics.averageRotation, algoResult.metrics.averageWaiting);
            ganttSize = size_gantt(algoResult.gantt);
            printf("gantt size = %d\n",ganttSize);
            ganttRectangles = (InstantResultNode*) malloc(ganttSize*sizeof(InstantResultNode));
            isStartButtonPressed = true;
            // freeProcesses(); //if you uncomment this line, then the processname in the readyqueue will be random and not understandable
            
        }
    }
    if(GuiButton((Rectangle){boundry.x+textPadding,boundry.y+22*textPadding,boundry.width - 20,30},"Reset")) {
        if(!isStartButtonPressed) {
            //generate processes
            generate_processes_file("config.conf","processes.txt",';');
            printf("new file generated\n");
            ganttSize = 0;
            ganttRectanglesSize = 0;
            if(ganttRectangles) {
                printf("ganttrectangles are being freed\n");
                free(ganttRectangles);
                ganttRectangles = NULL;
                printf("----------------\n");
            }
        }
    }
    
    DrawTextEx(font,"Quantum",(Vector2){boundry.x+textPadding, boundry.y+8*textPadding},textSize,0,RED);
    GuiSpinner((Rectangle){ boundry.x+textPadding, boundry.y+12*textPadding, boundry.width - 20, 30 }, NULL, &quantumValue, 1, 10, quantumSpinnerEditMode);


    
    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 4);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    if(GuiDropdownBox((Rectangle){boundry.x+textPadding, boundry.y+6*textPadding,boundry.width-20,25}, "FIFO;multilevel;sjf;round_robin", &selectedAlgoIndex, algosDropDown1EditMode)) {
         algosDropDown1EditMode = !algosDropDown1EditMode;
         if(!algosDropDown1EditMode && !isStartButtonPressed) {
         }
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
    // SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Multitasking Scheduling Algorithm Simulation System");
    font = LoadFont("../assets/fonts/DelaGothicOne-Regular.ttf");
    SetTargetFPS(60);
    numberOfAlgo = get_nb_algorithms("../build/algorithms");
    algorithms = load_all_algorithms("../build/algorithms");
    for(int i=0;i<numberOfAlgo;i++) {
        printf("%s\n", algorithms[i].name);
    }
    while (!WindowShouldClose()){
        preview_screen();
        currFrame++;
        currFrame%=60;
    }
    CloseWindow();
    return 0;
}