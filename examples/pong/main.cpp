#include "Game.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    Game game;

    // Internal resolution: 320x180 (16:9 retro resolution)
    // Window scale: 3x (so window starts at 960x540)
    // Press F11 or Alt+Enter to toggle fullscreen
    if (!game.init("My Game", 320, 180, 3)) {
        std::cerr << "Failed to initialize game\n";
        return 1;
    }

    game.run();

    return 0;
}
