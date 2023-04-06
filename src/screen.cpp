#include "screen.hpp"
#include <time.h>

Screen::Screen() {}

Screen::~Screen() 
{
	// Unload resources
}

void Screen::updateScreen()
{
	// Update screen
    if (WindowShouldClose())
    {
        m_nextScreen = 6;
        return;
    }
}

void Screen::drawScreen()
{
    // Draw background
    raylib::Rectangle fieldBackground {
        0.0f,
        0.0f,
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
    };
    fieldBackground.Draw(raylib::Color {20, 160, 133, 255});
}


//----------------------------------------------------------------------------------
// Logo Screen Functions Definition
//----------------------------------------------------------------------------------

LogoScreen::LogoScreen()
{
    m_nextScreen = 0;
    m_framesCounter = 0;
    m_lettersCount = 0;

    m_logoPositionX = GetScreenWidth() / 2 - 128;
    m_logoPositionY = GetScreenHeight() / 2 - 128;

    m_topSideRecWidth = 16;
    m_leftSideRecHeight = 16;
    m_bottomSideRecWidth = 16;
    m_rightSideRecHeight = 16;

    m_state = 0;
    m_alpha = 1.0f;
}

LogoScreen::~LogoScreen()
{
    // Unload resources
}

// Logo Screen Update logic
void LogoScreen::updateScreen()
{
    Screen::updateScreen();

    if (m_state == 0)                 // State 0: Top-left square corner blink logic
    {
        m_framesCounter++;

        if (m_framesCounter == 80)
        {
            m_state = 1;
            m_framesCounter = 0;      // Reset counter... will be used later...
        }
    }
    else if (m_state == 1)            // State 1: Bars animation logic: top and left
    {
        m_topSideRecWidth += 8;
        m_leftSideRecHeight += 8;

        if (m_topSideRecWidth == 256) m_state = 2;
    }
    else if (m_state == 2)            // State 2: Bars animation logic: bottom and right
    {
        m_bottomSideRecWidth += 8;
        m_rightSideRecHeight += 8;

        if (m_bottomSideRecWidth == 256) m_state = 3;
    }
    else if (m_state == 3)            // State 3: "raylib" text-write animation logic
    {
        m_framesCounter++;

        if (m_lettersCount < 10)
        {
            if (m_framesCounter / 12)   // Every 12 frames, one more letter!
            {
                m_lettersCount++;
                m_framesCounter = 0;
            }
        }
        else    // When all letters have appeared, just fade out everything
        {
            if (m_framesCounter > 200)
            {
                m_alpha -= 0.02f;

                if (m_alpha <= 0.0f)
                {
                    m_alpha = 0.0f;
                    m_nextScreen = 1;   // Jump to next screen
                }
            }
        }
    }
}

// Logo Screen Draw logic
void LogoScreen::drawScreen()
{
    if (m_state == 0)         // Draw blinking top-left square corner
    {
        if ((m_framesCounter / 10) % 2)
        {
            DrawRectangle(m_logoPositionX, m_logoPositionY, 16, 16, BLACK);
        }
    }
    else if (m_state == 1)    // Draw bars animation: top and left
    {
        DrawRectangle(m_logoPositionX, m_logoPositionY, m_topSideRecWidth, 16, BLACK);
        DrawRectangle(m_logoPositionX, m_logoPositionY, 16, m_leftSideRecHeight, BLACK);
    }
    else if (m_state == 2)    // Draw bars animation: bottom and right
    {
        DrawRectangle(m_logoPositionX, m_logoPositionY, m_topSideRecWidth, 16, BLACK);
        DrawRectangle(m_logoPositionX, m_logoPositionY, 16, m_leftSideRecHeight, BLACK);

        DrawRectangle(m_logoPositionX + 240, m_logoPositionY, 16, m_rightSideRecHeight, BLACK);
        DrawRectangle(m_logoPositionX, m_logoPositionY + 240, m_bottomSideRecWidth, 16, BLACK);
    }
    else if (m_state == 3)    // Draw "raylib" text-write animation + "powered by"
    {
        DrawRectangle(m_logoPositionX, m_logoPositionY, m_topSideRecWidth, 16, Fade(BLACK, m_alpha));
        DrawRectangle(m_logoPositionX, m_logoPositionY + 16, 16, m_leftSideRecHeight - 32, Fade(BLACK, m_alpha));

        DrawRectangle(m_logoPositionX + 240, m_logoPositionY + 16, 16, m_rightSideRecHeight - 32, Fade(BLACK, m_alpha));
        DrawRectangle(m_logoPositionX, m_logoPositionY + 240, m_bottomSideRecWidth, 16, Fade(BLACK, m_alpha));

        DrawRectangle(GetScreenWidth() / 2 - 112, GetScreenHeight() / 2 - 112, 224, 224, Fade(RAYWHITE, m_alpha));

        DrawText(TextSubtext("raylib", 0, m_lettersCount), GetScreenWidth() / 2 - 44, GetScreenHeight() / 2 + 48, 50, Fade(BLACK, m_alpha));

        if (m_framesCounter > 20) DrawText("powered by", m_logoPositionX, m_logoPositionY - 27, 20, Fade(DARKGRAY, m_alpha));
    }
}


