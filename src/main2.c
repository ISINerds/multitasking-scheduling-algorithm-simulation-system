/*******************************************************************************************
*
*   raygui - custom sliders
*
*   DEPENDENCIES:
*       raylib 4.0  - Windowing/input management and drawing.
*       raygui 3.0  - Immediate-mode GUI controls.
*
*   COMPILATION (Windows - MinGW):
*       gcc -o $(NAME_PART).exe $(FILE_NAME) -I../../src -lraylib -lopengl32 -lgdi32 -std=c99
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2016-2023 Ramon Santamaria (@raysan5)
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "../build_raylib/_deps/raygui-src/src/raygui.h"
#include "raylib.h"
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
 
    InitWindow(screenWidth, screenHeight, "Multitasking Scheduling Algorithm Simulation System");
 
    // TODO: Load resources / Initialize variables at this point
 
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
 
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update variables / Implement example logic at this point
        //----------------------------------------------------------------------------------
 
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
 
            ClearBackground(RAYWHITE);
 
            // TODO: Draw everything that requires to be drawn at this point:
 
 
            DrawRectangle(5, 5, 392.5, 175, (Color){ 220, 220, 220, 255 });
            DrawRectangle(402.5, 5, 392.5, 175, (Color){ 220, 220, 220, 255 });
            DrawRectangle(5, 185, 260, 130, (Color){ 220, 220, 220, 255 });
            DrawRectangle(270, 185, 260, 130, (Color){ 220, 220, 220, 255 });
            DrawRectangle(535, 185, 260, 130, (Color){ 220, 220, 220, 255 });
            DrawRectangle(5, 320, 790, 125, (Color){ 220, 220, 220, 255 });
            DrawText("Data", 20, 15, 25, RED);
            DrawText("Jobs", 417.5, 15, 25, RED);
            DrawText("Cpu", 20, 195, 25, RED);
            DrawText("Ready Queue", 285, 195, 25, RED);
            DrawText("Average", 550, 195, 25, RED);
            DrawText("Gantt", 20, 330, 25, RED);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
 
    // De-Initialization
    //--------------------------------------------------------------------------------------
 
    // TODO: Unload all loaded resources at this point
 
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
 
    return 0;
}