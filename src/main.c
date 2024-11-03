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


#define MAX_TARGETS_ON_SCREEN 4
Vector2 delta1;
Vector2 delta2;
Vector2 delta3;
Vector2 delta4;
Vector2 deltas[MAX_TARGETS_ON_SCREEN];


void recalculate_deltas (void)
{

    delta1.x = GetRandomValue(-10,10);
    delta1.y = GetRandomValue(-10,10); 
    delta2.x = GetRandomValue(-10,10); 
    delta2.y = GetRandomValue(-10,10); 
    delta3.x = GetRandomValue(-10,10); 
    delta3.y = GetRandomValue(-10,10); 
    delta4.x = GetRandomValue(-10,10); 
    delta4.y = GetRandomValue(-10,10); 

    deltas[0] = delta1;
    deltas[1] = delta2;
    deltas[2] = delta3;
    deltas[3] = delta4;
    

}


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
    Vector2 positions[MAX_TARGETS_ON_SCREEN] = {pos1,pos2,pos3,pos4};
    int ratio_target = 50;
    int ratio_cursor = 10;
    bool alive[MAX_TARGETS_ON_SCREEN]={true, true, true, true};
    Vector2 mousPosition;
    float cursor_speed = 5.0F;
    SetRandomSeed(5);
    int available_shots = 20;
    int points = 0;
    int points_per_target = 10;
    recalculate_deltas();

    InitAudioDevice();      // Initialize audio device

    Sound fx1 = LoadSound("resources/shoot.wav");
    Sound fx2 = LoadSound("resources/shoot_hit.wav");


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)  
        {
            positions[i].x = positions[i].x + deltas[i].x;
            positions[i].y = positions[i].y + deltas[i].y;
            
            if((positions[i].x < ratio_target/2)||(positions[i].x > (screenWidth - ratio_target/2)))
            {
                 deltas[i].x *=(-1);
            }
            if((positions[i].y < ratio_target/2)||(positions[i].y > (screenHeight - ratio_target/2)))
            {
                 deltas[i].y *=(-1);
            }
        }


        ballPosition = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            bool hit = false;
            if(available_shots > 0)
            {
                for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)
                {
                    if(CheckCollisionCircles(positions[i], ratio_target,ballPosition, ratio_cursor))
                    {
                        alive[i] = false;
                        points +=points_per_target;
                        PlaySound(fx2);
                        hit = true;
                    }         
                }
                available_shots--;
                if(hit == false)
                {
                    PlaySound(fx1);
                }
            }

           

        }

        bool all_finished = true;
        for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)
        {
            if(  alive[i] == true)
            all_finished = false;
        }
        if(all_finished == true)
        {
            recalculate_deltas();
            alive[0] = true;
            alive[1] = true;
            alive[2] = true;
            alive[3] = true;
        }    

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Avalable shots: %02i", available_shots), 10, 10, 40, BLUE);
        DrawText(TextFormat("Points: %02i", points), 10, 40, 40, BLUE);


        for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)
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
    UnloadSound(fx1);     // Unload sound data
    UnloadSound(fx2);     // Unload sound data

    CloseAudioDevice();     // Close audio device


    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

