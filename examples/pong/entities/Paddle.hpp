#ifndef PADDLE_H
#define PADDLE_H
#include "engine/CollisionManager.hpp"
#include "engine/CollisionRectangle.hpp"
#include "engine/Entity.hpp"
#include "engine/GameMeta.hpp"
#include "engine/Input.hpp"
#include "engine/Rectangle.hpp"
#include "engine/Vector2.hpp"
#include <cmath>

class Paddle : public Engine::Entity, public Engine::ICollidable {
private:
    Engine::Input* m_input = nullptr;
    float m_speed = 200.0f;
    Engine::Vector2f m_paddleSize = {16.0f, 48.0f};
    Engine::Rectangle<float> m_bounds;
    Engine::CollisionRectangle<float> m_collider;
    bool m_bPlayerControlled = false;

public:
    Paddle(Engine::Vector2f position, Engine::Input* input, bool playerControlled = true)
        : Entity(position, {"paddle"}),
          m_input(input),
          m_bounds(position, m_paddleSize),
          m_collider(&m_bounds),
          m_bPlayerControlled(playerControlled) {
        // AI paddle is slower to give player a chance to win
        if (!playerControlled) {
            m_speed = 120.0f;
        }
    }

    void Init() override {
        SetRenderLayer(5);

        // Register as collidable
        if (m_collisionManager) {
            m_collisionManager->Register(this);
        }
    }

    void Update(float deltaTime) override {
        if (!m_gameMeta) return;

        int worldHeight = m_gameMeta->GetWorldHeight();
        float direction = 0.0f;

        if (m_bPlayerControlled) {
            if (!m_input) return;
            direction = m_input->GetDirection().GetY();
        } else {
            // AI: follow the ball
            if (m_entityManager) {
                Engine::Entity* ball = m_entityManager->FindByTag("ball");
                if (ball) {
                    float ballY = ball->GetPosition().GetY();
                    float paddleCenter = GetPosition().GetY() + m_paddleSize.GetY() / 2.0f;
                    float diff = ballY - paddleCenter;

                    // Add dead zone to prevent jittering
                    if (std::abs(diff) > 5.0f) {
                        direction = diff > 0 ? 1.0f : -1.0f;
                    }
                }
            }
        }

        Engine::Vector2f newPos = GetPosition();
        newPos.SetY(newPos.GetY() + direction * m_speed * deltaTime);

        // Clamp to world bounds
        if (newPos.GetY() < 0) newPos.SetY(0);
        if (newPos.GetY() > worldHeight - m_paddleSize.GetY()) {
            newPos.SetY(static_cast<float>(worldHeight) - m_paddleSize.GetY());
        }

        SetPosition(newPos);
        m_bounds.SetPosition(newPos);
    }

    void Draw() override {
        if (!m_renderer) return;
        m_renderer->SetColor(Engine::Color::White());
        m_renderer->DrawFilledRect(GetPosition(),
            static_cast<int>(m_paddleSize.GetX()),
            static_cast<int>(m_paddleSize.GetY()));
    }

    // ICollidable interface
    Engine::CollisionRectangle<float>* GetCollider() override {
        return &m_collider;
    }

    Engine::Entity* AsEntity() override {
        return this;
    }

    Engine::Vector2f GetSize() const { return m_paddleSize; }
};
#endif
