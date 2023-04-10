#include "paddle.hpp"

// Paddle class function declarations
Paddle::Paddle(const raylib::Texture2DUnmanaged& texture)
	: m_texture(texture)
{
	m_width = m_texture.GetWidth();
	m_height = m_texture.GetHeight();
	m_velocity = 450.0f;
}

Paddle::~Paddle() {}

void Paddle::update(float deltaTime)
{
	if (m_pos.GetY() < 5.0f)	// Account for screen border of thickness 5px
		m_pos.y = 5.0f;
	else if (m_pos.GetY() + m_height > GetScreenHeight())
		m_pos.y = GetScreenHeight() - m_height - 5.0f;
}

void Paddle::draw()
{
	// Draw paddle texture
	m_texture.Draw(m_pos, WHITE);
}

raylib::Rectangle Paddle::getCollisionRec()
{
	raylib::Rectangle rec {
		m_pos.GetX(),
		m_pos.GetY(),
		static_cast<float>(m_width),
		static_cast<float>(m_height)
	};
	return rec;
}


// Paddle class function declarations
Player::Player(const raylib::Texture2DUnmanaged& texture)
	: Paddle(texture)
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
	{
		if (m_controlsInverted)
			m_pos.y += m_velocity * deltaTime;
		else
			m_pos.y -= m_velocity * deltaTime;
	}

	if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)))
	{
		if (m_controlsInverted)
			m_pos.y -= m_velocity * deltaTime;
		else
			m_pos.y += m_velocity * deltaTime;
	}

	Paddle::update(deltaTime);
}


// Computer class function declarations
Computer::Computer(const raylib::Texture2DUnmanaged& texture)
	: Paddle(texture)
{
	m_pos = raylib::Vector2 {
		static_cast<float>(GetScreenWidth() * 0.98f - m_width),
		static_cast<float>(GetScreenHeight() / 2 - m_height / 2)
	};
}

Computer::~Computer() {}

void Computer::update(std::unique_ptr<Ball>& ball, float deltaTime)
{
	// Follows the ball
	if (ball->getVelocity().GetX() > 0.0f)
	{
		raylib::Vector2 ballPos = ball->getPos();
		if (m_pos.GetY() + (m_height / 2) > ballPos.GetY())
			m_pos.y -= m_velocity * deltaTime;

		if (m_pos.GetY() + (m_height / 2) <= ballPos.GetY())
			m_pos.y += m_velocity * deltaTime;
	}

	Paddle::update(deltaTime);
}