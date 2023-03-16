#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"
#include "obstacle.hpp"
#include <random>

class Ball
{
private:
	raylib::Vector2 m_pos {};
	raylib::Vector2 m_velocity {};
	int m_radius {10};
	float m_baseSpeed {420.0f};			// Base speed
	float m_speed {m_baseSpeed};		// Current speed
	std::mt19937 m_mt;

	bool m_frozen {};
	float m_freezeDuration {};
	float m_freezeTimer {};
public:
	Ball();
	~Ball();
	void update(float deltaTime);
	void draw();
	void reset();
	raylib::Vector2 getPos() const { return m_pos; }
	raylib::Vector2 getVelocity() const { return m_velocity; }
	int getRadius() const { return m_radius; }
	float getSpeed() const { return m_speed; }
	void setSpeed(float newSpeed) { m_speed = newSpeed; }
	void setVelocity(raylib::Vector2 newVelocity) { m_velocity = newVelocity; }
	void setRandomVelocity();
	void onCollisionPaddles(raylib::Vector2 paddlePos, int paddleHeight);
	void onCollisionWalls();
	void onCollisionObstacles(const Obstacle& obstacle);
	void changeXVelocityDirection();
	void changeYVelocityDirection();
	void freeze(float freezeDuration);
};