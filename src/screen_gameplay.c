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
#include "layouts.h"
#include <stdio.h>
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
enum{
    CURSOR_NORMAL_SIZE = 25,
    CURSOR_BIG_SIZE = 60,
};


#define NUMBER_GRID_X       40U
#define NUMBER_GRID_Y       22U
#define MAX_NUM_EXPLOSIONS  10U


GameEnd finishScreen = 0;
#define MAX_TARGETS_ON_SCREEN 10
Vector2 deltas[MAX_TARGETS_ON_SCREEN];
Vector2 positions[MAX_TARGETS_ON_SCREEN];
int radioCursor = CURSOR_NORMAL_SIZE;
float cursor_speed = 5.0F;
bool alive[MAX_TARGETS_ON_SCREEN]={false};
Vector2 mousPosition;

Vector2 cursorPosition;

int points = 0;
bool user_failed = false;
int max_speed = 1;
int points_per_target = 0;
int radio_target = 0;
int required_points = 0;
int balls_per_game = 0;

bool bonusAlive = false;
double bonusMaxTime = 5.0;
double bonusStartTime;    

Vector2 bonusPosition;
Vector2 deltaBonus;
int radioBonus = 20;
Texture2D CoinSpriteTx;
Texture2D explosionSpriteTx;

static int framesCounter = 0;

int bonusCurrentFrame = 0;
int bonusFramesCounter = 0;
int bonnusFramesSpeed = 8;
Rectangle bonusFrameRec;

int explosionCurrentFrame = 0;
int explosionFramesCounter = 0;
int explosionFramesSpeed = 8;
bool activesExplosion[MAX_NUM_EXPLOSIONS] =  {0};
Rectangle explosionFrameRec[MAX_NUM_EXPLOSIONS];
int activesExplosionsFrame[MAX_NUM_EXPLOSIONS] = {0};
Vector2 activesExplosionsPosition[MAX_NUM_EXPLOSIONS] = {0};



int size_grid_x = 0;
int size_grid_y = 0;
float  bricks_scale = 0;


bool brickMap[7][NUMBER_GRID_Y][NUMBER_GRID_X] = 
    {
        {LAYOUT_1},
        {LAYOUT_2},
        {LAYOUT_3},
        {LAYOUT_4},
        {LAYOUT_1},
        {LAYOUT_3},
        {LAYOUT_2},        
    };
 
//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
bool check_bounce_balls(Vector2 *position, int radio, int level, Vector2 *deltas)
{   
    bool bounce = false;

    // Check bounce with 4 walls of the screen 
    if((position->x < radio)||(position->x > (GetScreenWidth() - radio)))
    {
        deltas->x *=(-1);
        bounce = true;
    }
    if((position->y < radio)||(position->y > (GetScreenHeight() - radio)))
    {
        deltas->y *=(-1);
        bounce = true;
    }



    for (int i = 0; i < NUMBER_GRID_Y; i++)
    {
        for (int j = 0; j < NUMBER_GRID_X; j++)
        {
            if (brickMap[level][i][j] == true)
            {

                Rectangle rec = {size_grid_x*j,  size_grid_y*i, size_grid_x, size_grid_y};
                if (CheckCollisionCircleRec(*position, radio, rec))
                {

                    Rectangle rec_1 ={size_grid_x*j ,  size_grid_y*i, size_grid_x, 1};
                    Rectangle rec_2 ={size_grid_x*j ,  size_grid_y*(i+1), size_grid_x, 1};

                    if (CheckCollisionCircleRec(*position, radio, rec_1) || CheckCollisionCircleRec(*position, radio, rec_2) )
                    {
                        deltas->y *=(-1);
                        position->y +=deltas->y;
                    }
                    else
                    {
                        deltas->x *=(-1);
                         position->x +=deltas->x;
                    }
                   
                   
                    bounce = true;
                }

            }

        }

    }

    return bounce;
}

//----------------------------------------------------------------------------------
// 
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

    deltaBonus.x =  GetRandomValue((-1)*max_speed, max_speed);
    deltaBonus.y = GetRandomValue((-1)*max_speed, max_speed);
    bonusPosition.x = GetRandomValue(0, GetScreenWidth());
    bonusPosition.y = GetRandomValue(0, GetScreenHeight()); 
}

