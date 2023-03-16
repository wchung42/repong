#pragma once

#include "screen.hpp"
#include "ball.hpp"
#include "field.hpp"
#include <memory>
#include <unordered_map>

class GameplayScreen : public Screen
{
private:
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Computer> m_computer;
	std::unique_ptr<Ball> m_ball;
	FieldType m_currentField;
	std::unique_ptr<Field> m_field;
	std::mt19937 m_mt;
	std::unordered_map<std::string, raylib::Texture2DUnmanaged> m_textures;
public:
	GameplayScreen();
	~GameplayScreen();
	void updateScreen();
	void drawScreen();
	void unloadScreen();
};