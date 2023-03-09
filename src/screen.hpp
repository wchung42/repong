#pragma once

#include "raylib.h"
#include "./include/raylib-cpp.hpp"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

class Screen
{
protected:
	int m_framesCounter {};
	int m_finishScreen {};
public:
	Screen();
	~Screen();
	virtual void InitScreen();
	virtual void UpdateScreen();
	virtual void DrawScreen();
	virtual void UnloadScreen();

	int GetFinishScreen() const { return m_finishScreen; } // Return the next screen

};