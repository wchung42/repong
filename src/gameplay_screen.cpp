#include "screen.hpp"
#include "gameplay_screen.hpp"
#include "player.hpp"
#include <iostream>

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

GameplayScreen::GameplayScreen()
{
    m_currentField = DEFAULT;
    m_player = std::make_unique<Player>();
    m_computer = std::make_unique<Computer>();
    m_ball = std::make_unique<Ball>();
    m_field = std::make_unique<Field>(m_player, m_computer, m_ball);
}

GameplayScreen::~GameplayScreen() 
{
    // Unload resources
}


// Gameplay Screen Update logic
void GameplayScreen::updateScreen()
{
    float deltaTime {GetFrameTime()};
    m_field->update(deltaTime);

}

// Gameplay Screen Draw logic
void GameplayScreen::drawScreen()
{
    //DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
    /*m_player->draw();
    m_computer->draw();
    m_ball->draw();*/
    m_field->draw();

}