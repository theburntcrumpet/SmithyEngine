#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H
#include "engine/Entity.hpp"
#include "engine/CollisionRectangle.hpp"
#include <vector>
#include <functional>

namespace Engine {
    // Interface for entities that can collide
    class ICollidable {
    public:
        virtual ~ICollidable() = default;
        virtual CollisionRectangle<float>* GetCollider() = 0;
        virtual Entity* AsEntity() = 0;
    };

    class CollisionManager {
    private:
        std::vector<ICollidable*> m_collidables;

    public:
        CollisionManager() = default;

        // Register a collidable entity
        void Register(ICollidable* collidable) {
            m_collidables.push_back(collidable);
        }

        // Unregister a collidable entity
        void Unregister(ICollidable* collidable) {
            m_collidables.erase(
                std::remove(m_collidables.begin(), m_collidables.end(), collidable),
                m_collidables.end()
            );
        }

        // Clear all registered collidables
        void Clear() {
            m_collidables.clear();
        }

        // Get all entities colliding with the given collidable
        std::vector<Entity*> GetCollisions(ICollidable* collidable) {
            std::vector<Entity*> results;
            auto* collider = collidable->GetCollider();
            if (!collider) return results;

            for (auto* other : m_collidables) {
                if (other == collidable) continue;
                auto* otherCollider = other->GetCollider();
                if (otherCollider && collider->IsColliding(*otherCollider)) {
                    results.push_back(other->AsEntity());
                }
            }
            return results;
        }

        // Get all entities with a specific tag that are colliding with the given collidable
        std::vector<Entity*> GetCollisionsWithTag(ICollidable* collidable, const std::string& tag) {
            std::vector<Entity*> results;
            auto* collider = collidable->GetCollider();
            if (!collider) return results;

            for (auto* other : m_collidables) {
                if (other == collidable) continue;
                Entity* entity = other->AsEntity();
                if (!entity->GetTags().count(tag)) continue;

                auto* otherCollider = other->GetCollider();
                if (otherCollider && collider->IsColliding(*otherCollider)) {
                    results.push_back(entity);
                }
            }
            return results;
        }

        // Check if collidable is colliding with any entity with the given tag
        bool IsCollidingWithTag(ICollidable* collidable, const std::string& tag) {
            auto* collider = collidable->GetCollider();
            if (!collider) return false;

            for (auto* other : m_collidables) {
                if (other == collidable) continue;
                Entity* entity = other->AsEntity();
                if (!entity->GetTags().count(tag)) continue;

                auto* otherCollider = other->GetCollider();
                if (otherCollider && collider->IsColliding(*otherCollider)) {
                    return true;
                }
            }
            return false;
        }

        // Iterate over all collisions with a callback
        void ForEachCollision(ICollidable* collidable, const std::function<void(Entity*)>& callback) {
            auto* collider = collidable->GetCollider();
            if (!collider) return;

            for (auto* other : m_collidables) {
                if (other == collidable) continue;
                auto* otherCollider = other->GetCollider();
                if (otherCollider && collider->IsColliding(*otherCollider)) {
                    callback(other->AsEntity());
                }
            }
        }
    };
}
#endif
