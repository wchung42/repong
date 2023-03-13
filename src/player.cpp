#include "player.hpp"

Player::Player()
{
	m_pos = raylib::Vector2 {
				static_cast<float>(GetScreenWidth() * 0.02f),
				static_cast<float>(GetScreenHeight() / 2 - m_height / 2)
	};
}

Player::~Player() {}

void Player::update(float deltaTime)
{
	if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)))
		m_pos.y -= m_velocity * deltaTime;
	
	if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)))
		m_pos.y += m_velocity * deltaTime;

	Paddle::update(deltaTime);
}