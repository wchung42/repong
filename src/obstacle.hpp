#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"

class Obstacle
{
private:
	raylib::Vector2 m_pos {};
	raylib::Vector2 m_size {};
	int m_health {};
public:
	Obstacle(
		raylib::Vector2 pos,
		raylib::Vector2 size,
		int health
	);
	~Obstacle();
	void update(float deltaTime);
	void draw();
	raylib::Rectangle getCollisionRec();
	raylib::Vector2 getPos() const { return m_pos; }
	raylib::Vector2 getSize() const { return m_size; }
	int getHealth() { return m_health; }
	void takeDamage() { --m_health; }
};