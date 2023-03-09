#include "raylib.h"
#include "./include/raylib-cpp.hpp"
#include "screen.hpp"
#include "gameplay_screen.hpp"

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void GameplayScreen::InitScreen()
{
    // TODO: Initialize GAMEPLAY screen variables here!
    m_framesCounter = 0;
    m_finishScreen = 0;
}

// Gameplay Screen Update logic
void GameplayScreen::UpdateScreen()
{
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        m_finishScreen = 1;
    }
}

// Gameplay Screen Draw logic
void GameplayScreen::DrawScreen()
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    raylib::Vector2 titlePosition {20, 10};
    raylib::DrawText("GAMEPLAY SCREEN", titlePosition.GetX(), titlePosition.GetY(), 20, MAROON);
    raylib::Vector2 instructionPosition {120, 220};
    raylib::DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", instructionPosition.GetX(), instructionPosition.GetY(), 30, MAROON);
}

// Gameplay Screen Unload logic
void GameplayScreen::UnloadScreen()
{
    // TODO: Unload GAMEPLAY screen variables here!
}