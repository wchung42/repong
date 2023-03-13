#include "paddle.hpp"

Paddle::Paddle()
{
	m_width = 30;
	m_height = m_width * 5;
	m_velocity = 400.0f;
}

Paddle::~Paddle() {}

void Paddle::update(float deltaTime)
{

}

void Paddle::draw()
{
	raylib::Rectangle rec {
		m_pos.x,
		m_pos.y,
		static_cast<float>(m_width), 
		static_cast<float>(m_height)
	};
	rec.Draw(raylib::Color {BLACK});
}

raylib::Rectangle Paddle::getCollisionRec()
{
	raylib::Rectangle rec {
		m_pos.GetX(),
		m_pos.GetY(),
		static_cast<float>(m_width),
		static_cast<float>(m_height)
	};
	return rec;
}