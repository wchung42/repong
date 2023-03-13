#pragma once

#include "paddle.hpp"

class Player : public Paddle
{
private:
public:
	Player();
	~Player();
	void update(float deltaTime);
};