//----------------------------------------------------------------------------------
// Gameplay Screen Initialization logic
//----------------------------------------------------------------------------------
void InitGameplayScreen(void)
{
    finishScreen = 0;
    user_failed = false;
    points = 0;
    radioCursor = CURSOR_NORMAL_SIZE;
    
    
    switch (getLevel()%7)
    {
        default:
        case 1:
        {
            points_per_target = 10;
            radio_target = 50;
            required_points = 50;
            balls_per_game = 5;
        }
        break;
        case 2:
        {
            points_per_target = 10;
            radio_target = 40;
            required_points = 80;
            balls_per_game = 4;
            bonusAlive = true;
        }
        break;
        case 3:
        {
            points_per_target = 20;
            radio_target = 30;
            required_points = 100;
            balls_per_game = 5;
        }
        break;
        case 4:
        {
            points_per_target = 20;
            radio_target = 20;
            required_points = 140;
            balls_per_game = 7;
        }
        break;
        case 5:
        {
            points_per_target = 30;
            radio_target = 25;
            required_points = 140;
            balls_per_game = 8;
            bonusAlive = true;
        }
        break;
        case 6:
        {
            points_per_target = 35;
            radio_target = 20;
            required_points = 160;
            balls_per_game = 9;
        }
        break;                
    }

    // Bonus animation
    float scale = ((float)radioBonus*2)/(((float)CoinSprite.width)/5);
    ImageResize(&CoinSprite, (CoinSprite.width*scale), (CoinSprite.height*scale)); 
    CoinSpriteTx = LoadTextureFromImage(CoinSprite); 

    bonusFrameRec.x= 0.0f;
    bonusFrameRec.y= 0.0f;
    bonusFrameRec.width = (float)CoinSpriteTx.width/5;
    bonusFrameRec.height = (float)CoinSpriteTx.height;

    // Explosion animation
    scale = ((float)radio_target*2)/(((float)explosionSprite.width)/6);
    ImageResize(&explosionSprite, (explosionSprite.width*scale), (explosionSprite.height*scale)); 
    explosionSpriteTx = LoadTextureFromImage(explosionSprite); 

    for (int i = 0; i < MAX_NUM_EXPLOSIONS; i++)
    {
        explosionFrameRec[i].x= 0.0f;
        explosionFrameRec[i].y= 0.0f;
        explosionFrameRec[i].width = (float)explosionSpriteTx.width/6;
        explosionFrameRec[i].height = (float)explosionSpriteTx.height;

        activesExplosion[i] = false;
    }
    // Initial cursor position
    cursorPosition.x = (float)GetScreenWidth()/2;
    cursorPosition.y = (float)GetScreenHeight()/2;
    
    // audio
    SetSoundVolume(fxCoin, 0.3);
    SetSoundVolume(balloonPop, 0.1);
    SetSoundPitch(balloonPop, (getLevel()%7)/4);   

    // Grid for bricks
    size_grid_x = (int)(GetScreenWidth()/NUMBER_GRID_X);
    size_grid_y = (int)(GetScreenHeight()/NUMBER_GRID_Y);
    bricks_scale = bricks.width / size_grid_x;
   
    
    for (int i = 0; i < MAX_TARGETS_ON_SCREEN; i++)
    {
        if (i < balls_per_game) alive[i] = true;
        else alive[i] = false;
    }

    recalculate_deltas(5);


}


