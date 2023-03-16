#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"
#include "paddle.hpp"
#include "field.hpp"
#include "utils.hpp"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

// Base Screen class declaration
class Screen
{
protected:
	int m_framesCounter {};
	int m_nextScreen {};
public:
	Screen();
	virtual ~Screen();
	virtual void updateScreen();
	virtual void drawScreen();

	int getNextScreen() const { return m_nextScreen; } // Return the next screen

};


// Logo screen class declaration
class LogoScreen : public Screen
{
private:
	int m_logoPositionX {};
	int m_logoPositionY {};
	int m_lettersCount {};
	int m_topSideRecWidth {};
	int m_leftSideRecHeight {};
	int m_bottomSideRecWidth {};
	int m_rightSideRecHeight {};
	int m_state {};					// Logo animation states
	float m_alpha {1.0f};			// Useful for fading
public:
	LogoScreen();
	~LogoScreen();
	void updateScreen();
	void drawScreen();
};


// Options screen class declaration
class OptionsScreen : public Screen
{
public:
	OptionsScreen();
	~OptionsScreen();
	void updateScreen();
	void drawScreen();
	void unloadScreen();
};


// Title screen class declaration
class TitleScreen : public Screen
{
public:
	TitleScreen();
	~TitleScreen();
	void updateScreen();
	void drawScreen();
};


// Gameplay screen class declaration
class GameplayScreen : public Screen
{
private:
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Computer> m_computer;
	std::unique_ptr<Ball> m_ball;
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


// Ending screen class declaration
class EndingScreen : public Screen
{
public:
	EndingScreen();
	~EndingScreen();
	void updateScreen();
	void drawScreen();
};