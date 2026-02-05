#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "engine/Input.hpp"
#include "engine/Vector2.hpp"
#include "engine/Camera.hpp"
#include "engine/Renderer.hpp"
#include "engine/SceneManager.hpp"

class Game {
public:
    Game();
    ~Game();

    bool init(const std::string& title, int renderWidth, int renderHeight, int windowScale = 2);
    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    void toggleFullscreen();
    void updateOutputRect();

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_renderTarget = nullptr;
    bool m_running = false;
    bool m_fullscreen = false;

    // Internal render resolution (game renders at this size)
    int m_renderWidth = 0;
    int m_renderHeight = 0;

    // Actual window size (may differ from render size)
    int m_windowWidth = 0;
    int m_windowHeight = 0;

    // Where the scaled game is drawn on screen (for letterboxing)
    SDL_Rect m_outputRect = {0, 0, 0, 0};

    // World size (larger than screen to demonstrate scrolling)
    int m_worldWidth = 640;
    int m_worldHeight = 360;

    // Camera
    Engine::Camera m_camera;

    // Game renderer (wraps SDL_Renderer with camera support)
    Engine::Renderer m_gameRenderer;

    // Input state
    Engine::Input m_input;

    // Scene manager
    Engine::SceneManager m_sceneManager;
};
