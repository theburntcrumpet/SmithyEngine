#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
#include <SDL_mixer.h>
#include <cstddef>
#include <iostream>
namespace Engine{
    class AudioPlayer{
        public:
            static bool PlayMusic(Mix_Music* music, int loops = -1){
                if (music == NULL) {
                    std::cout << "warn: null music object passed, not playing" << std::endl;
                    return false;
                }
                if(Mix_PlayMusic(music,loops) == -1) {
                    std::cout << "warn: couldn't play music" << std::endl;
                    return false;
                }
                return true;
            }

            // defaults to a one shot sound effect on next available channel
            // returns the channel number on success, -1 on failure
            static int PlaySoundEffect(Mix_Chunk* chunk, int channel = -1, int loops = 0){
                if (chunk == NULL) {
                    std::cout << "warn: null chunk object passed, not playing" << std::endl;
                    return -1;
                }
                int ch = Mix_PlayChannel(channel, chunk, loops);
                if(ch == -1){
                    std::cout << "warn: couldn't play sound effect" << std::endl;
                }
                return ch;
            }

            static bool IsChannelPlaying(int channel) {
                return channel != -1 && Mix_Playing(channel);
            }

            // provide a volume between 0.0 and 1.0
            static bool SetMusicVolume(float volume) {
                if (volume < 0.0f || volume > 1.0f) { return false; }
                int mixerVolume = volume * MIX_MAX_VOLUME;
                if(Mix_VolumeMusic(mixerVolume) == -1){
                    std::cout << "warn: couldn't set mixer volume" << std::endl;
                    return false;
                }
                return true;
            }

            // provide normalized volume
            static bool SetChunkVolume(float volume, Mix_Chunk* chunk) {
                if (chunk == NULL || volume < 0.0f || volume > 1.0f) return false;
                int mixerVolume = volume * MIX_MAX_VOLUME;
                if(Mix_VolumeChunk(chunk, mixerVolume) == -1){
                    std::cout << "warn: couldn't set chunk volume" << std::endl;
                    return false;
                }
                return true;
            }

            static void FreeChunk(Mix_Chunk* chunk) {
                Mix_FreeChunk(chunk);
            }

            static void FreeMusic(Mix_Music* music) {
                Mix_FreeMusic(music);
            }

            static bool IsMusicPlaying(){
                return Mix_PlayingMusic();
            }
    };
}
#endif