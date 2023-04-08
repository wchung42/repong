#pragma once

#include "paddle.hpp"
#include "ball.hpp"
#include "powerup.hpp"
#include "obstacle.hpp"
#include "texture_manager.hpp"
#include "sound_manager.hpp"


// Base Field class declarations
class Field
{	
protected:
	std::unique_ptr<Player>& m_player;
	std::unique_ptr<Computer>& m_computer;
	std::unique_ptr<Ball>& m_ball;
	bool m_changeFields {};
	SoundManager* m_soundManager;
public:
	Field(
		std::unique_ptr<Player>& player,
		std::unique_ptr<Computer>& computer,
		std::unique_ptr<Ball>& ball,
		SoundManager* soundManager
	);
	virtual ~Field();
	virtual void update(float deltaTime);
	virtual void draw();
	bool shouldChangeFields() { return m_changeFields; }

};


// Inverted Field class declarations
class InvertedField : public Field
{
public:
	InvertedField(
		std::unique_ptr<Player>& player,
		std::unique_ptr<Computer>& computer,
		std::unique_ptr<Ball>& ball,
		SoundManager* soundManager
	);
	~InvertedField();
	void update(float deltaTime);
	void draw();
};


// Powerup Field class declarations
class PowerUpField : public Field
{
private:
	std::unique_ptr<PowerUpSpawner> m_powerUpSpawner;
	std::vector<std::unique_ptr<PowerUp>> m_powerups;
public:
	PowerUpField(
		std::unique_ptr<Player>& player,
		std::unique_ptr<Computer>& computer,
		std::unique_ptr<Ball>& ball,
		TextureManager* textureManager,
		SoundManager* soundManager,
		std::mt19937& m_mt
	);
	~PowerUpField();
	void update(float deltaTime);
	void draw();
};


// Obstacle Field class
class ObstacleField : public Field
{
private:
	std::vector<Obstacle> m_obstacles;
	std::mt19937 m_mt;
public:
	ObstacleField(
		std::unique_ptr<Player>& player,
		std::unique_ptr<Computer>& computer,
		std::unique_ptr<Ball>& ball,
		SoundManager* soundManager,
		std::mt19937& m_mt
	);
	~ObstacleField();
	void update(float deltaTime);
	void draw();
	void spawnObstacles();
};


// -------------------------
// Lights Out Field
// -------------------------
class LightsOutField : public Field
{
private:
	raylib::RenderTexture m_holeTexture;
public:
	LightsOutField(
		std::unique_ptr<Player>& player,
		std::unique_ptr<Computer>& computer,
		std::unique_ptr<Ball>& ball,
		SoundManager* soundManager
	);
	~LightsOutField();
	void update(float deltaTime);
	void draw();
};