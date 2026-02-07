#include "engine/AudioManager.hpp"
#include <SDL.h>
#include <iostream>

int main() {
    Engine::AudioManager audio;
    if (!audio.Init()) {
        return -1;
    }

    audio.LoadEffect("cider", "./assets/cider.mp3");
    audio.LoadMusic("shanty", "./assets/sea shanty 96.mp3");

    audio.SetVolume("cider", 0.1f);
    audio.Play("cider", 0);
    audio.Play("shanty", 0);

    std::cout << "playing audio..." << std::endl;

    while (audio.IsPlaying("shanty")) {
        SDL_Delay(10);
    }

    audio.SetVolume("cider", 1.0f);
    audio.Play("cider", 0);

    while (audio.IsPlaying("cider")) {
        SDL_Delay(10);
    }

    return 0;
}
