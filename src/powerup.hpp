#pragma once

#include "raylib.h"
#include "include/raylib-cpp.hpp"
#include "ball.hpp"
#include <unordered_map>
#include <memory>

// Base powerup class declaration
class PowerUp
{
protected:
	raylib::Texture2DUnmanaged m_texture {};
	raylib::Vector2 m_pos {};
	float m_scale {};
	int m_width {};
	int m_height {};
public:
	PowerUp();
	PowerUp(
		raylib::Texture2DUnmanaged texture,
		raylib::Vector2	pos
	);
	virtual ~PowerUp();
	void draw();
	raylib::Rectangle getCollisionRec();
	virtual void onCollision(
		std::unique_ptr<Ball>& ball,
		std::unordered_map<std::string, Sound>& sounds
	);
};


// Speed powerup class declaration
class SpeedPowerUp : public PowerUp
{
private:
	float m_speedMultiplier {};
public:
	SpeedPowerUp(
		raylib::Texture2DUnmanaged texture,
		raylib::Vector2 pos,
		float speedMultiplier
	);
	~SpeedPowerUp();
	void onCollision(std::unique_ptr<Ball>& ball,
		std::unordered_map<std::string, Sound>& sounds) override;
};


// Freeze powerup class declaration
class FreezePowerUp : public PowerUp
{
private:
	float m_freezeDuration {};
public:
	FreezePowerUp(
		raylib::Texture2DUnmanaged texture,
		raylib::Vector2 pos,
		float freezeDuration
	);
	~FreezePowerUp();
	void onCollision(
		std::unique_ptr<Ball>& ball,
		std::unordered_map<std::string, Sound>& sounds) override;
};


// Powerup spawner class declaration
class PowerUpSpawner
{
private:
	std::mt19937 m_mt;
	std::unordered_map<std::string, raylib::Texture2DUnmanaged>& m_textures;
	float m_spawnRate {};
	float m_spawnTimer {};
	int m_maxPowerups {};
	int m_powerupCount {};
public:
	PowerUpSpawner(std::unordered_map<std::string, raylib::Texture2DUnmanaged>& textures);
	~PowerUpSpawner();
	void update(float deltaTime, std::vector<std::unique_ptr<PowerUp>>& powerUps);
	std::unique_ptr<PowerUp> spawnPowerUp();
	void decrementPowerupCount() { --m_powerupCount; }
};