#pragma once

#include "screen.hpp"

class TitleScreen : public Screen
{
public:
	TitleScreen();
	~TitleScreen();
	void updateScreen();
	void drawScreen();
};