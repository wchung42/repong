#include "raylib.h"
#include "./include/raylib-cpp.hpp"
#include "screen.hpp"
#include "title_screen.hpp"

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

TitleScreen::TitleScreen()
{

}

TitleScreen::~TitleScreen()
{

}

// Title Screen Update logic
void TitleScreen::updateScreen()
{
    // TODO: Update TITLE screen variables here!

    // Press enter or tap to change to GAMEPLAY screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        //finishScreen = 1;   // OPTIONS
        m_nextScreen = 2;   // GAMEPLAY
    }
}

// Title Screen Draw logic
void TitleScreen::drawScreen()
{
    // TODO: Draw TITLE screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
    raylib::Vector2 titlePosition {20, 10};
    raylib::DrawText("TITLE SCREEN", titlePosition.GetX(), titlePosition.GetY(), 20, DARKGREEN);
    raylib::Vector2 instructionPosition = {120, 220};
    raylib::DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", instructionPosition.GetX(), instructionPosition.GetY(), 30, DARKGREEN);
}
