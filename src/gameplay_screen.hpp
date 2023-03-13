#pragma once

#include "screen.hpp"
#include "player.hpp"
#include "computer.hpp"
#include "ball.hpp"
#include <memory>

class GameplayScreen : public Screen
{
private:
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Computer> m_computer;
	std::unique_ptr<Ball> m_ball;
public:
	void InitScreen();
	void UpdateScreen();
	void DrawScreen();
	void UnloadScreen();
};