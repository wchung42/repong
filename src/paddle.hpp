#pragma once

#include "include/raylib.h"
#include "include/raylib-cpp.hpp"
#include "ball.hpp"
#include <memory>

class Paddle
{
protected:
	raylib::Texture2DUnmanaged m_texture;
	raylib::Vector2 m_pos {};
	float m_velocity {};
	int m_width {};
	int m_height {};
	int m_score {};
public:
	Paddle(const raylib::Texture2DUnmanaged& texture);
	~Paddle();
	void update(float deltaTime);
	void draw();
	raylib::Rectangle getCollisionRec();
	raylib::Vector2 getPos() const { return m_pos; }
	int getScore() const { return m_score; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	void addScore() { ++m_score; }
};


// Player paddle class declaration
class Player : public Paddle
{
private:
	bool m_controlsInverted {};
public:
	Player(const raylib::Texture2DUnmanaged& texture);
	~Player();
	void update(float deltaTime);
	void invertControls(bool invert) { m_controlsInverted = invert; }
};


// Computer paddle class declaration
class Computer : public Paddle
{
public:
	Computer(const raylib::Texture2DUnmanaged& texture);
	~Computer();
	void update(std::unique_ptr<Ball>& ball, float deltaTime);
};