//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

TitleScreen::TitleScreen(raylib::Font& font)
    : m_font(font)
{
    utils::loadTextures({
        "./src/resources/textures/title.png",
        "./src/resources/textures/play_button.png",
        "./src/resources/textures/quit_button.png"
    }, m_textures);

    // Draw title
    m_title = m_textures["title"];
    m_titlePos = raylib::Vector2 {
        static_cast<float>(GetScreenWidth() / 2 - m_textures["title"].GetWidth() / 2),
        static_cast<float>(GetScreenHeight() * 0.20f)
    };

    float buttonScale {0.9};
    float titleButtonSpacing {25.0f};
    raylib::Vector2 playButtonPos {
        static_cast<float>(GetScreenWidth() / 2 - m_textures["play_button"].GetWidth() * buttonScale / 2),
        static_cast<float>(m_titlePos.GetY() + m_textures["title"].GetHeight() + titleButtonSpacing)
    };
    m_playButton = Button {
        playButtonPos,
        m_textures["play_button"],
        buttonScale
    };

    float buttonSpacing {20.0f};
    raylib::Vector2 quitButtonPos {
        static_cast<float>(GetScreenWidth() / 2 - m_textures["play_button"].GetWidth() * buttonScale / 2),
        static_cast<float>(m_playButton.getPos().GetY() + m_playButton.getHeight() - buttonSpacing)
    };
    m_quitButton = Button {
        quitButtonPos,
        m_textures["quit_button"],
        buttonScale
    };
}

TitleScreen::~TitleScreen()
{
    for (auto& [name, texture] : m_textures)
    {
        texture.Unload();
    }
}

// Title Screen Update logic
void TitleScreen::updateScreen()
{
    Screen::updateScreen();     // Check if windows close button is clicked

    if (m_playButton.isClicked())
    {
        //finishScreen = 1;     // OPTIONS
        m_nextScreen = 3;       // GAMEPLAY
        return;
    }

    if (m_quitButton.isClicked())
    {
        m_nextScreen = 6;     // Exit game
        return;
    }

    m_playButton.update();
    m_quitButton.update();
}

// Title Screen Draw logic
void TitleScreen::drawScreen()
{
    Screen::drawScreen();
    m_title.Draw(m_titlePos, 0.0f, 1.0f, WHITE);
    m_playButton.draw();
    m_quitButton.draw();
    
}


//----------------------------------------------------------------------------------
// Options Screen Functions Definition
//----------------------------------------------------------------------------------

OptionsScreen::OptionsScreen()
{

}

OptionsScreen::~OptionsScreen()
{

}

// Options Screen Update logic
void OptionsScreen::updateScreen()
{
    // TODO: Update OPTIONS screen variables here!
}

// Options Screen Draw logic
void OptionsScreen::drawScreen()
{
    Screen::drawScreen();
}


//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

GameplayScreen::GameplayScreen(std::string& winner, raylib::Font& font)
    : m_winner(winner), m_font(font), m_mt(static_cast<unsigned int>(time(nullptr)))
{
    // Load game textures
    utils::loadTextures({
        "./src/resources/textures/freeze.png",
        "./src/resources/textures/speed_up.png"
    }, m_textures);

    // Load sounds
    utils::loadSounds({
        "./src/resources/sfx/freeze.wav",
        "./src/resources/sfx/paddle_hit.wav",
        "./src/resources/sfx/score.wav",
        "./src/resources/sfx/speed.wav",
        "./src/resources/sfx/wall_hit.wav"
    }, m_sounds);

    m_player = std::make_unique<Player>();
    m_computer = std::make_unique<Computer>();
    m_ball = std::make_unique<Ball>(m_mt);
    m_field = std::make_unique<Field>(m_player, m_computer, m_ball, m_sounds);
}

GameplayScreen::~GameplayScreen()
{
    // Unload resources
    for (auto& [name, texture] : m_textures)
    {
        texture.Unload();
    }

    // Unload sounds
    for (auto& [name, sound] : m_sounds)
    {
        UnloadSound(sound);
    }
}

