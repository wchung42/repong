#include "field.hpp"
#include <iostream>

Field::Field(
	std::unique_ptr<Player>& player,
	std::unique_ptr<Computer>& computer,
	std::unique_ptr<Ball>& ball)
	: m_player(player), m_computer(computer), m_ball(ball)
{

}

Field::~Field() {}

void Field::update(float deltaTime)
{
    ///////////////////////
    // Handle collisions //
    ///////////////////////

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

    m_player->update(deltaTime);
    m_computer->update(m_ball, deltaTime);
    m_ball->update(deltaTime);
}

void Field::draw()
{
	m_player->draw();
	m_computer->draw();
	m_ball->draw();
}