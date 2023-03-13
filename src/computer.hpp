#pragma once

#include "paddle.hpp"
#include "ball.hpp"

class Computer : public Paddle
{
public:
	Computer();
	~Computer();
	void update(std::unique_ptr<Ball>& ball, float deltaTime);
};