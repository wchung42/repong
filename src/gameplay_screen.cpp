#include "screen.hpp"
#include "gameplay_screen.hpp"
#include "paddle.hpp"
#include "field.hpp"
#include "utils.hpp"
#include <iostream>

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

GameplayScreen::GameplayScreen()
    : m_mt((std::random_device())())
{
    m_currentField = DEFAULT;
    m_player = std::make_unique<Player>();
    m_computer = std::make_unique<Computer>();
    m_ball = std::make_unique<Ball>();
    m_field = std::make_unique<Field>(m_player, m_computer, m_ball);

    // Load game textures
    utils::loadTextures({
        "./src/resources/textures/freeze.png",
        "./src/resources/textures/speed_up.png"
    }, m_textures);

}

GameplayScreen::~GameplayScreen() 
{
    // Unload resources
}


// Gameplay Screen Update logic
void GameplayScreen::updateScreen()
{
    float deltaTime {GetFrameTime()};

    // change fields when someone scores
    if (m_field->shouldChangeFields())
    {
        std::uniform_int_distribution<int> distField(0, 100);
        int nextField {distField(m_mt)};
        if (nextField < 24)
        {
            m_field = std::make_unique<Field>(m_player, m_computer, m_ball);
            std::cout << "regular field" << '\n';
        }
        else if (nextField >= 25 && nextField <= 49)
        {
            m_field = std::make_unique<InvertedField>(m_player, m_computer, m_ball);
            std::cout << "inverted field" << '\n';
        }
        else if (nextField >= 50 && nextField <= 74)
        {
            m_field = std::make_unique<PowerUpField>(m_player, m_computer, m_ball, m_textures);
        }
        else if (nextField >= 75)
        {
            m_field = std::make_unique<ObstacleField>(m_player, m_computer, m_ball);
        }
    }
    else
    {
        m_field->update(deltaTime);
    }
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