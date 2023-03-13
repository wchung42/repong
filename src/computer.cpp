#include "computer.hpp"

Computer::Computer()
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
