#include "field.hpp"
#include "rlgl.h"
#include "raymath.h"
#include <iostream>

// constants from OpenGL
#define GL_SRC_ALPHA 0x0302
#define GL_MIN 0x8007

// Base Field class definitions
Field::Field(
	std::unique_ptr<Player>& player,
	std::unique_ptr<Computer>& computer,
	std::unique_ptr<Ball>& ball,
    SoundManager* soundManager
)
	: m_player(player), m_computer(computer), m_ball(ball), m_soundManager(soundManager)
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
        m_soundManager->playSound("wall_hit");
        m_ball->onCollisionWalls();
    }

    // Ball collision with left and right walls
    if (m_ball->getPos().GetX() + m_ball->getRadius() >= GetScreenWidth())
    {
        m_soundManager->playSound("score");
        m_changeFields = true;
        m_player->addScore();
        m_ball->reset();
    }
    else if (m_ball->getPos().GetX() - m_ball->getRadius() <= 0)
    {
        m_soundManager->playSound("score");
        m_changeFields = true;
        m_computer->addScore();
        m_ball->reset();
    }

    // Ball collision with paddles
    if (CheckCollisionCircleRec(m_ball->getPos(), m_ball->getRadius(), m_player->getCollisionRec()))
    {
        if (m_ball->getVelocity().GetX() < 0)
        {
            m_soundManager->playSound("paddle_hit");
            m_ball->onCollisionPaddles(m_player->getPos(), m_player->getHeight());
        }
    };

    if (CheckCollisionCircleRec(m_ball->getPos(), m_ball->getRadius(), m_computer->getCollisionRec()))
    {
        if (m_ball->getVelocity().GetX() > 0)
        {
            m_soundManager->playSound("paddle_hit");
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
    SoundManager* soundManager
) : Field(player, computer, ball, soundManager)
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
    TextureManager* textureManager,
    SoundManager* soundManager,
    std::mt19937& mt
) : Field(player, computer, ball, soundManager)
{
    m_powerUpSpawner = std::make_unique<PowerUpSpawner>(textureManager, mt);
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
            (*itPowerup)->onCollision(m_ball, m_soundManager);
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
    SoundManager* soundManager,
    std::mt19937& mt
)   
    : Field(player, computer, ball, soundManager), m_mt(mt)
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
            m_soundManager->playSound("wall_hit");
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

// -------------------------
// Lights Out Field
// -------------------------
LightsOutField::LightsOutField(
    std::unique_ptr<Player>& player,
    std::unique_ptr<Computer>& computer,
    std::unique_ptr<Ball>& ball,
    SoundManager* soundManager
) : Field(player, computer, ball, soundManager)
{
    // Make a render texture that we can put a hole into at the mouse pos
    m_holeTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}

LightsOutField::~LightsOutField()
{
    m_holeTexture.Unload();
}

void LightsOutField::update(float deltaTime)
{
    // --------------------------
    // Update hole around ball
    // --------------------------

    // Update the hole to be where the mouse is
    BeginTextureMode(m_holeTexture);
        ClearBackground(BLACK);

        // Force the blend mode to only set the alpha of the destination
        rlSetBlendFactors(GL_SRC_ALPHA, GL_SRC_ALPHA, GL_MIN);
        rlSetBlendMode(BLEND_CUSTOM);

        // Draw a blank 'hole' in our texture around the ball
        float visionRadius {100.0f};
        DrawCircle(m_ball->getPos().GetX(), m_ball->getPos().GetY(), visionRadius, BLANK);

        // Go back to normal
        rlSetBlendMode(BLEND_ALPHA);
    EndTextureMode();

    // Update field
    Field::update(deltaTime);
}

void LightsOutField::draw()
{
    // Draw field
    Field::draw();

    // Draw hole texture
    Rectangle src {0, 0, GetScreenWidth(), -GetScreenHeight()};
    Rectangle dest {0, 0, GetScreenWidth(), GetScreenHeight()};
    DrawTexturePro(m_holeTexture.texture, src, dest, Vector2Zero(), 0, WHITE);
}
