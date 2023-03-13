#include "screen.hpp"
#include "gameplay_screen.hpp"
#include "player.hpp"
#include <iostream>

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void GameplayScreen::InitScreen()
{
    m_player = std::make_unique<Player>();
    m_computer = std::make_unique<Computer>();
    m_ball = std::make_unique<Ball>();
}

// Gameplay Screen Update logic
void GameplayScreen::UpdateScreen()
{
    float deltaTime {GetFrameTime()};
    m_player->update(deltaTime);
    m_computer->update(m_ball, deltaTime);
    m_ball->update(deltaTime);

    //////////////////////
    // Handle collisions
    //////////////////////
    
    // Ball collision with top and bottom walls
    if ((m_ball->getPos().GetY() + m_ball->getRadius() >= GetScreenHeight()) ||
        (m_ball->getPos().GetY() - m_ball->getRadius() <= 0))
    {
        m_ball->changeYVelocityDirection();
    }

    // Ball collision with left and right walls
    if (m_ball->getPos().GetX() + m_ball->getRadius() >= GetScreenWidth())
    {
        std::cout << "player score" << '\n';
        m_ball->reset();
    }
    else if (m_ball->getPos().GetX() - m_ball->getRadius() <= 0)
    {
        std::cout << "Computer score" << '\n';
        m_ball->reset();
    }

    // Ball collision with paddles
    if (CheckCollisionCircleRec(m_ball->getPos(), m_ball->getRadius(), m_player->getCollisionRec()))
    {
        if (m_ball->getVelocity().GetX() < 0)
        {
            m_ball->onCollisionPaddles(m_player->getPos(), m_player->getHeight());
        }
    };

    if (CheckCollisionCircleRec(m_ball->getPos(), m_ball->getRadius(), m_computer->getCollisionRec()))
    {
        if (m_ball->getVelocity().GetX() > 0)
            m_ball->onCollisionPaddles(m_computer->getPos(), m_computer->getHeight());
    }
}

// Gameplay Screen Draw logic
void GameplayScreen::DrawScreen()
{
    //DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
    m_player->draw();
    m_computer->draw();
    m_ball->draw();

}

// Gameplay Screen Unload logic
void GameplayScreen::UnloadScreen()
{
    // TODO: Unload GAMEPLAY screen variables here!
}