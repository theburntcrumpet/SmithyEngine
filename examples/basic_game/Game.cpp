#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include "engine/Vector2.hpp"
#include "engine/Input.hpp"
#include "scenes/GameScene.hpp"
#include "scenes/WinScene.hpp"

Game::Game() = default;

Game::~Game() {
    if (m_renderTarget) {
        SDL_DestroyTexture(m_renderTarget);
    }
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}

bool Game::init(const std::string& title, int renderWidth, int renderHeight, int windowScale) {
    m_renderWidth = renderWidth;
    m_renderHeight = renderHeight;
    m_windowWidth = renderWidth * windowScale;
    m_windowHeight = renderHeight * windowScale;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize: " << SDL_GetError() << '\n';
        return false;
    }

    // Hint for nearest-neighbor scaling (pixelated, not blurry)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    m_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_windowWidth,
        m_windowHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!m_window) {
        std::cerr << "Window could not be created: " << SDL_GetError() << '\n';
        return false;
    }

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!m_renderer) {
        std::cerr << "Renderer could not be created: " << SDL_GetError() << '\n';
        return false;
    }

    // Create render target texture at internal resolution
    m_renderTarget = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_renderWidth,
        m_renderHeight
    );

    if (!m_renderTarget) {
        std::cerr << "Render target could not be created: " << SDL_GetError() << '\n';
        return false;
    }

    // Ensure the render target also uses nearest-neighbor scaling
    SDL_SetTextureScaleMode(m_renderTarget, SDL_ScaleModeNearest);

    updateOutputRect();

    // Initialize camera with viewport size
    m_camera.SetSize(static_cast<float>(m_renderWidth), static_cast<float>(m_renderHeight));

    // Initialize game renderer
    m_gameRenderer.SetSDLRenderer(m_renderer);
    m_gameRenderer.SetCamera(&m_camera);

    // Register scenes
    auto* gameScene = m_sceneManager.RegisterScene<Scenes::GameScene>("game", &m_camera, &m_input);
    gameScene->SetWorldSize(m_worldWidth, m_worldHeight);
    m_sceneManager.RegisterScene<Scenes::WinScene>("win", &m_input);

    // Start with game scene
    m_sceneManager.SwitchTo("game");
    m_sceneManager.Init(m_gameRenderer);

    m_running = true;
    return true;
}

void Game::toggleFullscreen() {
    m_fullscreen = !m_fullscreen;

    if (m_fullscreen) {
        // Use desktop fullscreen (keeps desktop resolution, faster switching)
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(m_window, 0);
    }

    // Get new window size and recalculate output rect
    SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
    updateOutputRect();
}

void Game::updateOutputRect() {
    // Calculate the largest size that fits while maintaining aspect ratio
    float scaleX = static_cast<float>(m_windowWidth) / static_cast<float>(m_renderWidth);
    float scaleY = static_cast<float>(m_windowHeight) / static_cast<float>(m_renderHeight);
    float scale = std::min(scaleX, scaleY);

    // For pixel-perfect scaling, use integer scaling if possible
    // This ensures each game pixel maps to exactly NxN screen pixels
    int intScale = static_cast<int>(scale);
    if (intScale >= 1) {
        scale = static_cast<float>(intScale);
    }

    int outputWidth = static_cast<int>(m_renderWidth * scale);
    int outputHeight = static_cast<int>(m_renderHeight * scale);

    // Center the output in the window (letterboxing/pillarboxing)
    m_outputRect.x = (m_windowWidth - outputWidth) / 2;
    m_outputRect.y = (m_windowHeight - outputHeight) / 2;
    m_outputRect.w = outputWidth;
    m_outputRect.h = outputHeight;
}

