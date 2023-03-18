#include "field.hpp"

// Base Field class definitions
Field::Field(
	std::unique_ptr<Player>& player,
	std::unique_ptr<Computer>& computer,
	std::unique_ptr<Ball>& ball,
    std::unordered_map<std::string, Sound>& sounds)
	: m_player(player), m_computer(computer), m_ball(ball), m_sounds(sounds)
{
    m_player->invertControls(false);
}

Field::~Field() {}

void Field::update(float deltaTime)
{
    //----------------------------------------------------------------
    // Handle collisions 
    //----------------------------------------------------------------

    // Ball collision with top and bottom walls
    if ((m_ball->getPos().GetY() + m_ball->getRadius() >= GetScreenHeight()) ||
        (m_ball->getPos().GetY() - m_ball->getRadius() <= 0))
    {
        PlaySound(m_sounds["wall_hit"]);
        m_ball->onCollisionWalls();
    }

    // Ball collision with left and right walls
    if (m_ball->getPos().GetX() + m_ball->getRadius() >= GetScreenWidth())
    {
        PlaySound(m_sounds["score"]);
        m_changeFields = true;
        m_player->addScore();
        m_ball->reset();
    }
    else if (m_ball->getPos().GetX() - m_ball->getRadius() <= 0)
    {
        PlaySound(m_sounds["score"]);
        m_changeFields = true;
        m_computer->addScore();
        m_ball->reset();
    }

    // Ball collision with paddles
    if (CheckCollisionCircleRec(m_ball->getPos(), m_ball->getRadius(), m_player->getCollisionRec()))
    {
        if (m_ball->getVelocity().GetX() < 0)
        {
            PlaySound(m_sounds["paddle_hit"]);
            m_ball->onCollisionPaddles(m_player->getPos(), m_player->getHeight());
        }
    };

    if (CheckCollisionCircleRec(m_ball->getPos(), m_ball->getRadius(), m_computer->getCollisionRec()))
    {
        if (m_ball->getVelocity().GetX() > 0)
        {
            PlaySound(m_sounds["paddle_hit"]);
            m_ball->onCollisionPaddles(m_computer->getPos(), m_computer->getHeight());
        }
    }

    m_player->update(deltaTime);
    m_computer->update(m_ball, deltaTime);
    m_ball->update(deltaTime);
}

void Field::draw()
{
    // Draw background
    raylib::Rectangle fieldBackground {
        0.0f,
        0.0f,
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
    };
    fieldBackground.Draw(raylib::Color {20, 160, 133, 255});

    raylib::Rectangle fieldBorder {fieldBackground};
    fieldBorder.DrawLines(WHITE, 3.0f);

    // Draw middle line
    raylib::Color middleLine {WHITE};
    middleLine.DrawLine(
        raylib::Vector2 {static_cast<float>(GetScreenWidth() / 2), 0.0f},
        raylib::Vector2 {static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight())},
        3.0f
    );
    
    m_ball->draw();
	m_player->draw();
	m_computer->draw();
	
}


// Inverted Field class definitions
InvertedField::InvertedField(
    std::unique_ptr<Player>& player,
    std::unique_ptr<Computer>& computer,
    std::unique_ptr<Ball>& ball,
    std::unordered_map<std::string, Sound>& sounds
) : Field(player, computer, ball, sounds)
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
    std::unordered_map<std::string, raylib::Texture2DUnmanaged>& textures,
    std::unordered_map<std::string, Sound>& sounds,
    std::mt19937& mt
) : Field(player, computer, ball, sounds)
{
    m_powerUpSpawner = std::make_unique<PowerUpSpawner>(textures, mt);
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
            (*itPowerup)->onCollision(m_ball, m_sounds);
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
    Field::draw();

    for (auto& powerup : m_powerups)
    {
        powerup->draw();
    } 
}


// Obstacle Field class definiions
ObstacleField::ObstacleField(
    std::unique_ptr<Player>& player,
    std::unique_ptr<Computer>& computer,
    std::unique_ptr<Ball>& ball,
    std::unordered_map<std::string, Sound>& sounds,
    std::mt19937& mt
) : Field(player, computer, ball, sounds), m_mt(mt)
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
            PlaySound(m_sounds["wall_hit"]);
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

    // Update obstacles
    for (auto& obstacle : m_obstacles)
    {
        obstacle.update(deltaTime);
    }

    Field::update(deltaTime);
}

void ObstacleField::draw()
{
    Field::draw();

    for (auto& obstacle : m_obstacles)
    {
        obstacle.draw();
    }
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
            xSpawnPos >= GetScreenWidth() / 2 - 150 - m_ball->getRadius() && 
            xSpawnPos <= GetScreenWidth() / 2 + 150 + m_ball->getRadius()
          );

        raylib::Vector2 obstacleSpawnPos {
            xSpawnPos,
            yPosDist(m_mt)
        };

        // Ensure obstacle does not go out of the bottom of the screen
        if (obstacleSpawnPos.GetY() + obstacleHeight > GetScreenHeight() - 3)
            obstacleHeight = GetScreenHeight() - 3 - obstacleSpawnPos.GetY();

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