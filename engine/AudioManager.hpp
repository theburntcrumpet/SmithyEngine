#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "engine/Audio.hpp"
#include "engine/AudioLoader.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace Engine {
    class AudioManager {
    private:
        std::unordered_map<std::string, std::unique_ptr<Audio>> m_audio;
        bool m_initialized = false;
        float m_musicVolume = 1.0f;
        float m_sfxVolume = 1.0f;

        // Apply effective volume (individual * global) to an audio object
        void ApplyEffectiveVolume(Audio* audio) {
            if (!audio) return;
            float globalVol = (audio->GetType() == AudioType::MUSIC) ? m_musicVolume : m_sfxVolume;
            float effectiveVol = audio->GetVolume() * globalVol;
            int mixerVol = static_cast<int>(effectiveVol * MIX_MAX_VOLUME);

            if (audio->GetType() == AudioType::MUSIC) {
                Mix_VolumeMusic(mixerVol);
            } else {
                audio->SetMixerVolume(mixerVol);
            }
        }

    public:
        AudioManager() = default;

        // Initialize the audio system (must call before loading audio)
        bool Init() {
            if (m_initialized) return true;
            m_initialized = AudioLoader::Init();
            return m_initialized;
        }

        // Load and register audio with a name
        Audio* Load(const std::string& name, const std::string& path, AudioType type) {
            auto audio = std::make_unique<Audio>(path, type);
            if (!audio->IsLoaded()) {
                return nullptr;
            }
            Audio* ptr = audio.get();
            m_audio[name] = std::move(audio);
            return ptr;
        }

        // Load music (convenience method)
        Audio* LoadMusic(const std::string& name, const std::string& path) {
            return Load(name, path, AudioType::MUSIC);
        }

        // Load sound effect (convenience method)
        Audio* LoadEffect(const std::string& name, const std::string& path) {
            return Load(name, path, AudioType::EFFECT);
        }

        // Get audio by name (nullptr if not found)
        Audio* Get(const std::string& name) {
            auto it = m_audio.find(name);
            return it != m_audio.end() ? it->second.get() : nullptr;
        }

        // Play audio by name (returns false if not found)
        bool Play(const std::string& name, int loops = -1) {
            Audio* audio = Get(name);
            if (audio) {
                ApplyEffectiveVolume(audio);
                return audio->Play(loops);
            }
            return false;
        }

        // Stop audio by name
        void Stop(const std::string& name) {
            Audio* audio = Get(name);
            if (audio) {
                audio->Stop();
            }
        }

        // Stop all audio
        void StopAll() {
            Mix_HaltMusic();
            Mix_HaltChannel(-1);
        }

        // Check if audio is playing by name
        bool IsPlaying(const std::string& name) {
            Audio* audio = Get(name);
            return audio ? audio->IsPlaying() : false;
        }

        // Set volume by name (0.0 - 1.0)
        bool SetVolume(const std::string& name, float volume) {
            Audio* audio = Get(name);
            if (!audio) return false;
            audio->SetVolume(volume);
            ApplyEffectiveVolume(audio);
            return true;
        }

        // Set global music volume (0.0 - 1.0)
        void SetMusicVolume(float volume) {
            if (volume < 0.0f) volume = 0.0f;
            if (volume > 1.0f) volume = 1.0f;
            m_musicVolume = volume;
            // Update all loaded music with new global volume
            for (auto& [name, audio] : m_audio) {
                if (audio && audio->GetType() == AudioType::MUSIC) {
                    ApplyEffectiveVolume(audio.get());
                }
            }
        }

        // Set global SFX volume (0.0 - 1.0)
        void SetSfxVolume(float volume) {
            if (volume < 0.0f) volume = 0.0f;
            if (volume > 1.0f) volume = 1.0f;
            m_sfxVolume = volume;
            // Update all loaded effects with new global volume
            for (auto& [name, audio] : m_audio) {
                if (audio && audio->GetType() == AudioType::EFFECT) {
                    ApplyEffectiveVolume(audio.get());
                }
            }
        }

        float GetMusicVolume() const { return m_musicVolume; }
        float GetSfxVolume() const { return m_sfxVolume; }

        // Unload audio by name
        void Unload(const std::string& name) {
            m_audio.erase(name);
        }

        // Check if audio exists
        bool Has(const std::string& name) const {
            return m_audio.find(name) != m_audio.end();
        }

        // Get count of loaded audio
        size_t Count() const {
            return m_audio.size();
        }

        // Clear all audio
        void Clear() {
            m_audio.clear();
        }

        // Shutdown audio system
        void Shutdown() {
            Clear();
            if (m_initialized) {
                Mix_CloseAudio();
                m_initialized = false;
            }
        }

        ~AudioManager() {
            Shutdown();
        }
    };
}
#endif
