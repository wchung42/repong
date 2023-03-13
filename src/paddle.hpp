#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"

class Paddle
{
protected:
	raylib::Vector2 m_pos {};
	float m_velocity {};
	int m_width {};
	int m_height {};
public:
	Paddle();
	~Paddle();
	void update(float deltaTime);
	void draw();
	raylib::Rectangle getCollisionRec();
	raylib::Vector2 getPos() { return m_pos; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
};