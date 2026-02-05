#ifndef BALL_H
#define BALL_H
#include "engine/CollisionManager.hpp"
#include "engine/CollisionRectangle.hpp"
#include "engine/Entity.hpp"
#include "engine/GameMeta.hpp"
#include "engine/Rectangle.hpp"
#include "engine/Vector2.hpp"
#include <cmath>
#include <iostream>

class Ball : public Engine::Entity, public Engine::ICollidable {
private:
    float m_speed = 150.0f;
    float m_size = 8.0f;
    Engine::Vector2f m_velocity;
    Engine::Rectangle<float> m_bounds;
    Engine::CollisionRectangle<float> m_collider;
    int m_playerScore = 0;
    int m_aiScore = 0;

public:
    Ball(Engine::Vector2f position)
        : Entity(position, {"ball"}),
          m_velocity(m_speed, 0.0f),
          m_bounds(position, Engine::Vector2f(m_size, m_size)),
          m_collider(&m_bounds) {}

    void Init() override {
        SetRenderLayer(5);
        // Start moving diagonally
        m_velocity = Engine::Vector2f(m_speed, m_speed * 0.5f);

        // Register as collidable
        if (m_collisionManager) {
            m_collisionManager->Register(this);
        }
    }

    void Update(float deltaTime) override {
        if (!m_gameMeta) return;

        int worldWidth = m_gameMeta->GetWorldWidth();
        int worldHeight = m_gameMeta->GetWorldHeight();

        Engine::Vector2f pos = GetPosition();
        pos = pos + m_velocity * deltaTime;

        // Bounce off top/bottom walls
        if (pos.GetY() < 0) {
            pos.SetY(0);
            m_velocity.SetY(-m_velocity.GetY());
        }
        if (pos.GetY() > worldHeight - m_size) {
            pos.SetY(static_cast<float>(worldHeight) - m_size);
            m_velocity.SetY(-m_velocity.GetY());
        }

        // Scoring: ball goes off left/right
        if (pos.GetX() < 0) {
            // Ball went off left side - AI scores
            m_aiScore++;
            std::cout << "Score - Player: " << m_playerScore << " | AI: " << m_aiScore << std::endl;
            Reset();
            return;
        }
        if (pos.GetX() > worldWidth - m_size) {
            // Ball went off right side - Player scores
            m_playerScore++;
            std::cout << "Score - Player: " << m_playerScore << " | AI: " << m_aiScore << std::endl;
            Reset();
            return;
        }

        // Check paddle collisions
        if (m_collisionManager) {
            auto collisions = m_collisionManager->GetCollisionsWithTag(this, "paddle");
            for (auto* paddle : collisions) {
                // Bounce off paddle
                m_velocity.SetX(-m_velocity.GetX());

                // Add some angle based on where ball hit the paddle
                float paddleCenter = paddle->GetPosition().GetY() + 24.0f; // half paddle height
                float ballCenter = pos.GetY() + m_size / 2.0f;
                float offset = (ballCenter - paddleCenter) / 24.0f; // -1 to 1
                m_velocity.SetY(offset * m_speed);

                // Push ball out of paddle
                if (m_velocity.GetX() > 0) {
                    pos.SetX(paddle->GetPosition().GetX() + 16.0f + 1.0f);
                } else {
                    pos.SetX(paddle->GetPosition().GetX() - m_size - 1.0f);
                }

                // Speed up slightly each hit
                m_speed *= 1.05f;
                float len = std::sqrt(m_velocity.GetX() * m_velocity.GetX() + m_velocity.GetY() * m_velocity.GetY());
                m_velocity = m_velocity * (m_speed / len);
                break;
            }
        }

        SetPosition(pos);
        m_bounds.SetPosition(pos);
    }

    void Draw() override {
        if (!m_renderer) return;
        m_renderer->SetColor(Engine::Color::White());
        m_renderer->DrawFilledRect(GetPosition(),
            static_cast<int>(m_size),
            static_cast<int>(m_size));
    }

    void Reset() {
        if (!m_gameMeta) return;

        int worldWidth = m_gameMeta->GetWorldWidth();
        int worldHeight = m_gameMeta->GetWorldHeight();

        // Reset to center
        SetPosition(Engine::Vector2f(
            static_cast<float>(worldWidth) / 2.0f - m_size / 2.0f,
            static_cast<float>(worldHeight) / 2.0f - m_size / 2.0f
        ));
        m_bounds.SetPosition(GetPosition());

        // Reset speed and direction (alternate sides)
        m_speed = 150.0f;
        m_velocity = Engine::Vector2f(
            m_velocity.GetX() > 0 ? -m_speed : m_speed,
            m_speed * 0.5f
        );
    }

    void SetVelocity(Engine::Vector2f velocity) {m_velocity = velocity;}

    // ICollidable interface
    Engine::CollisionRectangle<float>* GetCollider() override {
        return &m_collider;
    }

    Engine::Entity* AsEntity() override {
        return this;
    }

    Engine::Vector2f GetVelocity() const { return m_velocity; }
    float GetSize() const { return m_size; }
};
#endif
