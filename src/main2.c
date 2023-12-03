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

int started=0;

void render_job_pool(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Job pool",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
}
void render_gantt(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Gantt",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
}
void render_stats(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Stats",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
}
void render_menu(Rectangle boundry){
    DrawRectangleRounded(boundry,borderRadius,20,containerColor);
    DrawTextEx(font,"Menu",(Vector2){boundry.x+textPadding, boundry.y+textPadding},textSize,0,RED);
}
void show_gantt_stats(Rectangle ganttBoundry,Rectangle statsBoundry){
    render_gantt(ganttBoundry);
    render_stats(statsBoundry);
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