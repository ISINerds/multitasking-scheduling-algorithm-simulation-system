#include "./includes/utils/dynamic_menu.h"
#include "./includes/utils/processes_generator.h"
#include "./includes/utils/ProcessesTable.h"
#include "./includes/utils/algo_result.h"
#include <unistd.h>

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


int nbAlgorithms = 0;
Algorithm* algorithms = NULL;
Process* processes = NULL;

int rectangleWidth = 70;
int rectangleHeight = 50;

int ReadyQrectangleWidth = 70;
int ReadyQrectangleHeight = 50;

int algoChoice = 0;
bool editMode=false;
int value = 0; // Initial value of the spinner


typedef struct {
    int x;
    int y;
    int width;
    int height;
    Color color;
} RectangleData;

RectangleData rects[5];

int nb=5;
int currentRectangleIndex = 0; // Variable to keep track of current rectangle index
Color colors[] = { RED, BLUE, BLACK, PINK, YELLOW };
InstantResultNode tableau[5]= {
    {0,"P1",{"P1"},1,0,NULL},
    {1,"P2",{"P2","P1"},2,0,NULL},
    {2,"P2",{"P1","P1"},2,1,NULL},
    {3,"P3",{"P1","P3"},1,0,NULL},
    {4,"P1",{"P1"},1,1,NULL}
};
AlgoResult algoResult;

int started=0;

void render_job_pool(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Job pool",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
}
void render_gantt(Rectangle boundry,Rectangle boundry2){

    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Gantt",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);


    if (currentRectangleIndex < nb) {
            printf("Normally \n");
            rects[currentRectangleIndex].x = boundry.x + (currentRectangleIndex * rectangleWidth) +5;
            rects[currentRectangleIndex].y= boundry.y+50;
            rects[currentRectangleIndex].width= rectangleWidth ;
            rects[currentRectangleIndex].height= rectangleHeight;
            rects[currentRectangleIndex].color= colors[currentRectangleIndex];
            sleep(1);
            currentRectangleIndex++;
    }
    for (int i=0;i<currentRectangleIndex;i++) {
        DrawRectangle(rects[i].x,rects[i].y,rects[i].width,rects[i].height,rects[i].color);
        //DrawRectangle(boundry.x + (i * rectangleWidth) +5,boundry.y+10,rectangleWidth,rectangleHeight,colors[i]);
    }
    render_stats(boundry2,currentRectangleIndex);


}
// void render_queue(Rectangle boundry,int n){
//     if (n!=nb) {
//         printf("size : %d \n",tableau[n].readyQueueSize);
//          for (int j=0;j<tableau[n].readyQueueSize;j++) {
//             printf(" ready queue size : %d \n",tableau[n].readyQueueSize);
//             DrawRectangle(boundry.x+(j*ReadyQrectangleWidth)+5, boundry.y+5, ReadyQrectangleWidth, ReadyQrectangleHeight, colors[n]);
//         }
//     }
//     else {printf("Not showing the ready queue :\n");}
// }

void render_stats(Rectangle boundry,int n){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Ready Queue",(Vector2){boundry.x+boundry.width/3.3, boundry.y+textPadding},textSize,0,BLACK);
    //GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    if (n!=nb) {
        printf("size : %d \n",tableau[n].readyQueueSize);
         for (int j=0;j<tableau[n].readyQueueSize;j++) {
            printf(" ready queue size : %d \n",tableau[n].readyQueueSize);
            DrawRectangle(boundry.x+(j*ReadyQrectangleWidth)+5, boundry.y+50, ReadyQrectangleWidth, ReadyQrectangleHeight, colors[n]);
        }
    }
    else {printf("Not showing the ready queue :\n");}
    DrawTextEx(font,"Current job",(Vector2){boundry.x+boundry.width/8, boundry.y+2.2*ReadyQrectangleHeight},textSize-10,0,BLACK);
    RectangleData CurrJobRect = {boundry.x+boundry.width/8, boundry.y+2.2*ReadyQrectangleHeight+textSize ,60,40,BLACK};
    DrawRectangle(CurrJobRect.x,CurrJobRect.y,CurrJobRect.width,CurrJobRect.height,WHITE);
    DrawRectangleLinesEx((Rectangle){CurrJobRect.x,CurrJobRect.y,CurrJobRect.width,CurrJobRect.height,WHITE},3, BLACK);                          

    //GuiIconText(ICON_REPEAT,"") ;
    DrawTextEx(font,"Current time",(Vector2){boundry.x+2.2*boundry.width/3, boundry.y+2.2*ReadyQrectangleHeight},textSize-10,0,BLACK);
    RectangleData CurrTimeRect = {boundry.x + 3*boundry.width/4 , boundry.y+2.2*ReadyQrectangleHeight+textSize ,60,40,WHITE};
    DrawRectangle(CurrTimeRect.x,CurrTimeRect.y,CurrTimeRect.width,CurrTimeRect.height,WHITE);
    DrawRectangleLinesEx((Rectangle){CurrTimeRect.x,CurrTimeRect.y,CurrTimeRect.width,CurrTimeRect.height,WHITE},3, BLACK);                          


    DrawTextEx(font,"Average rotation time",(Vector2){boundry.x+boundry.width/4.2, boundry.y+3.7*ReadyQrectangleHeight},textSize-10,0,BLACK);
    RectangleData RotationTimeRect = {boundry.x + boundry.width/3 , boundry.y+4.2*ReadyQrectangleHeight ,100,35,WHITE};
    DrawRectangle(RotationTimeRect.x,RotationTimeRect.y,RotationTimeRect.width,RotationTimeRect.height,WHITE);
    DrawRectangleLinesEx((Rectangle){RotationTimeRect.x,RotationTimeRect.y,RotationTimeRect.width,RotationTimeRect.height,WHITE},3, BLACK);                       

    DrawTextEx(font,"Average waiting time",(Vector2){boundry.x+boundry.width/4.2, boundry.y+5*ReadyQrectangleHeight},textSize-10,0,BLACK);
    RectangleData WaitingTimeRect = {boundry.x + boundry.width/3 , boundry.y+5.5*ReadyQrectangleHeight ,100,35,WHITE};
    DrawRectangle(WaitingTimeRect.x,WaitingTimeRect.y,WaitingTimeRect.width,WaitingTimeRect.height,WHITE);
    DrawRectangleLinesEx((Rectangle){WaitingTimeRect.x,WaitingTimeRect.y,WaitingTimeRect.width,WaitingTimeRect.height,WHITE},3, BLACK);                       
