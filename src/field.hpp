#pragma once

#include "player.hpp"
#include "computer.hpp"
#include "ball.hpp"

typedef enum FieldType { DEFAULT = 0, POWERUP, OBSTACLE, INVERTED, DARK} FieldType;

class Field
{	
protected:
	std::unique_ptr<Player>& m_player;
	std::unique_ptr<Computer>& m_computer;
	std::unique_ptr<Ball>& m_ball;
public:
	Field(
		std::unique_ptr<Player>& player,
		std::unique_ptr<Computer>& computer,
		std::unique_ptr<Ball>& ball);
	virtual ~Field();
	virtual void update(float deltaTime);
	virtual void draw();
};