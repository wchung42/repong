#pragma once

#include "screen.hpp"

class EndingScreen : public Screen
{
public:
	EndingScreen();
	~EndingScreen();
	void updateScreen();
	void drawScreen();
};