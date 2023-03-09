#pragma once

#include "raylib.h"
#include "./include/raylib-cpp.hpp"
#include "screen.hpp"

class EndingScreen : public Screen
{
public:
	void InitScreen();
	void UpdateScreen();
	void DrawScreen();
	void UnloadScreen();
};