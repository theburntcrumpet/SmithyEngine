#ifndef SCENE_H
#define SCENE_H
#include "engine/EntityManager.hpp"
#include "engine/Renderer.hpp"
#include "engine/Input.hpp"
#include <string>
#include <any>
#include <unordered_map>

namespace Engine {
    class SceneManager; // Forward declaration

    class Scene {
        friend class SceneManager; // Allow SceneManager to access protected members

    protected:
        EntityManager m_entityManager;
        SceneManager* m_sceneManager = nullptr;
        std::unordered_map<std::string, std::any> m_sceneData;

    public:
        Scene() = default;
        virtual ~Scene() = default;

        // Called by SceneManager when scene is set
        void SetSceneManager(SceneManager* manager) { m_sceneManager = manager; }
        SceneManager* GetSceneManager() const { return m_sceneManager; }

        // Access to entity manager
        EntityManager& GetEntityManager() { return m_entityManager; }

        // Scene data for passing between scenes
        template<typename T>
        void SetData(const std::string& key, const T& value) {
            m_sceneData[key] = value;
        }

        template<typename T>
        T GetData(const std::string& key, const T& defaultValue = T{}) const {
            auto it = m_sceneData.find(key);
            if (it != m_sceneData.end()) {
                try {
                    return std::any_cast<T>(it->second);
                } catch (const std::bad_any_cast&) {
                    return defaultValue;
                }
            }
            return defaultValue;
        }

        bool HasData(const std::string& key) const {
            return m_sceneData.find(key) != m_sceneData.end();
        }

        void ClearData() { m_sceneData.clear(); }

        // Lifecycle methods - override in derived classes
        virtual void OnEnter() {}  // Called when scene becomes active
        virtual void OnExit() {}   // Called when scene is being switched away from

        virtual void Init(Renderer& renderer) {
            m_entityManager.InitAll(renderer);
        }

        virtual void Update(float deltaTime) {
            m_entityManager.UpdateAll(deltaTime);
        }

        virtual void Draw() {
            m_entityManager.DrawAll();
        }

        // Handle input events - override if needed
        virtual void HandleInput(Input& input) { (void)input; }
    };
}
#endif
