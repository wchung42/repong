#pragma once

#include "raylib.h"
#include "./include/raylib-cpp.hpp"
#include "screen.hpp"

class TitleScreen : public Screen
{
public:
	void InitScreen();
	void UpdateScreen();
	void DrawScreen();
	void UnloadScreen();
};