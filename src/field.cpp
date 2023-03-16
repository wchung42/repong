#include "field.hpp"
#include <iostream>

// Base Field class definitions
Field::Field(
	std::unique_ptr<Player>& player,
	std::unique_ptr<Computer>& computer,
	std::unique_ptr<Ball>& ball)
	: m_player(player), m_computer(computer), m_ball(ball)
{
    m_player->invertControls(false);
}

Field::~Field() {}

void Field::update(float deltaTime)
{
    ///////////////////////
    // Handle collisions //
    ///////////////////////

    // Ball collision with top and bottom walls
    if ((m_ball->getPos().GetY() + m_ball->getRadius() >= GetScreenHeight()) ||
        (m_ball->getPos().GetY() - m_ball->getRadius() <= 0))
    {
        m_ball->onCollisionWalls();
    }

    // Ball collision with left and right walls
    if (m_ball->getPos().GetX() + m_ball->getRadius() >= GetScreenWidth())
    {
        m_changeFields = true;
        m_ball->reset();
    }
    else if (m_ball->getPos().GetX() - m_ball->getRadius() <= 0)
    {
        m_changeFields = true;
        m_ball->reset();
    }

    // Ball collision with paddles
    if (CheckCollisionCircleRec(m_ball->getPos(), m_ball->getRadius(), m_player->getCollisionRec()))
    {
        if (m_ball->getVelocity().GetX() < 0)
        {
            m_ball->onCollisionPaddles(m_player->getPos(), m_player->getHeight());
        }
    };

    if (CheckCollisionCircleRec(m_ball->getPos(), m_ball->getRadius(), m_computer->getCollisionRec()))
    {
        if (m_ball->getVelocity().GetX() > 0)
            m_ball->onCollisionPaddles(m_computer->getPos(), m_computer->getHeight());
    }

    m_player->update(deltaTime);
    m_computer->update(m_ball, deltaTime);
    m_ball->update(deltaTime);
}

void Field::draw()
{
	m_player->draw();
	m_computer->draw();
	m_ball->draw();
}


// Inverted Field class definitions
InvertedField::InvertedField(
    std::unique_ptr<Player>& player,
    std::unique_ptr<Computer>& computer,
    std::unique_ptr<Ball>& ball
) : Field(player, computer, ball)
{
    m_player->invertControls(true);
}

InvertedField::~InvertedField()
{

}

void InvertedField::update(float deltaTime)
{
    Field::update(deltaTime);
}

void InvertedField::draw()
{
    Field::draw();
}


// Powerup Field class definitions
PowerUpField::PowerUpField(
    std::unique_ptr<Player>& player,
    std::unique_ptr<Computer>& computer,
    std::unique_ptr<Ball>& ball,
    std::unordered_map<std::string, raylib::Texture2DUnmanaged>& textures
) : Field(player, computer, ball)
{
    m_powerUpSpawner = std::make_unique<PowerUpSpawner>(textures);
}

PowerUpField::~PowerUpField()
{

}

void PowerUpField::update(float deltaTime)
{
    m_powerUpSpawner->update(deltaTime, m_powerups);

    // Handle collisions
    for (auto itPowerup {m_powerups.begin()}; itPowerup != m_powerups.end();)
    {
        // If ball collides with powerup, activate powerup and remove from vector
        if ((*itPowerup)->getCollisionRec().CheckCollision(m_ball->getPos(), m_ball->getRadius()))
        {
            (*itPowerup)->onCollision(m_ball);
            m_powerUpSpawner->decrementPowerupCount();
            itPowerup = m_powerups.erase(itPowerup);
        }
        else
            ++itPowerup;
    }

    Field::update(deltaTime);
}

void PowerUpField::draw()
{
    for (auto& powerup : m_powerups)
    {
        powerup->draw();
    }

    Field::draw();
}


// Obstacle Field class definiions
ObstacleField::ObstacleField(
    std::unique_ptr<Player>& player,
    std::unique_ptr<Computer>& computer,
    std::unique_ptr<Ball>& ball
) : Field(player, computer, ball), m_mt((std::random_device())())
{
    // Spawn obstacles
    this->spawnObstacles();
}

ObstacleField::~ObstacleField()
{

}

void ObstacleField::update(float deltaTime)
{
    // Handle collisions between ball and obstacle
    for (auto& obstacle : m_obstacles)
    {
        if (obstacle.getCollisionRec().CheckCollision(m_ball->getPos(), m_ball->getRadius()))
        {
            m_ball->onCollisionObstacles(obstacle);
            obstacle.takeDamage();
            obstacle.getHealth();
        }
    }

    // Delete broken obstacles
    for (auto itObstacle {m_obstacles.begin()}; itObstacle != m_obstacles.end();)
    {
        if ((*itObstacle).getHealth() <= 0)
            itObstacle = m_obstacles.erase(itObstacle);
        else
            ++itObstacle;
    }

    Field::update(deltaTime);
}

void ObstacleField::draw()
{
    for (auto& obstacle : m_obstacles)
    {
        obstacle.draw();
    }

    Field::draw();
}

void ObstacleField::spawnObstacles()
{
    std::uniform_real_distribution<float> xPosDist(GetScreenWidth() * 0.25f, GetScreenWidth() * 0.75f);
    std::uniform_real_distribution<float> yPosDist(GetScreenHeight() * 0.0f, GetScreenHeight() * 0.75f);

    int minObstacleCount {2};
    int maxObstacleCount {5};
    std::uniform_int_distribution<int> obstacleCountDist(minObstacleCount, maxObstacleCount);
    int obstacleCount {obstacleCountDist(m_mt)};

    for (int i {0}; i < obstacleCount; ++i)
    {
        // Get random width
        std::uniform_real_distribution<float> widthDist(32.0f, 128.0f);
        float obstacleWidth {widthDist(m_mt)};
        std::uniform_real_distribution<float> heightDist(obstacleWidth, obstacleWidth * 3.0f);
        float obstacleHeight {heightDist(m_mt)};

        // Get random position
        float xSpawnPos {};
        do {
            xSpawnPos = xPosDist(m_mt);
        } while (
            xSpawnPos >= GetScreenWidth() / 2 - 64 - m_ball->getRadius() && 
            xSpawnPos <= GetScreenWidth() / 2 + 64 + m_ball->getRadius()
          );

        raylib::Vector2 obstacleSpawnPos {
            xSpawnPos,
            yPosDist(m_mt)
        };

        // Calculate health based on width
        int health {static_cast<int>(obstacleWidth) / 16 + 1};

        Obstacle obstacle {
            obstacleSpawnPos,
            raylib::Vector2 {
                obstacleWidth,
                obstacleHeight
            },
            health
        };

        m_obstacles.push_back(obstacle);
    }
}