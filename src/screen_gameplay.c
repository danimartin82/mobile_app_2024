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
#include <stdio.h>
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------

GameEnd finishScreen = 0;
#define MAX_TARGETS_ON_SCREEN 10
Vector2 deltas[MAX_TARGETS_ON_SCREEN];
Vector2 positions[MAX_TARGETS_ON_SCREEN];
int ratio_cursor = 10;
float cursor_speed = 5.0F;
bool alive[MAX_TARGETS_ON_SCREEN]={false};
Vector2 mousPosition;

Vector2 ballPosition;

int points = 0;
bool user_failed = false;
int max_speed = 1;
int points_per_target = 0;
int radio_target = 0;
int required_points = 0;
int balls_per_game = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
void recalculate_deltas(int max_speed)
{

    SetRandomSeed(5);

    for (int i = 0; i < balls_per_game; i++)
    {
        Vector2 delta;
        Vector2 position;
        delta.x = GetRandomValue((-1)*max_speed, max_speed);
        delta.y = GetRandomValue((-1)*max_speed, max_speed);
        position.x =  GetRandomValue(0, GetScreenWidth());
        position.y =  GetRandomValue(0,  GetScreenHeight());
        deltas[i] = delta;
        positions[i] = position;
    }

}
// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    finishScreen = 0;
    user_failed = false;
    points = 0;

    ballPosition.x = (float)GetScreenWidth()/2;
    ballPosition.y = (float)GetScreenHeight()/2;

    switch (getLevel())
    {
        default:
        case 1:
        {
            points_per_target = 10;
            radio_target = 50;
            required_points = 50;
            balls_per_game = 4;
        }
        break;
        case 2:
        {
            points_per_target = 15;
            radio_target = 40;
            required_points = 80;
            balls_per_game = 5;
        }
        break;
        case 3:
        {
            points_per_target = 20;
            radio_target = 30;
            required_points = 100;
            balls_per_game = 6;
        }
        break;
        case 4:
        {
            points_per_target = 25;
            radio_target = 20;
            required_points = 120;
            balls_per_game = 7;
        }
        break;
        case 5:
        {
            points_per_target = 30;
            radio_target = 25;
            required_points = 140;
            balls_per_game = 8;
        }
        break;
        case 6:
        {
            points_per_target = 36;
            radio_target = 20;
            required_points = 160;
            balls_per_game = 9;
        }
        break;                
    }

    for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)
    {
        if (i < balls_per_game) alive[i] = true;
        else alive[i] = false;
    }

    recalculate_deltas(5);


}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
     for (int i = 0; i < balls_per_game; i++)  
        {
            positions[i].x += deltas[i].x;
            positions[i].y +=  deltas[i].y;
            bool bounce = false;
            if((positions[i].x < radio_target/2)||(positions[i].x > (GetScreenWidth() - radio_target/2)))
            {
                 deltas[i].x *=(-1);
                 bounce = true;
            }
            if((positions[i].y < radio_target/2)||(positions[i].y > (GetScreenHeight() - radio_target/2)))
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
            for (int i = 0; i < balls_per_game; i++)
            {
                if(CheckCollisionCircles(positions[i], radio_target, ballPosition, ratio_cursor))
                {
                    alive[i] = false;
                    points += points_per_target;
                    hit = true;
                }         
            }

            if(hit == false)
            {
                user_failed = true;
            }
        }

           


        bool all_finished = true;
        for (int i = 0; i < balls_per_game; i++)
        {
            if( alive[i] == true)
            all_finished = false;
        }
        if(all_finished == true)
        {
            max_speed++;
            recalculate_deltas(max_speed);

            for (int i = 0; i < balls_per_game; i++)
            {
                alive[i] = true;
            }    
        } 

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_Q) ||  user_failed == true)// || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = GAME_END_FAIL;
        PlaySound(fxCoin);
    }
    if (points >= required_points)
    {
        finishScreen = GAME_END_WIN;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{

        //DrawText(TextFormat("Avalable shots: %02i", available_shots), 10, 10, 40, BLUE);
        DrawText(TextFormat("Points: %02i", points), 10, 40, 40, BLUE);


        for (int i = 0; i < balls_per_game; i++)
        {
            if(alive[i] == true)
            {
                DrawCircleV(positions[i], radio_target, DARKGRAY);
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
GameEnd FinishGameplayScreen(void)
{
    return finishScreen;
}



int getLastPoints(void)
{
    return points;
}