void Game::run() {
    Uint64 previousTime = SDL_GetPerformanceCounter();
    const Uint64 frequency = SDL_GetPerformanceFrequency();

    while (m_running) {
        Uint64 currentTime = SDL_GetPerformanceCounter();
        float deltaTime = static_cast<float>(currentTime - previousTime) / static_cast<float>(frequency);
        previousTime = currentTime;

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    static bool keyUp = false;
    static bool keyDown = false;
    static bool keyLeft = false;
    static bool keyRight = false;

    // Reset per-frame action states
    m_input.GetPrimary().SetJustPressed(false);
    m_input.GetPrimary().SetJustReleased(false);
    m_input.GetSecondary().SetJustPressed(false);
    m_input.GetSecondary().SetJustReleased(false);
    m_input.GetStart().SetJustPressed(false);
    m_input.GetStart().SetJustReleased(false);
    m_input.GetSelect().SetJustPressed(false);
    m_input.GetSelect().SetJustReleased(false);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.repeat) break; // Ignore key repeats for JustPressed
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        keyUp = true;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        keyDown = true;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        keyLeft = true;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        keyRight = true;
                        break;
                    case SDL_SCANCODE_Z:
                    case SDL_SCANCODE_SPACE:
                        m_input.GetPrimary().SetIsPressed(true);
                        m_input.GetPrimary().SetJustPressed(true);
                        break;
                    case SDL_SCANCODE_X:
                        m_input.GetSecondary().SetIsPressed(true);
                        m_input.GetSecondary().SetJustPressed(true);
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (SDL_GetModState() & KMOD_ALT) {
                            toggleFullscreen();
                        } else {
                            m_input.GetStart().SetIsPressed(true);
                            m_input.GetStart().SetJustPressed(true);
                        }
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        m_input.GetSelect().SetIsPressed(true);
                        m_input.GetSelect().SetJustPressed(true);
                        break;
                    case SDL_SCANCODE_ESCAPE:
                        m_running = false;
                        break;
                    case SDL_SCANCODE_F11:
                        toggleFullscreen();
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        keyUp = false;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        keyDown = false;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        keyLeft = false;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        keyRight = false;
                        break;
                    case SDL_SCANCODE_Z:
                    case SDL_SCANCODE_SPACE:
                        m_input.GetPrimary().SetIsPressed(false);
                        m_input.GetPrimary().SetJustReleased(true);
                        break;
                    case SDL_SCANCODE_X:
                        m_input.GetSecondary().SetIsPressed(false);
                        m_input.GetSecondary().SetJustReleased(true);
                        break;
                    case SDL_SCANCODE_RETURN:
                        m_input.GetStart().SetIsPressed(false);
                        m_input.GetStart().SetJustReleased(true);
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        m_input.GetSelect().SetIsPressed(false);
                        m_input.GetSelect().SetJustReleased(true);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    m_windowWidth = event.window.data1;
                    m_windowHeight = event.window.data2;
                    updateOutputRect();
                }
                break;
        }
    }

    // Compute direction from key states
    float dirX = (keyRight ? 1.0f : 0.0f) - (keyLeft ? 1.0f : 0.0f);
    float dirY = (keyDown ? 1.0f : 0.0f) - (keyUp ? 1.0f : 0.0f);
    m_input.SetDirection(Engine::Vector2<float>(dirX, dirY));
}

void Game::update(float deltaTime) {
    m_sceneManager.Update(deltaTime);
}

void Game::render() {
    // === Render to internal resolution texture ===
    SDL_SetRenderTarget(m_renderer, m_renderTarget);

    // Clear with dark blue
    m_gameRenderer.Clear(Engine::Color(20, 20, 40));

    // Draw current scene
    m_sceneManager.Draw();

    // === Render internal texture to screen ===
    SDL_SetRenderTarget(m_renderer, nullptr);

    // Clear the actual screen with black (for letterbox bars)
    m_gameRenderer.Clear(Engine::Color::Black());

    // Draw the game scaled to fit, centered with letterboxing
    SDL_RenderCopy(m_renderer, m_renderTarget, nullptr, &m_outputRect);

    SDL_RenderPresent(m_renderer);
}
