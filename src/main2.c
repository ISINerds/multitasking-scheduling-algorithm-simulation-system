#include "./includes/utils/dynamic_menu.h"
#include "./includes/utils/processes_generator.h"
#include "./includes/utils/ProcessesTable.h"
#include "./includes/utils/algo_result.h"
#define min(a,b) (((a) < (b)) ? (a) : (b))
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

typedef struct {
    int x;
    int y;
    int width;
    int height;
    Color color;
} RectangleData;
int ReadyQrectangleWidth = 70;
int ReadyQrectangleHeight = 50;

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
char algoOptions[];
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
    int ww=boundry.width/5;
    int hh=boundry.height/8;
    int rectww=boundry.width/4;
    int recthh=boundry.height/8;
    int subTitleTextFront = boundry.width/20;


    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Stats",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);

    // ----------------
    DrawTextEx(font,"Current job",(Vector2){boundry.x+textPadding, boundry.y+boundry.height/6},subTitleTextFront+subTitleTextFront*0.2,0,BLACK);
    if(ganttSize!=0){
        if(currNode.readyQueueSize!=0){
            for(int i=0;i<min(4,currNode.readyQueueSize);i++){
                Color c = BLACK;
                if(currNode.readyQueue[i][1] - '0' != -46) {
                    // != null (idle state)
                    c = colors[currNode.readyQueue[i][1] - '0'];
                }
                DrawRectangleRounded((Rectangle){
                    .x=boundry.x+textPadding+(boundry.width*.2)*i,
                    .y=boundry.y+boundry.height/4,
                    .width=50,
                    .height=boundry.height/6,
                },0.1,20,c);
                DrawTextEx(font,currNode.readyQueue[i],(Vector2){boundry.x+textPadding+(boundry.width*.2)*i+25, boundry.y+boundry.height/4+boundry.height/12},subTitleTextFront,0,BLACK);
            }
        }
    }
    // ----------------
    // DrawTextEx(font,currNode.readyQueueSize!=0?currNode.readyQueue[0]:"",(Vector2){boundry.x+boundry.width/2,boundry.y+boundry.height/2},textSize,0,RED);
    // ------------------
    DrawTextEx(font,"Current job",(Vector2){boundry.x+boundry.width/8+abs(ww-rectww)/2, boundry.y+2*boundry.height/4-hh/2},subTitleTextFront,0,BLACK);
    DrawRectangle(boundry.x+boundry.width/8+abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh,WHITE);
    DrawRectangleLinesEx((Rectangle){boundry.x+boundry.width/8+abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh},3, BLACK);
    if(ganttSize!=0)DrawTextEx(font,currNode.processName,(Vector2){boundry.x+boundry.width/8+abs(ww-rectww)/2+ww/2-subTitleTextFront/2, boundry.y+2*boundry.height/4+hh/2-subTitleTextFront/2},subTitleTextFront,0,BLACK);

    //GuiIconText(ICON_REPEAT,"") ;
    // DrawTextEx(font,"Current time",(Vector2){boundry.x+2.2*boundry.width/3, boundry.y+2.2*ReadyQrectangleHeight},subTitleTextFront,0,BLACK);
    DrawTextEx(font,"Current time",(Vector2){boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2, boundry.y+2*boundry.height/4-hh/2},subTitleTextFront,0,BLACK);
    DrawRectangle(boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh,WHITE);
    DrawRectangleLinesEx((Rectangle){boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh},3, BLACK);
    // ADD the time here
    char ss[20];
    sprintf(ss,"%d",ganttRectanglesSize);
    DrawTextEx(font,ss,(Vector2){boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2+ww/2-subTitleTextFront/2, boundry.y+2*boundry.height/4+hh/2-subTitleTextFront/2},subTitleTextFront,0,BLACK);


    // DrawTextEx(font,"Average rotation time",(Vector2){boundry.x+boundry.width/4.2, boundry.y+3.7*ReadyQrectangleHeight},subTitleTextFront,0,BLACK);
    DrawTextEx(font,"Avg rotation time",(Vector2){boundry.x+boundry.width/8, boundry.y+3*boundry.height/4-hh/2},subTitleTextFront,0,BLACK);
    DrawRectangle(boundry.x+boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh,WHITE);
    DrawRectangleLinesEx((Rectangle){boundry.x+boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh},3, BLACK);
    if(ganttRectanglesSize == ganttSize && ganttSize!=0){
        char sss[20];
        sprintf(sss,"%.2f",algoResult.metrics.averageRotation);
        DrawTextEx(font,sss,(Vector2){boundry.x+boundry.width/8+ww/2-subTitleTextFront/2, boundry.y+3*boundry.height/4+hh/2-subTitleTextFront/2},subTitleTextFront,0,BLACK);
    }else{
        DrawTextEx(font,"",(Vector2){boundry.x+boundry.width/8+ww/2-subTitleTextFront/2, boundry.y+3*boundry.height/4+hh/2-subTitleTextFront/2},subTitleTextFront,0,BLACK);
    }

    DrawTextEx(font,"Avg waiting time",(Vector2){boundry.x+boundry.width-rectww-boundry.width/8, boundry.y+3*boundry.height/4-hh/2},subTitleTextFront,0,BLACK);
    DrawRectangle(boundry.x+boundry.width-rectww-boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh,WHITE);
    DrawRectangleLinesEx((Rectangle){boundry.x+boundry.width-rectww-boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh},3, BLACK);
    if(ganttRectanglesSize == ganttSize && ganttSize!=0){
        char sss[20];
        sprintf(sss,"%.2f",algoResult.metrics.averageWaiting);
        DrawTextEx(font,sss,(Vector2){boundry.x+boundry.width-rectww-boundry.width/8+ww/2-subTitleTextFront/2, boundry.y+3*boundry.height/4+hh/2-subTitleTextFront/2},subTitleTextFront,0,BLACK);
    }else{
        DrawTextEx(font,"",(Vector2){boundry.x+boundry.width-rectww-boundry.width/8+ww/2-subTitleTextFront/2, boundry.y+3*boundry.height/4+hh/2-subTitleTextFront/2},subTitleTextFront,0,BLACK);
    }

    // -------------------
}
void render_menu(Rectangle boundry){
    // printf("from menu\n");
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Menu",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
    DrawTextEx(font,"Scheduling Algorithms",(Vector2){boundry.x+textPadding, boundry.y+3*textPadding},textSize,0,RED);
    if(GuiButton((Rectangle){boundry.x+textPadding,boundry.y+18*textPadding,boundry.width - 20,30},"Start")&&!algosDropDown1EditMode) {
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
    if(GuiButton((Rectangle){boundry.x+textPadding,boundry.y+22*textPadding,boundry.width - 20,30},"Reset")&&!algosDropDown1EditMode) {
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
    if(GuiDropdownBox((Rectangle){boundry.x+textPadding, boundry.y+6*textPadding,boundry.width-20,25}, algoOptions, &selectedAlgoIndex, algosDropDown1EditMode)) {
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
        strcat(algoOptions,algorithms[i].name);
        if(i<numberOfAlgo-1)strcat(algoOptions,";");
    }
    printf("%s\n", algoOptions);
    while (!WindowShouldClose()){
        preview_screen();
        currFrame++;
        currFrame%=60;
    }
    CloseWindow();
    return 0;
}