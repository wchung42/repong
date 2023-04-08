#include "powerup.hpp"

// ---------------------------------------
// Base power up function definitions
// ---------------------------------------
PowerUp::PowerUp()
{

}

PowerUp::PowerUp(
	raylib::Texture2DUnmanaged texture,
	raylib::Vector2 pos
) 
	: m_texture(texture), m_pos(pos)
{
	m_scale = 0.12f;
	m_width = m_texture.GetWidth() * m_scale;
	m_height = m_texture.GetHeight() * m_scale;
}

PowerUp::~PowerUp()
{

}

void PowerUp::draw()
{
	m_texture.Draw(m_pos, 0.0f, m_scale, WHITE);
}

raylib::Rectangle PowerUp::getCollisionRec()
{
	raylib::Rectangle rec {
		m_pos.GetX(),
		m_pos.GetY(),
		static_cast<float>(m_width),
		static_cast<float>(m_height)
	};
	return rec;
}

void PowerUp::onCollision(
	std::unique_ptr<Ball>& ball,
	SoundManager* soundManager
)
{

}


// ---------------------------------------
// Speed power up function definitions
// ---------------------------------------
SpeedPowerUp::SpeedPowerUp(
	raylib::Texture2DUnmanaged texture,
	raylib::Vector2 pos,
	float speedMultiplier
) : PowerUp(texture, pos), m_speedMultiplier(speedMultiplier)
{

}

SpeedPowerUp::~SpeedPowerUp()
{

}

void SpeedPowerUp::onCollision(
	std::unique_ptr<Ball>& ball,
	SoundManager* soundManager)
{
	soundManager->playSound("speed_up");
	ball->setSpeed(ball->getSpeed() * m_speedMultiplier);
	raylib::Vector2 newVelocity {
		ball->getVelocity().GetX() * m_speedMultiplier,
		ball->getVelocity().GetY() * m_speedMultiplier
	};
	ball->setVelocity(newVelocity);
}


// ---------------------------------------
// Freeze power up function definitions
// ---------------------------------------
FreezePowerUp::FreezePowerUp(
	raylib::Texture2DUnmanaged texture,
	raylib::Vector2 pos,
	float freezeDuration
) : PowerUp(texture, pos), m_freezeDuration(freezeDuration)
{

}

FreezePowerUp::~FreezePowerUp()
{

}

void FreezePowerUp::onCollision(
	std::unique_ptr<Ball>& ball,
	SoundManager* soundManager)
{
	soundManager->playSound("freeze");
	ball->freeze(m_freezeDuration);
}


// ---------------------------------------
// Power up spawner function definitions
// ---------------------------------------
PowerUpSpawner::PowerUpSpawner(
	TextureManager* textureManager,
	std::mt19937& mt
) 
	: m_textureManager(textureManager), m_mt(mt)
{
	m_spawnRate = 5.0f;
	m_maxPowerups = 3;
}

PowerUpSpawner::~PowerUpSpawner()
{

}

void PowerUpSpawner::update(float deltaTime, std::vector<std::unique_ptr<PowerUp>>& powerUps)
{
	m_spawnTimer += deltaTime;
	if (m_spawnTimer >= m_spawnRate)
	{
		if (m_powerupCount < m_maxPowerups)
		{
			powerUps.push_back(std::move(spawnPowerUp()));
			++m_powerupCount;
		}
		m_spawnTimer = 0.0f;
	}
}

std::unique_ptr<PowerUp> PowerUpSpawner::spawnPowerUp()
{
	// Select power up to spawn
	std::uniform_int_distribution<int> powerUpDist(0, 100);
	int powerUpToSpawn {powerUpDist(m_mt)};

	// Get random spawn position
	std::uniform_int_distribution<int> spawnPosXDist(GetScreenWidth() * 0.2f, GetScreenWidth() * 0.8f);
	int spawnXPos {spawnPosXDist(m_mt)};
	std::uniform_int_distribution<int> spawnPosYDist(GetScreenHeight() * 0.2f, GetScreenHeight() * 0.8f);
	int spawnYPos {spawnPosYDist(m_mt)};
	raylib::Vector2 spawnPos {
		static_cast<float>(spawnXPos),
		static_cast<float>(spawnYPos)
	};

	std::unique_ptr<PowerUp> powerup;
	if (powerUpToSpawn < 49)
	{
		// Get speed multiplier
		std::uniform_real_distribution<float> multiplierDist(1.25f, 2.0f);
		float speedMultiplier {multiplierDist(m_mt)};
		powerup = std::make_unique<SpeedPowerUp>(m_textureManager->getTexture("speed_up"), spawnPos, speedMultiplier);
	}
	else if (powerUpToSpawn >= 50)
	{
		// Get random freeze duration
		std::uniform_real_distribution<float> freezeDurationDist(1.5f, 3.0f);
		float freezeDuration {freezeDurationDist(m_mt)};
		powerup = std::make_unique<FreezePowerUp>(m_textureManager->getTexture("freeze"), spawnPos, freezeDuration);
	}

	return powerup;
}