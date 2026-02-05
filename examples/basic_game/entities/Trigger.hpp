#ifndef TRIGGER_H
#define TRIGGER_H
#include "engine/Entity.hpp"
#include "engine/CollisionManager.hpp"
#include "engine/Rectangle.hpp"

namespace Entities {
    class Trigger : public Engine::Entity, public Engine::ICollidable {
    private:
        Engine::Rectangle<float> m_bounds;
        Engine::CollisionRectangle<float> m_collider;
        float m_width;
        float m_height;
        bool m_visible = false;  // Triggers are usually invisible

    public:
        Trigger(Engine::Vector2f position, float width, float height, std::initializer_list<std::string> tags)
            : Entity(position, tags),
              m_bounds(position, Engine::Vector2f(width, height)),
              m_collider(&m_bounds),
              m_width(width),
              m_height(height) {}

        void Init() override {
            SetRenderLayer(5); // Middle layer
        }

        void Update(float deltaTime) override {
            (void)deltaTime;
            // Triggers don't move, but sync bounds position just in case
            m_bounds.SetPosition(GetPosition());
        }

        void Draw() override {
            if (!m_renderer || !m_visible) return;

            // Draw semi-transparent indicator for debugging
            m_renderer->SetColor(255, 255, 0, 128);
            m_renderer->DrawRect(GetPosition(), static_cast<int>(m_width), static_cast<int>(m_height));
        }

        // ICollidable interface
        Engine::CollisionRectangle<float>* GetCollider() override {
            return &m_collider;
        }

        Engine::Entity* AsEntity() override {
            return this;
        }

        // Visibility control for debugging
        void SetVisible(bool visible) { m_visible = visible; }
        bool IsVisible() const { return m_visible; }
    };
}
#endif
