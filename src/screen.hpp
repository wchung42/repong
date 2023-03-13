#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

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