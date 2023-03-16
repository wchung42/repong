#include "obstacle.hpp"


Obstacle::Obstacle(
	raylib::Vector2 pos,
	raylib::Vector2 size,
	int health
)	: m_pos(pos), m_size(size), m_health(health)
{

}

Obstacle::~Obstacle()
{

}

void Obstacle::update(float deltaTime)
{
	// Clean this up plssssss
	// Literally just shades of green
	switch (m_health) {
		case 1:
			m_color = raylib::Color {18, 144, 119, 255};
			break;
		case 2:
			m_color = raylib::Color {16, 128, 106, 255};
			break;
		case 3:
			m_color = raylib::Color {14, 112, 93, 255};
			break;
		case 4:
			m_color = raylib::Color {10, 80, 66, 255};
			break;
		case 5:
			m_color = raylib::Color {10, 80, 66, 255};
			break;
		case 6:
			m_color = raylib::Color {8, 64, 53, 255};
			break;
		case 7:
			m_color = raylib::Color {6, 48, 39, 255};
			break;
		case 8:
			m_color = raylib::Color {4, 32, 26, 255};
			break;
		case 9:
			m_color = raylib::Color {2, 16, 13, 255};
			break;
	};
	
}

void Obstacle::draw()
{
	this->getCollisionRec().Draw(m_color);
}

raylib::Rectangle Obstacle::getCollisionRec()
{
	raylib::Rectangle rec {m_pos, m_size};
	return rec;
}