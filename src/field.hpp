#pragma once

#include "paddle.hpp"
#include "ball.hpp"
#include "powerup.hpp"
#include "obstacle.hpp"


// Base Field class declarations
class Field
{	
protected:
	std::unique_ptr<Player>& m_player;
	std::unique_ptr<Computer>& m_computer;
	std::unique_ptr<Ball>& m_ball;
	bool m_changeFields {};
	std::unordered_map<std::string, Sound>& m_sounds;
public:
	Field(
		std::unique_ptr<Player>& player,
		std::unique_ptr<Computer>& computer,
		std::unique_ptr<Ball>& ball,
		std::unordered_map<std::string, Sound>& sounds
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
		std::unordered_map<std::string, Sound>& sounds
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
		std::unordered_map<std::string, raylib::Texture2DUnmanaged>& textures,
		std::unordered_map<std::string, Sound>& sounds,
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
		std::unordered_map<std::string, Sound>& sounds,
		std::mt19937& m_mt
	);
	~ObstacleField();
	void update(float deltaTime);
	void draw();
	void spawnObstacles();
};