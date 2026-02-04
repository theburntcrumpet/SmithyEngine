#ifndef GAME_SCENE_H
#define GAME_SCENE_H
#include "engine/Scene.hpp"
#include "engine/Camera.hpp"
#include "engine/Input.hpp"
#include "engine/CollisionManager.hpp"
#include "entities/Player.hpp"
#include "entities/Grid.hpp"
#include "entities/Trigger.hpp"

namespace Scenes {
    class GameScene : public Engine::Scene {
    private:
        Engine::Camera* m_camera = nullptr;
        Engine::Input* m_input = nullptr;
        Engine::Renderer* m_renderer = nullptr;
        Engine::CollisionManager m_collisionManager;

        Entities::Player* m_player = nullptr;
        Entities::Trigger* m_winTrigger = nullptr;

        int m_worldWidth = 640;
        int m_worldHeight = 360;
        bool m_initialized = false;

    public:
        GameScene(Engine::Camera* camera, Engine::Input* input)
            : m_camera(camera), m_input(input) {}

        void SetWorldSize(int width, int height) {
            m_worldWidth = width;
            m_worldHeight = height;
        }

        void OnEnter() override {
            // Set camera bounds when entering scene
            if (m_camera) {
                m_camera->SetBounds(0.0f, 0.0f,
                    static_cast<float>(m_worldWidth),
                    static_cast<float>(m_worldHeight));
            }

            // If returning to scene, reset it
            if (m_initialized && m_renderer) {
                ResetScene();
            }
        }

        void Init(Engine::Renderer& renderer) override {
            m_renderer = &renderer;
            SetupEntities();
            m_initialized = true;
        }

        void Update(float deltaTime) override {
            m_entityManager.UpdateAll(deltaTime);

            // Camera follows player with deadzone
            if (m_camera && m_player) {
                float playerSize = m_player->GetSize();
                m_camera->FollowWithDeadzone(
                    m_player->GetPosition() + Engine::Vector2f(playerSize / 2, playerSize / 2),
                    64.0f
                );
            }
        }

        Entities::Player* GetPlayer() const { return m_player; }

    private:
        void SetupEntities() {
            // Create game entities
            m_entityManager.Create<Entities::Grid>(m_worldWidth, m_worldHeight, 32);

            m_player = m_entityManager.Create<Entities::Player>(
                Engine::Vector2f(100.0f, 75.0f),
                m_input,
                m_worldWidth,
                m_worldHeight
            );

            // Create win trigger in bottom-right corner
            m_winTrigger = m_entityManager.Create<Entities::Trigger>(
                Engine::Vector2f(static_cast<float>(m_worldWidth - 16), static_cast<float>(m_worldHeight - 16)),
                16.0f, 16.0f,
                std::initializer_list<std::string>{"trigger", "win_trigger"}
            );

            // Set dependencies on entities that need them (before Init)
            m_player->SetCollisionManager(&m_collisionManager);
            m_player->SetSceneManager(m_sceneManager);

            // Register collidables
            m_collisionManager.Register(m_player);
            m_collisionManager.Register(m_winTrigger);

            // Initialize all entities (sets renderer and entityManager automatically)
            m_entityManager.InitAll(*m_renderer);

            // Center camera on player
            if (m_camera && m_player) {
                m_camera->CenterOn(m_player->GetPosition());
            }
        }

        void ResetScene() {
            // Clear everything
            m_collisionManager.Clear();
            m_entityManager.Clear();
            m_player = nullptr;
            m_winTrigger = nullptr;

            // Recreate
            SetupEntities();
        }
    };
}
#endif