// Gameplay Screen Update logic
void GameplayScreen::updateScreen()
{
    Screen::updateScreen();     // Check if windows close button is clicked

    if (m_player->getScore() >= 10 || m_computer->getScore() >= 10)
    {
        m_winner = m_player->getScore() >= 10 ? "player" : "computer";
        m_nextScreen = ENDING;
        return;
    }

    float deltaTime {GetFrameTime()};

    // change fields when someone scores
    if (m_field->shouldChangeFields())
    {
        std::uniform_int_distribution<int> distField(0, 100);
        int nextField {distField(m_mt)};
        if (nextField < 19)
        {
            m_field = std::make_unique<Field>(m_player, m_computer, m_ball, m_sounds);
        }
        else if (nextField >= 20 && nextField <= 39)
        {
            m_field = std::make_unique<InvertedField>(m_player, m_computer, m_ball, m_sounds);
        }
        else if (nextField >= 40 && nextField <= 59)
        {
            m_field = std::make_unique<PowerUpField>(m_player, m_computer, m_ball, m_textures, m_sounds, m_mt);
        }
        else if (nextField >= 60 && nextField <= 79)
        {
            m_field = std::make_unique<ObstacleField>(m_player, m_computer, m_ball, m_sounds, m_mt);
        }
        else
        {
            m_field = std::make_unique<LightsOutField>(m_player, m_computer, m_ball, m_sounds);
        }
    }
    else
    {
        m_field->update(deltaTime);
    }
}

// Gameplay Screen Draw logic
void GameplayScreen::drawScreen()
{
    Screen::drawScreen();
    m_field->draw();
    
    // Draw scores
    raylib::Text playerScore {std::to_string(m_player->getScore()), 100.0f, WHITE, m_font, 1.0f};
    raylib::Vector2 playerScorePos {
        static_cast<float>(GetScreenWidth() * 0.45 - playerScore.Measure() / 2),
        static_cast<float>(GetScreenHeight() * 0.05f)
    };
    playerScore.Draw(playerScorePos);

    raylib::Text computerScore {std::to_string(m_computer->getScore()), 100.0f, WHITE, m_font, 1.0f};
    raylib::Vector2 computerScorePos {
        static_cast<float>(GetScreenWidth() * 0.55 - computerScore.Measure() / 2),
        static_cast<float>(GetScreenHeight() * 0.05f)
    };
    computerScore.Draw(computerScorePos);
}


//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

EndingScreen::EndingScreen(std::string& winner, raylib::Font& font)
    : m_winner(winner), m_font(font)
{
    // Load resources
    utils::loadTextures({
        "./src/resources/textures/play_again_button.png",
        "./src/resources/textures/quit_button.png"
    }, m_textures);

    std::string winText {};
    if (m_winner == "player")
        winText = "You win!";
    else
        winText = "Computer wins!";

    m_winnerText = raylib::Text {
        std::string {winText},
        100.0f,
        raylib::Color {6, 48, 39, 255},
        m_font,
        1.0f
    };
    raylib::Vector2 winnerTextSize = m_winnerText.MeasureEx();
    m_winnerTextPos = raylib::Vector2 {
        static_cast<float>(GetScreenWidth() / 2 - winnerTextSize.GetX() / 2),
        static_cast<float>(GetScreenHeight() * 0.25)
    };
    
    float textButtonSpacing {50.0f};
    float buttonSpacing {20.0f};
    float buttonScale {0.9f};
    m_playAgainButton = Button {
        raylib::Vector2 {
            static_cast<float>(GetScreenWidth() / 2 - m_textures["play_again_button"].GetWidth() * buttonScale / 2),
            m_winnerTextPos.GetY() + winnerTextSize.GetY() + textButtonSpacing},
        m_textures["play_again_button"],
        buttonScale
    };

    m_quitButton = Button {
        raylib::Vector2 {
            static_cast<float>(GetScreenWidth() / 2 - m_textures["quit_button"].GetWidth() * buttonScale / 2),
            static_cast<float>(m_playAgainButton.getPos().GetY() + m_playAgainButton.getHeight() - buttonSpacing)},
        m_textures["quit_button"],
        buttonScale
    };
}

EndingScreen::~EndingScreen()
{
    // Unload resources
    for (auto& [name, texture] : m_textures)
    {
        texture.Unload();
    }
}

// Ending Screen Update logic
void EndingScreen::updateScreen()
{
    Screen::updateScreen();     // Check if windows close button is clicked

    if (m_playAgainButton.isClicked())
    {
        //finishScreen = 1;     // OPTIONS
        m_nextScreen = 3;       // GAMEPLAY
        return;
    }

    if (m_quitButton.isClicked())
    {
        m_nextScreen = 6;     // Exit game
        return;
    }

    m_playAgainButton.update();
    m_quitButton.update();
}

// Ending Screen Draw logic
void EndingScreen::drawScreen()
{
    Screen::drawScreen();

    m_winnerText.Draw(m_winnerTextPos);
    m_playAgainButton.draw();
    m_quitButton.draw();
   /* DrawLine(
        GetScreenWidth() / 2,
        0.0f,
        GetScreenWidth() / 2,
        GetScreenHeight(),
        RED
    );*/
}
