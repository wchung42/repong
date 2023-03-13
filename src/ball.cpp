#define _USE_MATH_DEFINES
#include <cmath>
#include "ball.hpp"
#include <iostream>

Ball::Ball()
	: m_mt((std::random_device())())
{
	m_pos = raylib::Vector2 {
		static_cast<float>(GetScreenWidth() / 2),
		static_cast<float>(GetScreenHeight() / 2)
	};
	std::uniform_int_distribution<int> distAngle(5, 75);
	int angleInDegree = distAngle(m_mt);
	float angleInRadians = (angleInDegree * M_PI) / 180.0f;
	std::uniform_int_distribution<int> distDirection(0, 100);
	int direction = distDirection(m_mt) <= 49 ? -1 : 1;
	m_velocity = raylib::Vector2 {
		static_cast<float>(direction * m_speed * std::cos(angleInRadians)),
		static_cast<float>(m_speed * -std::sin(angleInRadians))
	};
}

Ball::~Ball() {}

void Ball::update(float deltaTime)
{
	m_pos += m_velocity * deltaTime;
}

void Ball::draw()
{
	m_pos.DrawCircle(m_radius, raylib::Color {0, 0, 0, 255});
}

void Ball::reset()
{
	m_pos = raylib::Vector2 {
		static_cast<float>(GetScreenWidth() / 2),
		static_cast<float>(GetScreenHeight() / 2)
	};
}

void Ball::onCollisionPaddles(raylib::Vector2 paddlePos, int paddleHeight)
{
	float relativeIntersectY = (paddlePos.GetY() + (static_cast<float>(paddleHeight / 2))) - m_pos.GetY();
	// Normalize relative y intersect
	float normalizedRelativeIntersectionY = (relativeIntersectY / (static_cast<float>(paddleHeight) / 2.0f));
	// Calculate new bounce angle
	float maxBounceAngle = (5.0f * M_PI) / 12.0f;
	float newBounceAngle = normalizedRelativeIntersectionY * maxBounceAngle;
	float direction = m_velocity.GetX() < 0.0f ? 1.0f : -1.0f;
	float newBallVx = m_speed * direction * std::cos(newBounceAngle);
	float newBallVy = m_speed * -std::sin(newBounceAngle);
	m_velocity = raylib::Vector2 {newBallVx, newBallVy};
}

void Ball::changeXVelocityDirection()
{
	m_velocity.x *= -1;
}

void Ball::changeYVelocityDirection()
{
	m_velocity.y *= -1;
}