#include "screen.hpp"

Screen::Screen() {}

Screen::~Screen() 
{
	// Unload resources
}

void Screen::updateScreen()
{
	// Update screen
}

void Screen::drawScreen()
{
	// Draw screen
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

TitleScreen::TitleScreen()
{

}

TitleScreen::~TitleScreen()
{

}

// Title Screen Update logic
void TitleScreen::updateScreen()
{
    // TODO: Update TITLE screen variables here!

    // Press enter or tap to change to GAMEPLAY screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        //finishScreen = 1;   // OPTIONS
        m_nextScreen = 2;   // GAMEPLAY
    }
}

// Title Screen Draw logic
void TitleScreen::drawScreen()
{
    // TODO: Draw TITLE screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
    raylib::Vector2 titlePosition {20, 10};
    raylib::DrawText("TITLE SCREEN", titlePosition.GetX(), titlePosition.GetY(), 20, DARKGREEN);
    raylib::Vector2 instructionPosition = {120, 220};
    raylib::DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", instructionPosition.GetX(), instructionPosition.GetY(), 30, DARKGREEN);
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
    // TODO: Draw OPTIONS screen here!
}


//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

GameplayScreen::GameplayScreen()
    : m_mt((std::random_device())())
{
    m_player = std::make_unique<Player>();
    m_computer = std::make_unique<Computer>();
    m_ball = std::make_unique<Ball>();
    m_field = std::make_unique<Field>(m_player, m_computer, m_ball);

    // Load game textures
    utils::loadTextures({
        "./src/resources/textures/freeze.png",
        "./src/resources/textures/speed_up.png"
        }, m_textures);

}

GameplayScreen::~GameplayScreen()
{
    // Unload resources
}


// Gameplay Screen Update logic
void GameplayScreen::updateScreen()
{
    float deltaTime {GetFrameTime()};

    // change fields when someone scores
    if (m_field->shouldChangeFields())
    {
        std::uniform_int_distribution<int> distField(0, 100);
        int nextField {distField(m_mt)};
        if (nextField < 24)
        {
            m_field = std::make_unique<Field>(m_player, m_computer, m_ball);
        }
        else if (nextField >= 25 && nextField <= 49)
        {
            m_field = std::make_unique<InvertedField>(m_player, m_computer, m_ball);
        }
        else if (nextField >= 50 && nextField <= 74)
        {
            m_field = std::make_unique<PowerUpField>(m_player, m_computer, m_ball, m_textures);
        }
        else if (nextField >= 75)
        {
            m_field = std::make_unique<ObstacleField>(m_player, m_computer, m_ball);
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
    m_field->draw();
}


//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

EndingScreen::EndingScreen()
{

}

EndingScreen::~EndingScreen()
{
    // Unload resources
}

// Ending Screen Update logic
void EndingScreen::updateScreen()
{
    // TODO: Update ENDING screen variables here!

    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        m_nextScreen = 1;
        //PlaySound(fxCoin);
    }
}

// Ending Screen Draw logic
void EndingScreen::drawScreen()
{
    // TODO: Draw ENDING screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
    raylib::Vector2 titlePosition {20, 10};
    raylib::DrawText("ENDING SCREEN", titlePosition.GetX(), titlePosition.GetY(), 20, DARKBLUE);
    raylib::Vector2 instructionPosition {120, 220};
    raylib::DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", instructionPosition.GetX(), instructionPosition.GetY(), 30, DARKBLUE);
}