#ifndef GAME_SCENE_H
#define GAME_SCENE_H
#include "engine/Scene.hpp"
#include "engine/Camera.hpp"
#include "engine/Input.hpp"
#include "engine/CollisionManager.hpp"
#include "engine/GameMeta.hpp"
#include "entities/Paddle.hpp"
#include "entities/Ball.hpp"

namespace Scenes {
    class GameScene : public Engine::Scene {
    private:
        Engine::Camera* m_camera = nullptr;
        Engine::Input* m_input = nullptr;
        Engine::Renderer* m_renderer = nullptr;
        Engine::CollisionManager m_collisionManager;
        Engine::GameMeta m_gameMeta{320, 180};

        Paddle* m_playerPaddle = nullptr;
        Paddle* m_aiPaddle = nullptr;
        Ball* m_ball = nullptr;
        Ball* m_ball2 = nullptr;

        bool m_initialized = false;

    public:
        GameScene(Engine::Camera* camera, Engine::Input* input)
            : m_camera(camera), m_input(input) {}

        void SetWorldSize(int width, int height) {
            m_gameMeta.SetWorldSize(width, height);
        }

        void OnEnter() override {
            if (m_camera) {
                m_camera->SetBounds(0.0f, 0.0f,
                    static_cast<float>(m_gameMeta.GetWorldWidth()),
                    static_cast<float>(m_gameMeta.GetWorldHeight()));
            }

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
        }

    private:
        void SetupEntities() {
            int worldWidth = m_gameMeta.GetWorldWidth();
            int worldHeight = m_gameMeta.GetWorldHeight();

            float paddleMargin = 16.0f;
            float paddleHeight = 48.0f;
            float centerY = (static_cast<float>(worldHeight) - paddleHeight) / 2.0f;

            // Player paddle on the left
            m_playerPaddle = m_entityManager.Create<Paddle>(
                Engine::Vector2f(paddleMargin, centerY),
                m_input,
                true  // player controlled
            );

            // AI paddle on the right
            m_aiPaddle = m_entityManager.Create<Paddle>(
                Engine::Vector2f(static_cast<float>(worldWidth) - paddleMargin - 16.0f, centerY),
                nullptr,
                false  // AI controlled
            );

            // Ball in center
            m_ball = m_entityManager.Create<Ball>(
                Engine::Vector2f(
                    static_cast<float>(worldWidth) / 2.0f - 4.0f,
                    static_cast<float>(worldHeight) / 2.0f - 4.0f
                )
            );

            m_ball2 = m_entityManager.Create<Ball>(
                Engine::Vector2f(
                    static_cast<float>(worldWidth) / 3.0f - 4.0f,
                    static_cast<float>(worldHeight) / 3.0f - 4.0f
                )
            );

            // Initialize all entities (entities self-register as collidables in Init)
            m_entityManager.InitAll(*m_renderer, &m_collisionManager, &m_gameMeta);

            // Set ball2 to move opposite direction
            m_ball2->SetVelocity(Engine::Vector2f{-m_ball2->GetVelocity().GetX(), 0});
        }

        void ResetScene() {
            m_collisionManager.Clear();
            m_entityManager.Clear();
            m_playerPaddle = nullptr;
            m_aiPaddle = nullptr;
            m_ball = nullptr;

            SetupEntities();
        }
    };
}
#endif
