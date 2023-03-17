#include "utils.hpp"

void utils::loadTextures(
	const std::vector<std::string>& texturePaths,
	std::unordered_map<std::string, raylib::Texture2DUnmanaged>& textures)
{
    // Load textures into GPU
    for (auto& path : texturePaths)
    {
        raylib::Texture2DUnmanaged texture(path);
        // Get name of file as key
        std::string baseFilename = path.substr(path.find_last_of("/\\") + 1);
        std::string::size_type const p(baseFilename.find_last_of('.'));
        std::string fileNameWithoutExtension = baseFilename.substr(0, p);
        textures[fileNameWithoutExtension] = texture;
    }

    // Set textures to filter_bilinear flag for better scaling
    for (auto& [name, texture] : textures)
    {
        SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    }
}

void utils::loadSounds(
    const std::vector<std::string>& soundPaths,
    std::unordered_map<std::string, Sound>& sounds)
{
    // Load sounds
    for (auto& path : soundPaths)
    {
        Sound sound = LoadSound(path.c_str());
        // Get name of file as key
        std::string baseFilename = path.substr(path.find_last_of("/\\") + 1);
        std::string::size_type const p(baseFilename.find_last_of('.'));
        std::string fileNameWithoutExtension = baseFilename.substr(0, p);
        sounds[fileNameWithoutExtension] = sound;
    }
}