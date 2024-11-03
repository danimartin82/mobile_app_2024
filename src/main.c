/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

For a C++ project simply rename the file to .cpp and re-run the build script 

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "HOLA raylib [core] example - keyboard input");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    Vector2 pos1 = { (float)screenWidth/4, (float)screenHeight/4 };
    Vector2 pos2 = { (float)screenWidth/2 +200, (float)2*screenHeight/3 };
    Vector2 pos3 = { (float)screenWidth/3-100, (float)screenHeight/2+100 };
    Vector2 pos4 = { (float)screenWidth/2+200, (float)screenHeight/4-50 };
    Vector2 positions[4] = {pos1,pos2,pos3,pos4};
    int ratio_target = 50;
    int ratio_cursor = 10;
    bool alive[4]={true, true, true, true};
    Vector2 mousPosition;
    float cursor_speed = 5.0F;
    SetRandomSeed(5);
    Vector2 delta1 = {GetRandomValue(-10,10),GetRandomValue(-10,10)}; 
    Vector2 delta2 = {GetRandomValue(-10,10),GetRandomValue(-10,10)}; 
    Vector2 delta3 = {GetRandomValue(-10,10),GetRandomValue(-10,10)}; 
    Vector2 delta4 = {GetRandomValue(-10,10),GetRandomValue(-10,10)}; 
    Vector2 deltas[4] = {delta1, delta2, delta3, delta4};


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        for (int i = 0; i<4; i++)  
        {
            positions[i].x = positions[i].x + deltas[i].x;
            positions[i].y = positions[i].y + deltas[i].y;
            
            if((positions[i].x < 0)||(positions[i].x > screenWidth))
            {
                 deltas[i].x *=(-1);
            }
            if((positions[i].y < 0)||(positions[i].y > screenHeight))
            {
                 deltas[i].y *=(-1);
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //  DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);
        ballPosition = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for (int i =0; i<4; i++)
            {
                if(CheckCollisionCircles(positions[i], ratio_target,ballPosition, ratio_cursor))
                {
                    alive[i] = false;
                }         
            }

        }
        for (int i = 0; i<4; i++)
        {
            if(alive[i] == true)
            {
                DrawCircleV(positions[i], ratio_target, DARKGRAY);
            }

        }




        DrawCircleV(ballPosition, ratio_cursor, MAROON);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