//----------------------------------------------------------------------------------
// Gameplay Screen Update logic
//----------------------------------------------------------------------------------
void UpdateGameplayScreen(void)
{
    framesCounter++;
    if(framesCounter%2 == 0)
    {   
        // animation of bonus
        bonusCurrentFrame++;
        if (bonusCurrentFrame > 5) bonusCurrentFrame = 0;
        bonusFrameRec.x = (float)bonusCurrentFrame*(float)CoinSprite.width/5;

        // animation of explosions
        for (int i = 0; i < MAX_NUM_EXPLOSIONS; i++)
        {
            if(activesExplosion[i] == true)
            {
                activesExplosionsFrame[i]++;
                if (activesExplosionsFrame[i] > 6)
                {
                    activesExplosionsFrame[i] = 0;
                    activesExplosion[i] = false;
                }   
                explosionFrameRec[i].x = (float)activesExplosionsFrame[i]*(float)explosionSprite.width/6;
            }
        }

    }


    // Check cursor size after bonus
    if (radioCursor == CURSOR_BIG_SIZE)
    {  
        if(GetTime() >= (bonusMaxTime + bonusStartTime))
        {
            radioCursor = CURSOR_NORMAL_SIZE;
        }
    }

    // check if balls bounce  
    for (int i = 0; i < balls_per_game; i++)  
    {
        positions[i].x += deltas[i].x;
        positions[i].y +=  deltas[i].y;

        bool bounce = check_bounce_balls(&positions[i], radio_target,getLevel()%7 , &deltas[i]);

    }

    // bounce of bonus
    if (bonusAlive == true)
    {
        bonusPosition.x +=  deltaBonus.x;
        bonusPosition.y +=  deltaBonus.y;

        (void)check_bounce_balls(&bonusPosition, radioBonus, getLevel(), &deltaBonus);

    }

    // cursor
    cursorPosition = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        bool hit = false;
        for (int i = 0; i < balls_per_game; i++)
        {
            if(CheckCollisionCircles(positions[i], radio_target, cursorPosition, radioCursor))
            {
                alive[i] = false;
                points += points_per_target;
                PlaySound(balloonPop);
                hit = true;

                for (int i = 0; i < MAX_NUM_EXPLOSIONS; i++)
                {
                    if(activesExplosion[i] == false)
                    {
                        activesExplosion[i] = true;
                        activesExplosionsPosition[i].x = cursorPosition.x - radio_target;
                        activesExplosionsPosition[i].y = cursorPosition.y - radio_target;
                        break;
                    }
                }
            }


        }
        if (hit == false)
        {
            user_failed = true;
        }

        if (bonusAlive == true)
        {
            if(CheckCollisionCircles(cursorPosition, radioCursor, bonusPosition, radioBonus))
            {
                radioCursor = CURSOR_BIG_SIZE;
                bonusAlive = false;
                PlaySound(bonus);
                bonusStartTime = GetTime();
                user_failed = false;
            }
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
        PlaySound(gameOver);
    }
    if (points >= required_points)
    {
        finishScreen = GAME_END_WIN;
        PlaySound(fxCoin);
    }
}

//----------------------------------------------------------------------------------
// Gameplay Screen Draw logic
//----------------------------------------------------------------------------------
void DrawGameplayScreen(void)
{
    int a = getLevel()%6;

    float scale = ((float)GetScreenWidth())/((float)backgrounds[a].width);
    Vector2 position ={0,0};
    DrawTextureEx(backgrounds[a], position, 0.0, scale , WHITE);




    DrawText(TextFormat("Points: %02i", points), 10, 40, 40, BLUE);


    for (int i = 0; i < balls_per_game; i++)
    {
        if(alive[i] == true)
        {
          
            float scale = ((float)radio_target * 2)/((float)redBall.width);
            Vector2 position ={positions[i].x - (redBall.width*scale)/2, positions[i].y - (redBall.height*scale)/2};
            DrawTextureEx(redBall, position, 0.0, scale , WHITE);
        }

    }
    int level = getLevel()%7;
    for (int i = 0; i < NUMBER_GRID_Y; i++)
    {
        for (int j = 0; j < NUMBER_GRID_X; j++)
        {
            if (brickMap[level][i][j] == true)
            {
                Vector2 position ={size_grid_x*j, size_grid_y*i};
                DrawTextureEx(bricks, position, 0.0, 1/bricks_scale, WHITE);

            }

        }

    }


    if (bonusAlive == true)
    {
        Vector2 position ={bonusPosition.x - (bonusFrameRec.width)/2, bonusPosition.y - (bonusFrameRec.height)/2};
        DrawTextureRec(CoinSpriteTx, bonusFrameRec, position, WHITE);
        
    }
    if ( radioCursor == CURSOR_BIG_SIZE)
    {
        int t = bonusMaxTime - (GetTime() - bonusStartTime) + 1;
        DrawText(TextFormat("%i", t), cursorPosition.x - 10, cursorPosition.y - 100 , 40, BLUE);
    }


    scale = ((float)radioCursor * 2)/((float)aim.width);
    position.x = cursorPosition.x - (aim.width*scale)/2;
    position.y = cursorPosition.y - (aim.height*scale)/2;
    DrawTextureEx(aim, position, 0.0, scale , WHITE);

     for (int i = 0; i < MAX_NUM_EXPLOSIONS; i++)
    {
        if(activesExplosion[i] == true)
        {
             DrawTextureRec(explosionSpriteTx, explosionFrameRec[i], activesExplosionsPosition[i], WHITE);
        }
    }

}

//----------------------------------------------------------------------------------
// Gameplay Screen Unload logic
//----------------------------------------------------------------------------------
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

//----------------------------------------------------------------------------------
// Gameplay Screen should finish?
//----------------------------------------------------------------------------------
GameEnd FinishGameplayScreen(void)
{
    return finishScreen;
}



int getLastPoints(void)
{
    return points;
}