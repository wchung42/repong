#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"
#include "paddle.hpp"
#include "field.hpp"
#include "utils.hpp"
#include "button.hpp"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING, EXIT } GameScreen;

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
};


// Title screen class declaration
class TitleScreen : public Screen
{
private:
	std::unordered_map<std::string, raylib::Texture2DUnmanaged> m_textures;
	raylib::Font& m_font;
	raylib::Text m_title {};
	raylib::Vector2 m_titlePos {};
	Button m_playButton;
	Button m_quitButton;
public:
	TitleScreen(raylib::Font& m_font);
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
	std::unordered_map<std::string, Sound> m_sounds;
	std::string& m_winner;
	raylib::Font& m_font;
public:
	GameplayScreen(std::string& winner, raylib::Font& font);
	~GameplayScreen();
	void updateScreen();
	void drawScreen();
};


// Ending screen class declaration
class EndingScreen : public Screen
{
private:
	std::unordered_map<std::string, raylib::Texture2DUnmanaged> m_textures;
	std::string m_winner;
	raylib::Text m_winnerText;
	raylib::Vector2 m_winnerTextPos;
	Button m_playAgainButton;
	Button m_quitButton;
	raylib::Font& m_font;
public:
	EndingScreen(std::string& winner, raylib::Font& m_font);
	~EndingScreen();
	void updateScreen();
	void drawScreen();
};