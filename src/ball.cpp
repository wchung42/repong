#define _USE_MATH_DEFINES
#include <cmath>
#include "ball.hpp"
#include "obstacle.hpp"
#include <iostream>

Ball::Ball()
	: m_mt((std::random_device())())
{
	m_pos = raylib::Vector2 {
		static_cast<float>(GetScreenWidth() / 2),
		static_cast<float>(GetScreenHeight() / 2)
	};
	this->setRandomVelocity();
}

Ball::~Ball() {}

void Ball::update(float deltaTime)
{
	if (m_frozen)
	{
		// Zero velocity
		m_velocity = raylib::Vector2 {0.0f, 0.0f};

		m_freezeTimer += deltaTime;
		if (m_freezeTimer >= m_freezeDuration)
		{
			// Calculate new velocity
			this->setRandomVelocity();
			// Unfreeze
			m_frozen = false;
			m_freezeTimer = 0.0f;
		}
	}
	else
		m_pos += m_velocity * deltaTime;
}

void Ball::draw()
{
	m_pos.DrawCircle(m_radius, raylib::Color {0, 0, 0, 255});
}

void Ball::reset()
{
	m_pos = raylib::Vector2 {							// Reset ball position
		static_cast<float>(GetScreenWidth() / 2),
		static_cast<float>(GetScreenHeight() / 2)
	};
	m_speed = m_baseSpeed;								// Reset ball speed
	this->setRandomVelocity();
}

void Ball::setRandomVelocity()
{
	int minAngle {30};
	int maxAngle {60};
	std::uniform_int_distribution<int> distAngle(minAngle, maxAngle);
	int angleInDegree = distAngle(m_mt);
	float angleInRadians = (angleInDegree * M_PI) / 180.0f;
	std::uniform_int_distribution<int> distDirection(0, 100);
	int direction = distDirection(m_mt) <= 49 ? -1 : 1;
	m_velocity = raylib::Vector2 {
		static_cast<float>(direction * m_speed * std::cos(angleInRadians)),
		static_cast<float>(m_speed * -std::sin(angleInRadians))
	};
}

void Ball::onCollisionPaddles(raylib::Vector2 paddlePos, int paddleHeight)
{
	m_speed *= 1.1f;	// Increase speed by 10 percent on collision

	// Calculate new ball velocity
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

void Ball::onCollisionWalls()
{
	m_speed *= 1.05f;		// Increase speed by 5 percent on collision
	m_velocity.y *= -1;		// Change y direction
}

void Ball::onCollisionObstacles(const Obstacle& obstacle)
{
	if ((this->m_pos.GetY() >= obstacle.getPos().GetY() + this->m_radius ||
		this->m_pos.GetY() <= obstacle.getPos().GetY() - this->m_radius) &&
		(this->m_pos.GetX() >= obstacle.getPos().GetX() && 
		this->m_pos.GetX() <= obstacle.getPos().GetX() + obstacle.getSize().x))
	{
		m_velocity.y *= -1;
	}
	else
	{
		m_velocity.x *= -1;
	}

}

void Ball::changeXVelocityDirection()
{
	m_velocity.x *= -1;
}

void Ball::changeYVelocityDirection()
{
	m_velocity.y *= -1;
}

void Ball::freeze(float freezeDuration)
{
	m_frozen = true;
	m_freezeDuration = freezeDuration;
}