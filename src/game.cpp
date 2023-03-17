#include "game.hpp"
#include "screen.hpp"
#include <iostream>
#include <memory>

Game::Game() {};

Game::~Game() {};

void Game::initialize()
{
    // Initialize window
    m_title = std::string ("RE:Pong");
	m_window.raylib::Window::Init(m_windowWidth, m_windowHeight, m_title);
    SetExitKey(0);
	
    // Load font
    m_font = raylib::Font("./src/resources/fonts/Roboto-Regular.ttf", 128);
    SetTextureFilter(m_font.texture, TEXTURE_FILTER_BILINEAR);

    // Setup and init first screen
    m_currentScreen = LOGO;
    m_screen = std::make_unique<LogoScreen>();

    m_window.SetTargetFPS(m_targetFPS);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
}

void Game::runLoop()
{
	while (!m_exitGame)
	{
		updateGame(m_window.GetFrameTime());
		renderGame();
	}
}

void Game::shutdown()
{
	m_window.Close();
}

void Game::updateGame(float deltaTime)
{
    if (WindowShouldClose())
    {
        m_exitGame = true;
        return;
    }

	// Update game variables
    if (m_onTransition)
    {
        updateTransition(); // Update transition (fade-in, fade-out)
        return;
    }

    m_screen->updateScreen();
    int nextScreen {m_screen->getNextScreen()};
    switch (m_currentScreen)
    {
        case LOGO:
        {
            if (nextScreen) transitionToScreen(TITLE);

        } break;
        case TITLE:
        {
            if (nextScreen == 1) transitionToScreen(OPTIONS);
            else if (nextScreen == 3) transitionToScreen(GAMEPLAY);
            else if (nextScreen == 6) changeToScreen(EXIT);
        } break;
        case OPTIONS:
        {
            if (nextScreen) transitionToScreen(TITLE);

        } break;
        case GAMEPLAY:
        {
            if (nextScreen) transitionToScreen(ENDING);
            //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

        } break;
        case ENDING:
        {
            if (nextScreen == 3) transitionToScreen(GAMEPLAY);
            else if (nextScreen == 6) changeToScreen(EXIT);
        } break;
        case EXIT:
        {
            m_exitGame = true;
        } break;
        default: break;
    }
    
    //----------------------------------------------------------------------------------
}

void Game::renderGame()
{
	BeginDrawing();
		m_window.ClearBackground(RAYWHITE);
        m_screen->drawScreen();

        // Draw full screen rectangle in front of everything
        if (m_onTransition) drawTransition();
	EndDrawing();
}

// Change to next screen, no transition
void Game::changeToScreen(GameScreen screen)
{
    // Init next screen
    switch (screen)
    {
        case LOGO: 
        {
            m_screen = std::make_unique<LogoScreen>();
        } break;
        case TITLE:
        {
            m_screen = std::make_unique<TitleScreen>(m_font);
        } break;
        case GAMEPLAY: 
        {
            m_screen = std::make_unique<GameplayScreen>(m_winner, m_font);
        } break;
        case ENDING: 
        {
            m_screen = std::make_unique<EndingScreen>(m_winner, m_font);
        } break;
        case EXIT:
        {
            m_exitGame = true;
        } break;
        default: break;
    }
    m_currentScreen = screen;
}

// Request transition to next screen
void Game::transitionToScreen(GameScreen screen)
{
    m_onTransition = true;
    m_transFadeOut = false;
    m_transFromScreen = m_currentScreen;
    m_transToScreen = screen;
    m_transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
void Game::updateTransition(void)
{
    if (!m_transFadeOut)
    {
        m_transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (m_transAlpha > 1.01f)
        {
            m_transAlpha = 1.0f;

            // Load next screen
            switch (m_transToScreen)
            {
                case LOGO:
                {
                    m_screen = std::make_unique<LogoScreen>();
                } break;
                case TITLE:
                {
                    m_screen = std::make_unique<TitleScreen>(m_font);
                } break;
                case GAMEPLAY:
                {
                    m_screen = std::make_unique<GameplayScreen>(m_winner, m_font);
                } break;
                case ENDING:
                {
                    m_screen = std::make_unique<EndingScreen>(m_winner, m_font);
                } break;
                default: break;
            }

            m_currentScreen = m_transToScreen;

            // Activate fade out effect to next loaded screen
            m_transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        m_transAlpha -= 0.02f;

        if (m_transAlpha < -0.01f)
        {
            m_transAlpha = 0.0f;
            m_transFadeOut = false;
            m_onTransition = false;
            m_transFromScreen = -1;
            m_transToScreen = UNKNOWN;
        }
    }
}

// Draw transition effect (full-screen rectangle)
void Game::drawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, m_transAlpha));
}