// }
}
void render_menu(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Menu",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);

    Rectangle DropdownRect = { boundry.x+boundry.x/8, boundry.y+40, boundry.width/2, 35 }; //boundry.y + 40
    DrawText("Quantum", boundry.x+boundry.x/8, boundry.y+80, 25, BLACK); //+40
    Rectangle SpinnerRect = { boundry.x+boundry.x/8, 80+DropdownRect.y, boundry.width/2, 35 }; //+80
    Rectangle Startbutton = { boundry.x+boundry.x/8, SpinnerRect.y+80 , 200, 35}; 
    Rectangle Resetbutton = { boundry.x+boundry.x/8, Startbutton.y+80, 200, 35};

    const char *dropdownItems =  "Option 1;Option 2;Option3";
        //if (editMode) GuiLock();

        DrawRectangle(5, 5, 392.5, 175, (Color){ 220, 220, 220, 255 });
        GuiSetStyle(BUTTON, TEXT_SIZE, 40);
        GuiSetStyle(BUTTON,TEXT_COLOR_NORMAL,ColorToInt(BLACK));
        GuiSetStyle(BUTTON,BORDER_COLOR_NORMAL,ColorToInt(BLACK));
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        GuiUnlock();
        GuiSetStyle(BUTTON,BASE_COLOR_NORMAL,ColorToInt(WHITE));
        GuiSetStyle(BUTTON,TEXT_PADDING,1);
        //GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, borderColorInt);
        GuiSetStyle(DROPDOWNBOX,TEXT_COLOR_NORMAL,ColorToInt(BLACK));
        GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        GuiSetStyle(DROPDOWNBOX,BORDER_COLOR_NORMAL,ColorToInt(BLACK));
        GuiSetStyle(DROPDOWNBOX,BASE_COLOR_NORMAL,ColorToInt(WHITE));
        GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 0);
        if (GuiDropdownBox(DropdownRect, dropdownItems, &algoChoice, editMode)) editMode=!editMode;
        if(GuiButton(Startbutton, "Start")){
            
        }
        if (GuiButton(Resetbutton, GuiIconText(ICON_REPEAT,"Reset"))){

        }
        GuiSpinner(SpinnerRect, NULL, &value, -10, 10, false);
}
void show_gantt_stats(Rectangle ganttBoundry,Rectangle statsBoundry){
    render_gantt(ganttBoundry,statsBoundry);
    // sleep(1);
}
void preview_screen(void){
    int w = GetRenderWidth();
    int h = GetRenderHeight();
    BeginDrawing(); 
    ClearBackground(backgroundColor);
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
    Rectangle ganttRect = {
        .x = padding,
        .y = 2*h/3 + gap,
        .width = w-2*padding,
        .height = (h-gap)/3 - padding,
    };
    show_gantt_stats(ganttRect,statsRect);
    EndDrawing();

}
int main(void){
    const int screenWidth = 900;
    const int screenHeight = 500;
 
    InitWindow(screenWidth, screenHeight, "Multitasking Scheduling Algorithm Simulation System");
    font = LoadFont("../assets/fonts/DelaGothicOne-Regular.ttf");
 
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        preview_screen();
        printf("%d\n",GetFPS());
    }
    CloseWindow();
    return 0;
}