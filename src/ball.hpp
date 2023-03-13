#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"
#include <random>

class Ball
{
private:
	raylib::Vector2 m_pos {};
	raylib::Vector2 m_velocity {};
	int m_radius {10};
	float m_speed {500.0f};				// Base speed
	std::mt19937 m_mt;
public:
	Ball();
	~Ball();
	void update(float deltaTime);
	void draw();
	void reset();
	int getRadius() { return m_radius; }
	raylib::Vector2 getPos() { return m_pos; }
	raylib::Vector2 getVelocity() { return m_velocity; }
	void setVelocity(raylib::Vector2 newVelocity) { m_velocity = newVelocity; }
	void onCollisionPaddles(raylib::Vector2 paddlePos, int paddleHeight);
	void changeXVelocityDirection();
	void changeYVelocityDirection();
};