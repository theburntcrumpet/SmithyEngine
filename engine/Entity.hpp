#ifndef ENTITY_H
#define ENTITY_H
#include "engine/Renderer.hpp"
#include "engine/Vector2.hpp"
#include <initializer_list>
#include <string>
#include <set>

namespace Engine {
    // Forward declarations to avoid circular includes
    class EntityManager;
    class CollisionManager;
    class SceneManager;
    class GameMeta;
    class AudioManager;

    class Entity {
    private:
        Vector2f m_position = Vector2f(0.0f);
        std::set<std::string> m_tags;
        int m_nRenderLayer = 0;

    protected:
        // Dependencies - set at init time, used throughout entity lifetime
        Renderer* m_renderer = nullptr;
        EntityManager* m_entityManager = nullptr;
        CollisionManager* m_collisionManager = nullptr;
        SceneManager* m_sceneManager = nullptr;
        GameMeta* m_gameMeta = nullptr;
        AudioManager* m_audioManager = nullptr;

    public:
        Entity() = default;
        virtual ~Entity() = default;
        Entity(Vector2f position) : m_position(position) {}
        Entity(Vector2f position, std::initializer_list<std::string> tags) : m_position(position), m_tags(tags) {}

        // Getters
        Vector2f GetPosition() const { return m_position; }
        const std::set<std::string>& GetTags() const { return m_tags; }
        int GetRenderLayer() const { return m_nRenderLayer; }

        // Setters for core properties
        void SetPosition(Vector2f position) { m_position = position; }
        void SetRenderLayer(int renderLayer) { m_nRenderLayer = renderLayer; }

        // Dependency injection - call these before/during Init
        void SetRenderer(Renderer* renderer) { m_renderer = renderer; }
        void SetEntityManager(EntityManager* entityManager) { m_entityManager = entityManager; }
        void SetCollisionManager(CollisionManager* collisionManager) { m_collisionManager = collisionManager; }
        void SetSceneManager(SceneManager* sceneManager) { m_sceneManager = sceneManager; }
        void SetGameMeta(GameMeta* gameMeta) { m_gameMeta = gameMeta; }
        void SetAudioManager(AudioManager* audioManager) { m_audioManager = audioManager; }

        // Lifecycle methods - override in derived classes
        virtual void Init() {}
        virtual void Update(float deltaTime) { (void)deltaTime; }
        virtual void Draw() {}
    };
}
#endif
