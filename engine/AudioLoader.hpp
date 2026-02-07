#ifndef AUDIOLOADER_H
#define AUDIOLOADER_H

#define AUDIO_INIT_FREQUENCY MIX_DEFAULT_FREQUENCY
#define AUDIO_INIT_FORMAT MIX_DEFAULT_FORMAT
#define AUDIO_INIT_CHANNELS MIX_DEFAULT_CHANNELS
#define AUDIO_INIT_CHUNKSIZE 4096

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
namespace Engine {
class AudioLoader {
public:
  static bool Init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
      std::cout << "failed to initialize audio system" << std::endl;
      return false;
    }
    if (Mix_OpenAudio(AUDIO_INIT_FREQUENCY, AUDIO_INIT_FORMAT, AUDIO_INIT_CHANNELS, AUDIO_INIT_CHUNKSIZE) == -1) {
        std::cout << "failed to open audio device" << std::endl;
        return false;
    }
    return true;
  }

  static Mix_Chunk *LoadSoundEffect(std::string strPath) {
    Mix_Chunk* wav = Mix_LoadWAV(strPath.c_str());
    if (wav == NULL){
        std::cout << "warn: failed to load sound effect" << std::endl;
    }
    return wav;
  }

  static Mix_Music *LoadMusic(std::string strPath) {
    Mix_Music* mus = Mix_LoadMUS(strPath.c_str());
    if (mus == NULL) {
        std::cout << "warn: failed to load music" << std::endl;
    }
    return mus;
  }
};
} // namespace Engine
#endif