/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
#define MAX_TARGETS_ON_SCREEN 4
Vector2 delta1;
Vector2 delta2;
Vector2 delta3;
Vector2 delta4;
Vector2 deltas[MAX_TARGETS_ON_SCREEN];
Vector2 pos1, pos2, pos3, pos4;
Vector2 positions[MAX_TARGETS_ON_SCREEN];
int ratio_target = 50;
int ratio_cursor = 10;
bool alive[MAX_TARGETS_ON_SCREEN]={true, true, true, true};
Vector2 mousPosition;
float cursor_speed = 5.0F;
Vector2 ballPosition;

int points = 0;
int points_per_target = 10;
int max_speed = 1;
int loc_screenWidth, loc_screenHeight;
bool user_failed = false;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
void recalculate_deltas (int max_speed, int screenWidth, int screenHeight)
{

    pos1.x = (float)screenWidth/4;
    pos1.y = (float)screenHeight/4;
    pos2.x = (float)screenWidth/2 +200;
    pos2.y = (float)2*screenHeight/3;
    pos3.x = (float)screenWidth/3-100;
    pos3.y = (float)screenHeight/2+100;
    pos4.x = (float)screenWidth/2+200;
    pos4.y = (float)screenHeight/4-50;
    positions[0] = pos1;
    positions[1] = pos2;
    positions[2] = pos3;
    positions[3] = pos4;
    SetRandomSeed(5);
    delta1.x = GetRandomValue((-1)*max_speed, max_speed);
    delta1.y = GetRandomValue((-1)*max_speed, max_speed);
    delta2.x = GetRandomValue((-1)*max_speed, max_speed);
    delta2.y = GetRandomValue((-1)*max_speed, max_speed);
    delta3.x = GetRandomValue((-1)*max_speed, max_speed); 
    delta3.y = GetRandomValue((-1)*max_speed, max_speed); 
    delta4.x = GetRandomValue((-1)*max_speed, max_speed); 
    delta4.y = GetRandomValue((-1)*max_speed, max_speed);

    deltas[0] = delta1;
    deltas[1] = delta2;
    deltas[2] = delta3;
    deltas[3] = delta4;
    printf("delta1.x =%f\n", delta1.x);
    printf("delta1.y =%f\n", delta1.y);

}
// Gameplay Screen Initialization logic
void InitGameplayScreen(int screenWidth, int screenHeight)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    user_failed = false;

    ballPosition.x = (float)screenWidth/2;
    ballPosition.y = (float)screenHeight/2;
    loc_screenWidth = screenWidth;
    loc_screenHeight = screenHeight;



    recalculate_deltas(5, loc_screenWidth, loc_screenHeight);


}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
     for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)  
        {
            positions[i].x += deltas[i].x;
            positions[i].y +=  deltas[i].y;
            bool bounce = false;
            if((positions[i].x < ratio_target/2)||(positions[i].x > (loc_screenWidth - ratio_target/2)))
            {
                 deltas[i].x *=(-1);
                 bounce = true;
            }
            if((positions[i].y < ratio_target/2)||(positions[i].y > (loc_screenHeight - ratio_target/2)))
            {
                 deltas[i].y *=(-1);
                 bounce = true;
            }
            if(bounce == true)
            {
            }
        }


        ballPosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
  
            bool hit = false;
            for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)
            {
                if(CheckCollisionCircles(positions[i], ratio_target,ballPosition, ratio_cursor))
                {
                    alive[i] = false;
                    points +=points_per_target;
                    hit = true;
                }         
            }

            if(hit == false)
            {
                user_failed = true;
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
            max_speed++;
            recalculate_deltas(max_speed, loc_screenWidth, loc_screenHeight);
            alive[0] = true;
            alive[1] = true;
            alive[2] = true;
            alive[3] = true;
        }    


    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_Q) ||  user_failed == true)// || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{

        //DrawText(TextFormat("Avalable shots: %02i", available_shots), 10, 10, 40, BLUE);
        DrawText(TextFormat("Points: %02i", points), 10, 40, 40, BLUE);


        for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)
        {
            if(alive[i] == true)
            {
                DrawCircleV(positions[i], ratio_target, DARKGRAY);
            }

        }




    DrawCircleV(ballPosition, ratio_cursor, MAROON);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}