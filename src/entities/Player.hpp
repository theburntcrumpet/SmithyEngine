#ifndef PLAYER_H
#define PLAYER_H
#include "engine/Entity.hpp"
#include "engine/Input.hpp"
#include "engine/CollisionManager.hpp"
#include "engine/Rectangle.hpp"
#include "engine/SceneManager.hpp"

namespace Entities {
    class Player : public Engine::Entity, public Engine::ICollidable {
    private:
        Engine::Input* m_input = nullptr;
        float m_speed = 100.0f;
        float m_size = 16.0f;
        int m_worldWidth = 0;
        int m_worldHeight = 0;

        Engine::Rectangle<float> m_bounds;
        Engine::CollisionRectangle<float> m_collider;

    public:
        Player(Engine::Vector2f position, Engine::Input* input, int worldWidth, int worldHeight)
            : Entity(position, {"player"}),
              m_input(input),
              m_worldWidth(worldWidth),
              m_worldHeight(worldHeight),
              m_bounds(position, Engine::Vector2f(16.0f, 16.0f)),
              m_collider(&m_bounds) {}

        void Init() override {
            SetRenderLayer(10); // Player renders on top
        }

        void Update(float deltaTime) override {
            if (!m_input) return;

            // Move based on input
            Engine::Vector2f direction = m_input->GetDirection();
            Engine::Vector2f newPos = GetPosition() + direction * m_speed * deltaTime;

            // Clamp to world bounds
            if (newPos.GetX() < 0) newPos.SetX(0);
            if (newPos.GetY() < 0) newPos.SetY(0);
            if (newPos.GetX() > m_worldWidth - m_size) newPos.SetX(static_cast<float>(m_worldWidth) - m_size);
            if (newPos.GetY() > m_worldHeight - m_size) newPos.SetY(static_cast<float>(m_worldHeight) - m_size);

            SetPosition(newPos);
            m_bounds.SetPosition(newPos); // Sync collision bounds

            // Check for collision with win trigger
            if (m_collisionManager && m_sceneManager) {
                if (m_collisionManager->IsCollidingWithTag(this, "win_trigger")) {
                    m_sceneManager->SwitchTo("win");
                }
            }
        }

        void Draw() override {
            if (!m_renderer) return;
            m_renderer->SetColor(Engine::Color::White());
            m_renderer->DrawFilledRect(GetPosition(), static_cast<int>(m_size), static_cast<int>(m_size));
        }

        float GetSize() const { return m_size; }

        // ICollidable interface
        Engine::CollisionRectangle<float>* GetCollider() override {
            return &m_collider;
        }

        Engine::Entity* AsEntity() override {
            return this;
        }
    };
}
#endif
