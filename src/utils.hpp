#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"
#include <unordered_map>

namespace utils
{
	void loadTextures(
		const std::vector<std::string>& texturePaths,
		std::unordered_map<std::string, raylib::Texture2DUnmanaged>& textures
	);

	void loadSounds(
		const std::vector<std::string>& soundPaths,
		std::unordered_map<std::string, Sound>& sounds
	);
}