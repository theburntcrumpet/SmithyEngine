#include "engine/Text.hpp"
#include "engine/Renderer.hpp"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Text Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdlRenderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create engine renderer
    Engine::Renderer renderer;
    renderer.SetSDLRenderer(sdlRenderer);

    // Create text objects
    Engine::Text title;
    Engine::Text subtitle;
    Engine::Text instructions;

    // Initialize with a system font (adjust path as needed for your system)
    // Common paths: /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf (Linux)
    //               C:/Windows/Fonts/arial.ttf (Windows)
    //               /System/Library/Fonts/Helvetica.ttc (macOS)
    if (!title.Init(&renderer, "./assets/font.ttf", 48)) {
        std::cout << "Failed to load title font, trying system font..." << std::endl;
        if (!title.Init(&renderer, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 48)) {
            std::cout << "Failed to load any font!" << std::endl;
        }
    }

    if (!subtitle.Init(&renderer, "./assets/font.ttf", 24)) {
        subtitle.Init(&renderer, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    }

    if (!instructions.Init(&renderer, "./assets/font.ttf", 16)) {
        instructions.Init(&renderer, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
    }

    // Set text content
    title.SetText("Hello, Smithy!");
    title.SetColor(Engine::Color::White());

    subtitle.SetText("SDL2_ttf Text Rendering Example");
    subtitle.SetColor(Engine::Color::Cyan());

    instructions.SetText("Press SPACE to change color, ESC to quit");
    instructions.SetColor(Engine::Color::Yellow());

    // Color cycling
    Engine::Color colors[] = {
        Engine::Color::White(),
        Engine::Color::Red(),
        Engine::Color::Green(),
        Engine::Color::Blue(),
        Engine::Color::Cyan(),
        Engine::Color::Magenta(),
        Engine::Color::Yellow()
    };
    int colorIndex = 0;

    // Main loop
    bool running = true;
    SDL_Event event;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    colorIndex = (colorIndex + 1) % 7;
                    title.SetColor(colors[colorIndex]);
                }
            }
        }

        // Clear screen
        renderer.Clear(Engine::Color::Black());

        // Draw text centered on screen
        title.DrawCenteredScreen(&renderer, 400, 200);
        subtitle.DrawCenteredScreen(&renderer, 400, 280);
        instructions.DrawCenteredScreen(&renderer, 400, 500);

        // Present
        SDL_RenderPresent(sdlRenderer);

        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
