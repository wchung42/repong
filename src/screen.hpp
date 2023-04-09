#pragma once

#include "include/raylib.h"
#include "include/raylib-cpp.hpp"
#include "paddle.hpp"
#include "field.hpp"
#include "utils.hpp"
#include "button.hpp"
#include "texture_manager.hpp"
#include "sound_manager.hpp"

//-------------------------------------
// Type Definitions
//-------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING, EXIT } GameScreen;

//-------------------------------------
// Base Screen Declaration
//-------------------------------------
class Screen
{
protected:
	int m_framesCounter {};
	int m_nextScreen {};
	TextureManager* m_textureManager {nullptr};
	SoundManager* m_soundManager {nullptr};
public:
	Screen();
	Screen(
		std::unique_ptr<TextureManager>& textureManager,
		std::unique_ptr<SoundManager>& soundManager
	);
	virtual ~Screen();
	virtual void updateScreen();
	virtual void drawScreen();

	int getNextScreen() const { return m_nextScreen; } // Return the next screen

};


//-------------------------------------
// Logo Screen Declaration
//-------------------------------------
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


//-------------------------------------
// Options Screen Declaration
//-------------------------------------
class OptionsScreen : public Screen
{
public:
	OptionsScreen(
		std::unique_ptr<TextureManager>& textureManager,
		std::unique_ptr<SoundManager>& soundManager
	);
	~OptionsScreen();
	void updateScreen();
	void drawScreen();
};


//-------------------------------------
// Title Screen Declaration
//-------------------------------------
class TitleScreen : public Screen
{
private:
	std::unordered_map<std::string, raylib::Texture2DUnmanaged> m_textures;
	raylib::Font& m_font;
	raylib::Texture2DUnmanaged m_title {};
	raylib::Vector2 m_titlePos {};
	Button m_playButton;
	Button m_quitButton;
public:
	TitleScreen(
		raylib::Font& m_font,
		std::unique_ptr<TextureManager>& textureManager,
		std::unique_ptr<SoundManager>& soundManager
	);
	~TitleScreen();
	void updateScreen();
	void drawScreen();
};


//-------------------------------------
// Gameplay Screen Declaration
//-------------------------------------
class GameplayScreen : public Screen
{
private:
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Computer> m_computer;
	std::unique_ptr<Ball> m_ball;
	std::unique_ptr<Field> m_field;
	std::mt19937 m_mt;
	std::string& m_winner;
	raylib::Font& m_font;
	int m_maxScore {1};
public:
	GameplayScreen(
		std::string& winner,
		raylib::Font& font,
		std::unique_ptr<TextureManager>& textureManager,
		std::unique_ptr<SoundManager>& soundManager
	);
	~GameplayScreen();
	void updateScreen();
	void drawScreen();
};


//-------------------------------------
// Ending Screen Declaration
//-------------------------------------
class EndingScreen : public Screen
{
private:
	std::string m_winner;
	raylib::Text m_winnerText;
	raylib::Vector2 m_winnerTextPos;
	Button m_playAgainButton;
	Button m_quitButton;
	raylib::Font& m_font;
public:
	EndingScreen(
		std::string& winner,
		raylib::Font& m_font,
		std::unique_ptr<TextureManager>& textureManager,
		std::unique_ptr<SoundManager>& soundManager
	);
	~EndingScreen();
	void updateScreen();
	void drawScreen();
};