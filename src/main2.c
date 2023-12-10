#include "./includes/utils/dynamic_menu.h"
#include "./includes/utils/processes_generator.h"
#include "./includes/utils/ProcessesTable.h"
#include "./includes/utils/algo_result.h"
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define RAYGUI_IMPLEMENTATION
#include "../build_raylib/_deps/raygui-src/src/raygui.h"
#include "raylib.h"
#include "../assets/styles/style_terminal.h"

#define textPadding 10
#define borderRadius 0.1
#define gap 5
#define padding 5
#define textSize 30
#define containerColor (Color){ 220, 220, 220, 255 }
#define backgroundColor (Color){13,2,8,1}
Font font;


int ReadyQrectangleWidth = 70;
int ReadyQrectangleHeight = 50;

Algorithm* algorithms = NULL;
Process* processes = NULL;
int processes_number = 0;
AlgoResult algoResult = {NULL, {0, 0}};
InstantResultNode currNode;
int numberOfAlgo = 0;
int started = 0;
int currFrame = 0;
float delay = 5;
InstantResultNode* ganttRectangles = NULL;
int ganttRectanglesSize = 0;
int ganttSize = 0;
char algoOptions[100];
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

void render_job_pool(Rectangle boundry){

    // DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    
    DrawRectangleRoundedLines(boundry,borderRadius,2,2,ColorFromHSV(120,1,1));
    DrawText("Job pool",boundry.x+textPadding, boundry.y+textPadding,textSize,ColorFromHSV(135,1,1));

    int padding_x = 20;
    int padding_y_top = 60;
    int padding_y_bottom = 20;
    int padding_y = padding_y_bottom + padding_y_top;

    float width = boundry.width - 2*padding_x;
    float height = boundry.height - padding_y;

    BeginScissorMode(boundry.x, boundry.y + padding_y_top*0.8, boundry.width ,boundry.height - 1.2*padding_y_top);
    
    static float panelScrollX = 20.0;
    static float panelVelocityX = 0.0;
    if(CheckCollisionPointRec(GetMousePosition(),boundry)){
        if(IsKeyDown(KEY_LEFT_SHIFT) && GetMouseWheelMove()){
            panelVelocityX *= 0.9;
            panelVelocityX+=GetMouseWheelMove() * boundry.width;
            panelScrollX+=panelVelocityX * GetFrameTime();
        }else{
            panelVelocityX *= 0.9;
            panelVelocityX+=GetMouseWheelMoveV().x * boundry.width;
            panelScrollX+=panelVelocityX * GetFrameTime();
        }
    }
    float x_scroll = boundry.x+panelScrollX;
    // Horizantal Scroll 
    static float panelScrollY = 0.0;
    static float panelVelocityY = 0.0;
    if(CheckCollisionPointRec(GetMousePosition(),boundry)){
        if(!IsKeyDown(KEY_LEFT_SHIFT)){
            panelVelocityY *= 0.9;
            panelVelocityY+=GetMouseWheelMoveV().y * boundry.height;
            panelScrollY+=panelVelocityY*GetFrameTime();
        }
    }
    float y_scroll = boundry.y + panelScrollY;


    float column_width =100;
    float row_height = 70;

    // To generalize values
    if(x_scroll<=-column_width*(4-boundry.width/column_width+0.5)){
        panelScrollX =  -column_width*(4-boundry.width/column_width+0.5) - 5;
        x_scroll = -column_width*(4-boundry.width/column_width+0.5) - 5;
        // printf("hey1\n");
    }
    if(x_scroll>=padding_x){
        panelScrollX = padding_x;
        x_scroll = padding_x;
    }
    if(y_scroll>=5){
        // printf("%f\n",y_scroll);
        // fflush(stdout);
        panelScrollY =  0;
        y_scroll = 5;
        // printf("hey3\n");
    }
    if(y_scroll<=-row_height*(processes_number-boundry.height/row_height+2.5)){
        // printf("hey4\n");
        y_scroll = -row_height*(processes_number-boundry.height/row_height+2.5);
        // fflush(stdout);
        panelScrollY = -row_height*(processes_number-boundry.height/row_height+2.5)-5;
    }
    // To generalize values


    int current_y=0;
    for(int current_x=0;current_x<4;current_x++){
        Rectangle rect = {
            .x = current_x*column_width + padding_x/2 + x_scroll,
            .y = current_y*row_height + padding_y_top + y_scroll,
            .width = column_width,
            .height = row_height,
        };
        // DrawRectangleRec(rect,LIGHTGRAY);
        
        DrawRectangleRoundedLines(rect, borderRadius,2,2 , ColorFromHSV(120,1,1));
        
        switch (current_x)
            {
            case 0:
                DrawText("Process",rect.x + 10 , rect.y + 10 , 20, ColorFromHSV(120,1,1));
                break;
            case 1:
                DrawText("Arrival\n\nTime" ,rect.x + 10, rect.y + 10, 20, ColorFromHSV(120,1,1));
                break;
            case 2:
                DrawText("Burst\n\nTime" ,rect.x + 10 , rect.y + 10, 20, ColorFromHSV(120,1,1));
                break;
            case 3:
                DrawText("Priority" ,rect.x + 10 , rect.y + 10, 20, ColorFromHSV(120,1,1));
                break;
            
            default:
                break;
            }

    }
    current_y++;

    for(;current_y<processes_number+1;current_y++ ){
        for(int current_x=0;current_x<4;current_x++){
            Rectangle rect={
                .x = current_x*column_width + padding_x/2 + x_scroll,
                .y = (current_y)*row_height + padding_y_top + y_scroll,
                .width = column_width,
                .height = row_height,
            } ;
            // DrawRectangleRec(rect,LIGHTGRAY);
            DrawRectangleRoundedLines(rect, borderRadius,2,2 , ColorFromHSV(120,1,1));
            // DrawRectangleLinesEx(rect, 2 , BLACK);
            switch (current_x)
            {
            case 0:
                DrawText(processes[current_y-1].processName,rect.x + 10, rect.y + 10, 20, ColorFromHSV(120,1,1));
                break;
            case 1:
                DrawText(TextFormat("%d", processes[current_y-1].arrivalTime) ,rect.x + 10, rect.y + 10, 20, ColorFromHSV(120,1,1));
                break;
            case 2:
                DrawText(TextFormat("%d", processes[current_y-1].runTime) ,rect.x + 10 , rect.y + 10, 20, ColorFromHSV(120,1,1));
                break;
            case 3:
                DrawText(TextFormat("%d", processes[current_y-1].priority) ,rect.x + 10, rect.y + 10, 20, ColorFromHSV(120,1,1));
                break;
            
            default:
                break;
            }
        } 
    }
        // printf("**x= %d ** y= %d  ** w=%d  ** h=%d\n",boundry.x,boundry.y,boundry.width,boundry.height);

    EndScissorMode();
}
void render_gantt(Rectangle boundry){
    // DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawRectangleRoundedLines(boundry,borderRadius,2,2,ColorFromHSV(120,1,1));
    DrawText("Gantt",boundry.x+textPadding, boundry.y+textPadding,textSize,ColorFromHSV(135,1,1));
    if(isStartButtonPressed && algoResult.gantt &&  is_empty_gantt(algoResult.gantt)) {
        isStartButtonPressed = false;
        free(algoResult.gantt);
        algoResult.gantt = NULL;
        printf("not pressed\n");
    }
    if(isStartButtonPressed && algoResult.gantt && currFrame%(int)delay==0){
        currNode = dequeue_gantt(algoResult.gantt);
        ganttRectanglesSize++;
        ganttRectangles[ganttRectanglesSize-1]=currNode;
    }
    BeginScissorMode(boundry.x,boundry.y,boundry.width,boundry.height);
    static float panelScroll = 0.0;
    static float panelVelocity = 0.0;
    if(CheckCollisionPointRec(GetMousePosition(),boundry)){
        panelVelocity *= 0.9;
        panelVelocity+=GetMouseWheelMove()*boundry.height;
        panelScroll+=panelVelocity*GetFrameTime();
    }
    // printf("%d %d %f \n", boundry.x, boundry.y, panelScroll);
    panelScroll = fmax(min(0, panelScroll), -((ganttRectanglesSize) * (boundry.width*0.05 + textPadding) - boundry.width + textPadding));
    if(ganttRectanglesSize * (boundry.width*0.05 + textPadding) <= boundry.width) panelScroll = 0;
    boundry.x+=panelScroll;
    for(int i=0;i<ganttRectanglesSize;i++){
        // printf("%d\n", name[1]-'0');
        Color c = BLACK;
        if(ganttRectangles[i].processName != NULL) {
            // != null (idle state)
            int processNumber;
            sscanf(ganttRectangles[i].processName,"%*[^0-9]%d",&processNumber);
            
            c = ColorFromHSV(360/processes_number*processNumber,1,0.5);
        }
        DrawRectangleRounded((Rectangle){
            .x=textPadding+boundry.x+(boundry.width*0.05+textPadding)*i,
            .y=boundry.height*0.3+boundry.y,
            .width=boundry.width*0.05,
            .height=boundry.height*0.5,
        },0.1,20,c);
        char* timeChar[255];
        sprintf(timeChar, "%d", ganttRectangles[i].t);
        DrawText(timeChar,boundry.x+textPadding+(boundry.width*0.05+textPadding)*i+1, boundry.height*0.3+boundry.y+10+boundry.height*0.5, boundry.width*0.014, WHITE);
        DrawText(ganttRectangles[i].processName,textPadding+boundry.x+(boundry.width*0.05+textPadding)*i+6, boundry.height*0.3+boundry.y+10, 0.025*boundry.width, WHITE);
        if(ganttRectangles[i].quit == 1) {
            DrawRectangle(textPadding+boundry.x+(boundry.width*0.05+textPadding)*i +boundry.width*0.05 - 4, boundry.height*0.2+boundry.y, 4,boundry.height*0.2, c);

            DrawTriangle((Vector2){ textPadding+boundry.x+(boundry.width*0.05+textPadding)*i +boundry.width*0.05 - 12 - 2, boundry.height*0.2+boundry.y },
                        (Vector2){ textPadding+boundry.x+(boundry.width*0.05+textPadding)*i +boundry.width*0.05 + 12 - 2 , boundry.height*0.2+boundry.y },
                        (Vector2){ textPadding+boundry.x+(boundry.width*0.05+textPadding)*i + boundry.width*0.05 - 2, boundry.height*0.15+boundry.y - 4}, c);
        }
    }
    EndScissorMode();

}
void render_stats(Rectangle boundry){
    int ww=boundry.width/5;
    int hh=boundry.height/8;
    int rectww=boundry.width/4;
    int recthh=boundry.height/8;
    int subTitleTextFront = boundry.width/20;


    // DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawRectangleRoundedLines(boundry,borderRadius,2,2,ColorFromHSV(120,1,1));
    DrawText("Stats",boundry.x+textPadding, boundry.y+textPadding,textSize,ColorFromHSV(135,1,1));

    // ----------------
    DrawText("Ready queue",boundry.x+textPadding, boundry.y+boundry.height/6,subTitleTextFront+subTitleTextFront*0.2,BLACK);
    if(ganttSize!=0){
        if(currNode.readyQueueSize!=0){
            int readyQueueRectSize = boundry.width*0.15;
            for(int i=0;i<min(4,currNode.readyQueueSize);i++){
                int processNumber;
                sscanf(currNode.readyQueue[i],"%*[^0-9]%d",&processNumber);
                Color c = ColorFromHSV(360/processes_number*processNumber,1,0.5);
                DrawRectangleRounded((Rectangle){
                    .x=boundry.x+textPadding+(boundry.width*.2)*i,
                    .y=boundry.y+boundry.height/4,
                    .width=readyQueueRectSize,
                    .height=boundry.height/6,
                },0.1,20,c);
                DrawText(currNode.readyQueue[i],boundry.x+textPadding+(boundry.width*.2)*i+readyQueueRectSize*.3, boundry.y+boundry.height/4+boundry.height/40,subTitleTextFront,WHITE);
            }
        }
    }
    // ----------------
    // DrawTextEx(font,currNode.readyQueueSize!=0?currNode.readyQueue[0]:"",(Vector2){boundry.x+boundry.width/2,boundry.y+boundry.height/2},textSize,0,RED);
    // ------------------
    DrawText("Current job",boundry.x+boundry.width/8+abs(ww-rectww)/2, boundry.y+2*boundry.height/4-hh/2,subTitleTextFront,ColorFromHSV(120,1,1));
    // DrawRectangle(boundry.x+boundry.width/8+abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh,WHITE);
    DrawRectangleRoundedLines((Rectangle){boundry.x+boundry.width/8+abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh},borderRadius,2,2, ColorFromHSV(120,1,1));
    // DrawRectangleLinesEx((Rectangle){boundry.x+boundry.width/8+abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh},3, ColorFromHSV(120,1,1));
    if(ganttSize!=0)DrawText(currNode.processName,boundry.x+boundry.width/8+abs(ww-rectww)/2+ww/2-subTitleTextFront/2, boundry.y+2*boundry.height/4+hh/2-subTitleTextFront/2,subTitleTextFront,ColorFromHSV(120,1,1));

    //GuiIconText(ICON_REPEAT,"") ;
    // DrawTextEx(font,"Current time",(Vector2){boundry.x+2.2*boundry.width/3, boundry.y+2.2*ReadyQrectangleHeight},subTitleTextFront,0,ColorFromHSV(120,1,1));
    DrawText("Current time",boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2, boundry.y+2*boundry.height/4-hh/2,subTitleTextFront,ColorFromHSV(120,1,1));
    // DrawRectangle(boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh,WHITE);
    DrawRectangleRoundedLines((Rectangle){boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh},borderRadius,2,2, ColorFromHSV(120,1,1));
    // DrawRectangleLinesEx((Rectangle){boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2,boundry.y+2*boundry.height/4,ww,hh},3, ColorFromHSV(120,1,1));
    // ADD the time here
    char ss[20];
    sprintf(ss,"%d",ganttRectanglesSize);
    DrawText(ss,boundry.x+boundry.width-ww-boundry.width/8-abs(ww-rectww)/2+ww/2-subTitleTextFront/2, boundry.y+2*boundry.height/4+hh/2-subTitleTextFront/2,subTitleTextFront,ColorFromHSV(120,1,1));


    // DrawTextEx(font,"Average rotation time",(Vector2){boundry.x+boundry.width/4.2, boundry.y+3.7*ReadyQrectangleHeight},subTitleTextFront,0,ColorFromHSV(120,1,1));
    DrawText("Avg rotation time",boundry.x+boundry.width/8, boundry.y+3*boundry.height/4-hh/2,subTitleTextFront,ColorFromHSV(120,1,1));
    // DrawRectangle(boundry.x+boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh,WHITE);
    DrawRectangleRoundedLines((Rectangle){boundry.x+boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh},borderRadius,2,2, ColorFromHSV(120,1,1));
    // DrawRectangleLinesEx((Rectangle){boundry.x+boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh},3, ColorFromHSV(120,1,1));
    if(ganttRectanglesSize == ganttSize && ganttSize!=0){
        char sss[20];
        sprintf(sss,"%.2f",algoResult.metrics.averageRotation);
        DrawText(sss,boundry.x+boundry.width/8+ww/2-subTitleTextFront/2, boundry.y+3*boundry.height/4+hh/2-subTitleTextFront/2,subTitleTextFront,ColorFromHSV(120,1,1));
    }else{
        DrawText("",boundry.x+boundry.width/8+ww/2-subTitleTextFront/2, boundry.y+3*boundry.height/4+hh/2-subTitleTextFront/2,subTitleTextFront,ColorFromHSV(120,1,1));
    }

    DrawText("Avg waiting time",boundry.x+boundry.width-rectww-boundry.width/8, boundry.y+3*boundry.height/4-hh/2,subTitleTextFront,ColorFromHSV(120,1,1));
    // DrawRectangle(boundry.x+boundry.width-rectww-boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh,WHITE);
    DrawRectangleRoundedLines((Rectangle){boundry.x+boundry.width-rectww-boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh},borderRadius,2,2, ColorFromHSV(120,1,1));
    // DrawRectangleLinesEx((Rectangle){boundry.x+boundry.width-rectww-boundry.width/8,boundry.y+3*boundry.height/4,rectww,recthh},3, ColorFromHSV(120,1,1));
    if(ganttRectanglesSize == ganttSize && ganttSize!=0){
        char sss[20];
        sprintf(sss,"%.2f",algoResult.metrics.averageWaiting);
        DrawText(sss,boundry.x+boundry.width-rectww-boundry.width/8+ww/2-subTitleTextFront/2, boundry.y+3*boundry.height/4+hh/2-subTitleTextFront/2,subTitleTextFront,ColorFromHSV(120,1,1));
    }else{
        DrawText("",boundry.x+boundry.width-rectww-boundry.width/8+ww/2-subTitleTextFront/2, boundry.y+3*boundry.height/4+hh/2-subTitleTextFront/2,subTitleTextFront,ColorFromHSV(120,1,1));
    }

    // -------------------
}
void render_menu(Rectangle boundry){
    // DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawRectangleRoundedLines(boundry,borderRadius,2,2,ColorFromHSV(120,1,1));
    DrawText("Menu",boundry.x+textPadding, boundry.y+textPadding,1.2 * textSize,ColorFromHSV(135,1,1));
    // if(GuiButton((Rectangle){boundry.x+textPadding,boundry.y+18*textPadding,boundry.width - 20,30},"Start")&&!algosDropDown1EditMode) {
    // if(GuiButton((Rectangle){boundry.x + boundry.width * 0.1, boundry.y+boundry.height*0.64 ,boundry.width * 0.4, boundry.height * 0.11},"Start")&&!algosDropDown1EditMode) {
    if(GuiButton((Rectangle){boundry.x + boundry.width * 0.09, boundry.y+boundry.height*0.81 ,boundry.width * 0.4, boundry.height * 0.11},"Start")&&!algosDropDown1EditMode) {
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
            // processes_number = getNbProcesses("./processes.txt");
            // processes = getTableOfProcesses("./processes.txt");
            Queue* q = create_queue_from_array(processes,processes_number);
            printf("I about to run the algo \n");

            algoResult = algorithms[selectedAlgoIndex].run(q, processes_number, quantumValue);
            printf("%f %f\n", algoResult.metrics.averageRotation, algoResult.metrics.averageWaiting);
            ganttSize = size_gantt(algoResult.gantt);
            printf("gantt size = %d\n",ganttSize);
            printf("begin problem\n");
            ganttRectangles = (InstantResultNode*) malloc(ganttSize*sizeof(InstantResultNode));
            printf("end problem\n");
            isStartButtonPressed = true;
            // freeProcesses(); //if you uncomment this line, then the processname in the readyqueue will be random and not understandable
            
        }
    }
    // if(GuiButton((Rectangle){boundry.x+textPadding,boundry.y+22*textPadding,boundry.width - 20,30},"Reset")&&!algosDropDown1EditMode) {
    if(GuiButton((Rectangle){boundry.x+boundry.width*0.11 + boundry.width * 0.4,boundry.y+boundry.height*0.81 ,boundry.width * 0.4, boundry.height * 0.11},"Reset")&&!algosDropDown1EditMode) {
        if(!isStartButtonPressed) {
            //generate processes
            generate_processes_file("config.conf","processes.txt",';');
            printf("new file generated\n");
            ganttSize = 0;
            ganttRectanglesSize = 0;
            processes_number = getNbProcesses("./processes.txt");
            processes = getTableOfProcesses("./processes.txt");
            if(ganttRectangles) {
                printf("ganttrectangles are being freed\n");
                free(ganttRectangles);
                ganttRectangles = NULL;
                printf("----------------\n");
            }
        }
    }
    
    DrawText("Quantum",boundry.x+textPadding, boundry.y + boundry.height * 0.37, 0.75 * textSize,ColorFromHSV(135,1,1));
    GuiSpinner((Rectangle){ boundry.x+boundry.width * 0.1, boundry.height * 0.37 + textSize, boundry.width * 0.8, boundry.height * 0.11 }, NULL, &quantumValue, 1, 10, quantumSpinnerEditMode);
    DrawText("Simulation Latency",boundry.x+textPadding, boundry.y + boundry.height * 0.48 + textSize, 0.75 * textSize,ColorFromHSV(135,1,1));
    GuiSlider((Rectangle){ boundry.x + boundry.width * 0.1, boundry.y+boundry.height*0.48 + 2*textSize ,boundry.width * 0.8, boundry.height * 0.11 }, "", TextFormat("%2.2f", delay), &delay, 1, 60);
    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 4);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    // DrawTextEx(font,"Scheduling Algorithms",(Vector2){boundry.x+textPadding, boundry.y+3*textPadding},textSize,0,RED);
    DrawText("Scheduling Algorithms",boundry.x+textPadding, boundry.height * 0.26 - textSize, 0.75 * textSize,ColorFromHSV(135,1,1));
    if(GuiDropdownBox((Rectangle){boundry.x+boundry.width * 0.1, boundry.height * 0.26, boundry.width * 0.8, boundry.height * 0.11}, algoOptions, &selectedAlgoIndex, algosDropDown1EditMode)) {
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
        .width = (w-2*padding-3*gap)/3,
        .height = 2*(h-gap)/3,
    };
    render_job_pool(jobPoolRect);
    Rectangle statsRect = {
        .x = gap+padding+(w-2*padding-3*gap)/3,
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
    Rectangle menuRect = {
        .x = 2*gap+padding+2*(w-2*padding-2*gap)/3,
        .y = 0+padding,
        .width = (w-2*padding-3*gap)/3,
        .height = 2*(h-gap)/3,
    };
    render_menu(menuRect);
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
    processes_number = getNbProcesses("./processes.txt");
    processes = getTableOfProcesses("./processes.txt");

    GuiLoadStyleTerminal();
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

    for(int i=0;i<numberOfAlgo;i++) {
        strcat(algoOptions,algorithms[i].name);
        if(i<numberOfAlgo-1)strcat(algoOptions,";");
    }
    printf("%s\n", algoOptions);
    while(!WindowShouldClose()) {
        preview_screen();
        currFrame++;
        currFrame%=(int)delay;
    }
    CloseWindow();
    return 0;
}