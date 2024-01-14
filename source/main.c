/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>

#include "P_Initializer.h"
#include "P_Time.h"
#include "P_Score.h"
#include "P_Audio.h"

#include "bird.h"
#include "ball.h"

struct Figure
{
    int x;
    int y;
    u16 *gfx;
    int pal;
};

float BirdySpeed;
int Gameplay_state;
int Gameplay_playerScore;
int keys;

#define FigCount 3
#define DefaultBirdxPos 30
#define DefaultBirdyPos 80

float Birdx = DefaultBirdxPos;
float Birdy = DefaultBirdyPos;

struct Figure AllFig[FigCount];

struct Figure f1, f2, f3;

int x1, y, x2, y2, x3, y3;
// All structs

u16 *gfx;
u16 *gfx1;
u16 *gfx2;
u16 *gfx3;

// Time variables (for the chronometer)
int min = 0, sec = 0, msec = 0;

// Timer 2 ISR (chronometer)
void TIMER0_ISR()
{
    msec = (msec + 1) % 1000;
    if (!msec)
    {
        sec = (sec + 1) % 60;
        if (!sec)
            min++;
    }
}

// Update the chronometer every VBlank
void VBLANK_ISR()
{
    updateChronoDisp_Sub(min, sec, msec);
}

void configureGraphics_Main()
{

    // REG_DISPCNT = MODE_5_2D | DISPLAY_BG0_ACTIVE;

    VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE_0x06400000;

    oamInit(&oamMain, SpriteMapping_1D_32, false);
    gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_16Color);
    gfx1 = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_16Color);
    gfx2 = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_16Color);
    gfx3 = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_16Color);
}

void configSprites_Main()
{
    dmaCopy(ballPal, SPRITE_PALETTE, ballPalLen);
    dmaCopy(birdPal, &SPRITE_PALETTE[(ballPalLen) / 2], birdPalLen);

    dmaCopy(ballTiles, gfx, ballTilesLen);
    dmaCopy(ballTiles, gfx1, ballTilesLen);
    dmaCopy(ballTiles, gfx2, ballTilesLen);
    dmaCopy(birdTiles, gfx3, birdTilesLen);

    oamSet(&oamMain,                  // oam handler
           0,                         // Number of sprite
           f1.x, f1.y,                // Coordinates
           0,                         // Priority
           f1.pal,                    // Palette to use
           SpriteSize_32x32,          // Sprite size
           SpriteColorFormat_16Color, // Color format
           gfx,                       // Loaded graphic to display
           -1,                        // Affine rotation to use (-1 none)
           false,                     // Double size if rotating
           false,                     // Hide this sprite
           false, false,              // Horizontal or vertical flip
           false                      // Mosaic
    );

    oamSet(&oamMain,                  // oam handler
           1,                         // Number of sprite
           f2.x, f2.y,                // Coordinates
           0,                         // Priority
           f2.pal,                    // Palette to use
           SpriteSize_32x32,          // Sprite size
           SpriteColorFormat_16Color, // Color format
           gfx1,                      // Loaded graphic to display
           -1,                        // Affine rotation to use (-1 none)
           false,                     // Double size if rotating
           false,                     // Hide this sprite
           false, false,              // Horizontal or vertical flip
           false                      // Mosaic
    );

    oamSet(&oamMain,                  // oam handler
           2,                         // Number of sprite
           f3.x, f3.y,                // Coordinates
           0,                         // Priority
           f3.pal,                    // Palette to use
           SpriteSize_32x32,          // Sprite size
           SpriteColorFormat_16Color, // Color format
           gfx2,                      // Loaded graphic to display
           -1,                        // Affine rotation to use (-1 none)
           false,                     // Double size if rotating
           false,                     // Hide this sprite
           false, false,              // Horizontal or vertical flip
           false                      // Mosaic
    );

    oamSet(&oamMain, 3, Birdx, Birdy, 0, 1, SpriteSize_32x32, SpriteColorFormat_16Color, gfx3, -1,
           false,
           false,
           false, false,
           false);
}

void updateSprites()
{
    for (int i = 0; i < FigCount; i++)
    {
        AllFig[i].x--;

        // If pipes are not offscreen (left)
        if (AllFig[i].x <= 256)
        {
            // If pipes are offscreen (right)
            if (AllFig[i].x <= -32)
            {
                // Reset pipes positon
                AllFig[i].x = 256;
                AllFig[i].y = +rand() % 192; // Add some random
            }

            // Set pipes position
            oamSet(&oamMain, i, AllFig[i].x - 10, AllFig[i].y, 0, AllFig[i].pal, SpriteSize_32x32, SpriteColorFormat_16Color, AllFig[i].gfx, -1, false, false, false, false, false);
        }
        else
        {
            // Set pipes position
            oamSet(&oamMain, i, 256, 0, 0, AllFig[i].pal, SpriteSize_32x32, SpriteColorFormat_16Color, AllFig[i].gfx, -1, false, false, false, false, false);
        }
    }
}

