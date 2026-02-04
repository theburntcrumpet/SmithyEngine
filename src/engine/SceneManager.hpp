#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include "engine/Scene.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <any>

namespace Engine {
    class SceneManager {
    private:
        std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
        Scene* m_currentScene = nullptr;
        Scene* m_nextScene = nullptr;
        std::unordered_map<std::string, std::any> m_transitionData;
        Renderer* m_renderer = nullptr;
        bool m_initialized = false;

    public:
        SceneManager() = default;

        void SetRenderer(Renderer* renderer) { m_renderer = renderer; }

        // Register a scene with a name
        template<typename T, typename... Args>
        T* RegisterScene(const std::string& name, Args&&... args) {
            static_assert(std::is_base_of<Scene, T>::value, "T must derive from Scene");
            auto scene = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = scene.get();
            scene->SetSceneManager(this);
            m_scenes[name] = std::move(scene);
            return ptr;
        }

        // Get a registered scene by name
        Scene* GetScene(const std::string& name) {
            auto it = m_scenes.find(name);
            return it != m_scenes.end() ? it->second.get() : nullptr;
        }

        // Switch to a scene by name (happens at end of current frame)
        void SwitchTo(const std::string& name) {
            auto it = m_scenes.find(name);
            if (it != m_scenes.end()) {
                m_nextScene = it->second.get();
            }
        }

        // Switch to a scene with data to pass
        template<typename T>
        void SwitchTo(const std::string& name, const std::string& dataKey, const T& data) {
            m_transitionData[dataKey] = data;
            SwitchTo(name);
        }

        // Switch with multiple data items
        void SwitchTo(const std::string& name, const std::unordered_map<std::string, std::any>& data) {
            m_transitionData = data;
            SwitchTo(name);
        }

        // Get current scene
        Scene* GetCurrentScene() const { return m_currentScene; }

        // Process any pending scene switch
        void ProcessSceneSwitch() {
            if (m_nextScene && m_nextScene != m_currentScene) {
                // Exit current scene
                if (m_currentScene) {
                    m_currentScene->OnExit();
                }

                // Transfer transition data to new scene
                for (const auto& [key, value] : m_transitionData) {
                    m_nextScene->m_sceneData[key] = value;
                }
                m_transitionData.clear();

                // Enter new scene
                m_currentScene = m_nextScene;
                m_nextScene = nullptr;

                m_currentScene->OnEnter();

                // Initialize if we have a renderer and haven't been initialized
                if (m_renderer && !m_initialized) {
                    m_currentScene->Init(*m_renderer);
                }
            }
        }

        // Initialize current scene
        void Init(Renderer& renderer) {
            m_renderer = &renderer;
            m_initialized = true;
            ProcessSceneSwitch();
            if (m_currentScene) {
                m_currentScene->Init(renderer);
            }
        }

        // Update current scene
        void Update(float deltaTime) {
            ProcessSceneSwitch();
            if (m_currentScene) {
                m_currentScene->Update(deltaTime);
            }
        }

        // Draw current scene
        void Draw() {
            if (m_currentScene) {
                m_currentScene->Draw();
            }
        }

        // Handle input for current scene
        void HandleInput(Input& input) {
            if (m_currentScene) {
                m_currentScene->HandleInput(input);
            }
        }

        // Check if a scene exists
        bool HasScene(const std::string& name) const {
            return m_scenes.find(name) != m_scenes.end();
        }

        // Remove a scene
        void RemoveScene(const std::string& name) {
            auto it = m_scenes.find(name);
            if (it != m_scenes.end()) {
                if (m_currentScene == it->second.get()) {
                    m_currentScene = nullptr;
                }
                if (m_nextScene == it->second.get()) {
                    m_nextScene = nullptr;
                }
                m_scenes.erase(it);
            }
        }

        // Clear all scenes
        void Clear() {
            m_currentScene = nullptr;
            m_nextScene = nullptr;
            m_scenes.clear();
            m_transitionData.clear();
        }
    };
}
#endif
