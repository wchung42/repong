#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"
#include "ball.hpp"
#include <memory>

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


// Player paddle class declaration
class Player : public Paddle
{
private:
	bool m_controlsInverted {};
public:
	Player();
	~Player();
	void update(float deltaTime);
	void invertControls(bool invert) { m_controlsInverted = invert; }
};


// Computer paddle class declaration
class Computer : public Paddle
{
public:
	Computer();
	~Computer();
	void update(std::unique_ptr<Ball>& ball, float deltaTime);
};