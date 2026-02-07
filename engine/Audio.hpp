#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>

namespace Engine {
    enum AudioType { MUSIC, EFFECT };

    class Audio {
    private:
        AudioType type;
        Mix_Music* music = nullptr;
        Mix_Chunk* chunk = nullptr;
        int channel = -1;
        float volume = 1.0f;

    public:
        Audio(const std::string& path, AudioType audioType) : type(audioType) {
            if (type == MUSIC) {
                music = Mix_LoadMUS(path.c_str());
                if (music == nullptr) {
                    std::cout << "warn: failed to load music: " << path << std::endl;
                }
            } else {
                chunk = Mix_LoadWAV(path.c_str());
                if (chunk == nullptr) {
                    std::cout << "warn: failed to load sound effect: " << path << std::endl;
                }
            }
        }

        ~Audio() {
            Free();
        }

        bool Play(int loops = -1) {
            if (type == MUSIC) {
                if (music == nullptr) {
                    std::cout << "warn: null music object, not playing" << std::endl;
                    return false;
                }
                if (Mix_PlayMusic(music, loops) == -1) {
                    std::cout << "warn: couldn't play music" << std::endl;
                    return false;
                }
            } else {
                if (chunk == nullptr) {
                    std::cout << "warn: null chunk object, not playing" << std::endl;
                    return false;
                }
                // For effects, default to 0 loops (play once) if -1 passed
                int effectLoops = (loops == -1) ? 0 : loops;
                channel = Mix_PlayChannel(-1, chunk, effectLoops);
                if (channel == -1) {
                    std::cout << "warn: couldn't play sound effect" << std::endl;
                    return false;
                }
            }
            return true;
        }

        void Stop() {
            if (type == MUSIC) {
                Mix_HaltMusic();
            } else if (channel != -1) {
                Mix_HaltChannel(channel);
                channel = -1;
            }
        }

        bool IsPlaying() const {
            if (type == MUSIC) {
                return Mix_PlayingMusic();
            } else if (channel != -1) {
                return Mix_Playing(channel);
            }
            return false;
        }

        bool SetVolume(float vol) {
            if (vol < 0.0f || vol > 1.0f) return false;
            volume = vol;
            int mixerVolume = static_cast<int>(vol * MIX_MAX_VOLUME);

            if (type == MUSIC) {
                Mix_VolumeMusic(mixerVolume);
            } else if (chunk != nullptr) {
                Mix_VolumeChunk(chunk, mixerVolume);
            }
            return true;
        }

        float GetVolume() const {
            return volume;
        }

        // Set raw mixer volume (0 - MIX_MAX_VOLUME) without changing stored volume
        // Used by AudioManager to apply effective volume (individual * global)
        void SetMixerVolume(int mixerVolume) {
            if (type == MUSIC) {
                Mix_VolumeMusic(mixerVolume);
            } else if (chunk != nullptr) {
                Mix_VolumeChunk(chunk, mixerVolume);
            }
        }

        int GetChannel() const {
            return channel;
        }

        AudioType GetType() const {
            return type;
        }

        bool IsLoaded() const {
            return (type == MUSIC) ? (music != nullptr) : (chunk != nullptr);
        }

        void Free() {
            if (music != nullptr) {
                Mix_FreeMusic(music);
                music = nullptr;
            }
            if (chunk != nullptr) {
                Mix_FreeChunk(chunk);
                chunk = nullptr;
            }
            channel = -1;
        }
    };
}
#endif