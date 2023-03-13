#include "raylib.h"
#include "./include/raylib-cpp.hpp"
#include "screen.hpp"
#include "ending_screen.hpp"

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

EndingScreen::EndingScreen()
{

}

EndingScreen::~EndingScreen()
{
    // Unload resources
}

// Ending Screen Update logic
void EndingScreen::updateScreen()
{
    // TODO: Update ENDING screen variables here!

    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        m_nextScreen = 1;
        //PlaySound(fxCoin);
    }
}

// Ending Screen Draw logic
void EndingScreen::drawScreen()
{
    // TODO: Draw ENDING screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
    raylib::Vector2 titlePosition {20, 10};
    raylib::DrawText("ENDING SCREEN", titlePosition.GetX(), titlePosition.GetY(), 20, DARKBLUE);
    raylib::Vector2 instructionPosition {120, 220};
    raylib::DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", instructionPosition.GetX(), instructionPosition.GetY(), 30, DARKBLUE);
}