#pragma once

#include "include/raylib.h"
#include "include/raylib-cpp.hpp"
#include "obstacle.hpp"
#include <random>

class Ball
{
private:
	raylib::Texture2DUnmanaged m_texture;	// Texture
	raylib::Vector2 m_pos {};				// Position
	raylib::Vector2 m_velocity {};			// Velocity
	float m_maxXVelocity {2000.0f};
	float m_maxYVelocity {2000.0f};
	float m_width {};
	float m_height {};
	float m_radius {};
	float m_scale {};
	float m_baseSpeed {500.0f};			// Base speed
	float m_speed {m_baseSpeed};		// Current speed
	float m_maxSpeed {1250.0f};			// Max speed
	std::mt19937& m_mt;

	bool m_frozen {};
	float m_freezeDuration {};
	float m_freezeTimer {};
public:
	Ball(const raylib::Texture2DUnmanaged& texture, std::mt19937& mt);
	~Ball();
	void update(float deltaTime);
	void draw();
	void reset();
	raylib::Vector2 getPos() const { return m_pos; }
	raylib::Vector2 getVelocity() const { return m_velocity; }
	float getRadius() const { return m_radius; }
	raylib::Vector2 getCenterPos();
	float getSpeed() const { return m_speed; }
	void setSpeed(float newSpeed) { m_speed = newSpeed; }
	void setVelocity(raylib::Vector2 newVelocity) { m_velocity = newVelocity; }
	void setRandomVelocity();
	void onCollisionPaddles(raylib::Vector2 paddlePos, int paddleHeight);
	void onCollisionWalls();
	void onCollisionObstacles(const Obstacle& obstacle);
	void freeze(float freezeDuration);
};