void P_GL_updateGame()
{
    // Read held keys
    scanKeys();
    keys = keysDown();
    // Read the touch position.

	touchPosition touch;
	touchRead(&touch);

    if (Gameplay_state == 0)
    {

        // P_GL_updateGameStart();
        if (keys & KEY_DOWN || touch.px > 0 || touch.py > 0)
        {
            Gameplay_state = 1;
             mmEffect(SFX_JUMP);
        }
    }
    else if (Gameplay_state == 1)
    {

        // Modify position of the sprite accordingly
        if ((keys & KEY_UP) || touch.px > 0 || touch.py > 0)
        {
            BirdySpeed = -3;
            mmEffect(SFX_JUMP);
        }

        if (BirdySpeed < 4)
            BirdySpeed += 0.1;
        if (
            (Birdy < 0) ||
            (Birdy > 192 - 32))
        {
            Gameplay_state = 2;
        }
        else
        {
            Birdy += BirdySpeed;

            oamSet(&oamMain, 3, Birdx, Birdy, 0, 1, SpriteSize_32x32, SpriteColorFormat_16Color, gfx3, -1,
                   false,
                   false,
                   false, false,
                   false);
        }

        for (int i = 0; i < FigCount; i++)
            if (AllFig[i].x - 15 <= Birdx && Birdx <= AllFig[i].x + 15)
                if (AllFig[i].y - 15 <= Birdy && Birdy <= AllFig[i].y + 15)
                {
                    Gameplay_state = 2;
                }
        // Add score
        for (int i = 0; i < FigCount; i++)
            if (AllFig[i].x + 16 == Birdx)
            {
                Gameplay_playerScore++;
            }
    }

    else if (Gameplay_state == 2)
    {
        displayMaxScore();
        oamSet(&oamMain, 3, DefaultBirdxPos, DefaultBirdyPos, 0, 1, SpriteSize_32x32, SpriteColorFormat_16Color, gfx3, -1,
               false,
               true,
               false, false,
               false);

        if (min < 2)
        {
            if (keys & KEY_DOWN)
            {
                Restart_Game();
            }
        }
    }
}

void Restart_Game()
{

    f1.x = 0;
    f1.y = 0;
    f1.pal = 0;

    f2.x = 64;
    f2.y = 64;
    f2.pal = 0;

    f3.x = 128;
    f3.y = 128;
    f3.pal = 0;

    P_InitNDS();
    configureGraphics_Main();
    configSprites_Main();

    f1.gfx = gfx;
    f2.gfx = gfx1;
    f3.gfx = gfx2;

    AllFig[0] = f1;
    AllFig[1] = f2;
    AllFig[2] = f3;
    BirdySpeed = 0;
    Gameplay_state = 0;
    Gameplay_playerScore = 0;
    Birdx = DefaultBirdxPos;
    Birdy = DefaultBirdyPos;
}

void Init_Game()
{
    f1.x = 0;
    f1.y = 0;
    f1.pal = 0;

    f2.x = 64;
    f2.y = 64;
    f2.pal = 0;

    f3.x = 128;
    f3.y = 128;
    f3.pal = 0;

    P_InitNDS();

    configureGraphics_Main();
    configSprites_Main();

    f1.gfx = gfx;
    f2.gfx = gfx1;
    f3.gfx = gfx2;

    AllFig[0] = f1;
    AllFig[1] = f2;
    AllFig[2] = f3;
    BirdySpeed = 0;
    Gameplay_state = 0;
    Gameplay_playerScore = 0;
    Birdx = DefaultBirdxPos;
    Birdy = DefaultBirdyPos;

    min = sec = msec = 0;
    TIMER_DATA(0) = TIMER_FREQ_1024(1000);
    TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
    irqSet(IRQ_TIMER0, &TIMER0_ISR);
    irqEnable(IRQ_TIMER0);
    irqSet(IRQ_VBLANK, &VBLANK_ISR);
    irqEnable(IRQ_VBLANK);
}

int main(void)
{
    Init_Game();

    while (1)
    {
        P_GL_updateGame();
        updateSprites();
        updateScore(Gameplay_playerScore);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }
}
