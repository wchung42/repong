#pragma once

#include "screen.hpp"

class OptionsScreen : public Screen
{
public:
	OptionsScreen();
	~OptionsScreen();
	void updateScreen();
	void drawScreen();
	void unloadScreen();
};