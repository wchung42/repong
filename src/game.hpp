#pragma once

#include "screen.hpp"
#include "texture_manager.hpp"
#include "sound_manager.hpp"
#include <string>
#include <memory>

class Game
{
public:
	GameScreen m_currentScreen = LOGO;
private:
	// Resource managers and resources
	std::unique_ptr<TextureManager> m_textureManager;
	std::unique_ptr<SoundManager> m_soundManager;
	raylib::Font m_font;
	raylib::AudioDevice m_audio;

	// Game window
	raylib::Window m_window {};
	int m_windowWidth {1280};
	int m_windowHeight {m_windowWidth / 16 * 9};
	int m_targetFPS {60};

	// Game attributes
	std::string m_title {};
	bool m_exitGame {};

	// Screen variables
	float m_transAlpha {0.0f};
	bool m_onTransition {};
	bool m_transFadeOut {};
	int m_transFromScreen {-1};
	std::unique_ptr<Screen> m_screen;
	GameScreen m_transToScreen = UNKNOWN;
	std::string m_winner {};

public:
	Game();
	~Game();
	// Initialize the game
	void initialize();
	// Run the game loop
	void runLoop();
	// Shutdown game properly
	void shutdown();
private:
	// Helper functions for running the game loop
	void updateGame(float deltaTime);
	void renderGame();
	void changeToScreen(GameScreen screen);		// Change to screen, no transition effect
	void transitionToScreen(GameScreen screen); // Request transition to next screen
	void updateTransition();					// Update transition effect
	void drawTransition();						// Draw transition effect (fullscreen rectangle)
};