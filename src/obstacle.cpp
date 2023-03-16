#include "obstacle.hpp"
#include <iostream>

Obstacle::Obstacle(
	raylib::Vector2 pos,
	raylib::Vector2 size,
	int health
)	: m_pos(pos), m_size(size), m_health(health)
{
	std::cout << "obstacle created" << '\n';
	std::cout << m_health << '\n';

}

Obstacle::~Obstacle()
{
	std::cout << "obstacle destroyed" << '\n';
}

void Obstacle::update(float deltaTime)
{
	
}

void Obstacle::draw()
{
	this->getCollisionRec().Draw(BLACK);
}

raylib::Rectangle Obstacle::getCollisionRec()
{
	raylib::Rectangle rec {m_pos, m_size};
	return rec;
}