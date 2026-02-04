#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include "engine/Entity.hpp"
#include "engine/Renderer.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <unordered_map>

namespace Engine {
    class EntityManager {
    private:
        std::vector<std::unique_ptr<Entity>> m_entities;
        std::unordered_map<std::string, std::vector<Entity*>> m_tagIndex;
        std::vector<Entity*> m_pendingRemoval;
        bool m_needsSort = false;

    public:
        EntityManager() = default;

        // Add an entity (takes ownership)
        template<typename T, typename... Args>
        T* Create(Args&&... args) {
            static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity");
            auto entity = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = entity.get();
            AddToTagIndex(ptr);
            m_entities.push_back(std::move(entity));
            m_needsSort = true;
            return ptr;
        }

        // Add an existing entity (takes ownership)
        Entity* Add(std::unique_ptr<Entity> entity) {
            Entity* ptr = entity.get();
            AddToTagIndex(ptr);
            m_entities.push_back(std::move(entity));
            m_needsSort = true;
            return ptr;
        }

        // Mark entity for removal (safe to call during update)
        void Remove(Entity* entity) {
            m_pendingRemoval.push_back(entity);
        }

        // Remove all entities with a specific tag
        void RemoveByTag(const std::string& tag) {
            auto it = m_tagIndex.find(tag);
            if (it != m_tagIndex.end()) {
                for (Entity* entity : it->second) {
                    m_pendingRemoval.push_back(entity);
                }
            }
        }

        // Get first entity with tag (nullptr if not found) - O(1)
        Entity* FindByTag(const std::string& tag) {
            auto it = m_tagIndex.find(tag);
            if (it != m_tagIndex.end() && !it->second.empty()) {
                return it->second.front();
            }
            return nullptr;
        }

        // Get all entities with tag - O(1)
        const std::vector<Entity*>& FindAllByTag(const std::string& tag) {
            auto it = m_tagIndex.find(tag);
            if (it != m_tagIndex.end()) {
                return it->second;
            }
            static const std::vector<Entity*> empty;
            return empty;
        }

        // Get all entities (for custom iteration)
        std::vector<Entity*> GetAll() {
            std::vector<Entity*> result;
            result.reserve(m_entities.size());
            for (auto& entity : m_entities) {
                if (entity) {
                    result.push_back(entity.get());
                }
            }
            return result;
        }

        // Iterate over entities with a callback
        void ForEach(const std::function<void(Entity*)>& callback) {
            for (auto& entity : m_entities) {
                if (entity) {
                    callback(entity.get());
                }
            }
        }

        // Iterate over entities with specific tag - O(n) where n = entities with tag
        void ForEachWithTag(const std::string& tag, const std::function<void(Entity*)>& callback) {
            auto it = m_tagIndex.find(tag);
            if (it != m_tagIndex.end()) {
                for (Entity* entity : it->second) {
                    callback(entity);
                }
            }
        }

        // Initialize all entities (injects renderer and entityManager, then calls Init)
        void InitAll(Renderer& renderer) {
            for (auto& entity : m_entities) {
                if (entity) {
                    entity->SetRenderer(&renderer);
                    entity->SetEntityManager(this);
                    entity->Init();
                }
            }
        }

        // Update all entities
        void UpdateAll(float deltaTime) {
            for (auto& entity : m_entities) {
                if (entity) {
                    entity->Update(deltaTime);
                }
            }
            ProcessRemovals();
        }

        // Draw all entities (sorted by render layer)
        void DrawAll() {
            if (m_needsSort) {
                SortByRenderLayer();
                m_needsSort = false;
            }

            for (auto& entity : m_entities) {
                if (entity) {
                    entity->Draw();
                }
            }
        }

        // Force re-sort (call if you change an entity's render layer)
        void MarkDirty() {
            m_needsSort = true;
        }

        // Get entity count
        size_t Count() const {
            return m_entities.size();
        }

        // Get count of entities with specific tag - O(1)
        size_t CountByTag(const std::string& tag) const {
            auto it = m_tagIndex.find(tag);
            return it != m_tagIndex.end() ? it->second.size() : 0;
        }

        // Clear all entities
        void Clear() {
            m_entities.clear();
            m_tagIndex.clear();
            m_pendingRemoval.clear();
        }

    private:
        void AddToTagIndex(Entity* entity) {
            for (const std::string& tag : entity->GetTags()) {
                m_tagIndex[tag].push_back(entity);
            }
        }

        void RemoveFromTagIndex(Entity* entity) {
            for (const std::string& tag : entity->GetTags()) {
                auto it = m_tagIndex.find(tag);
                if (it != m_tagIndex.end()) {
                    auto& vec = it->second;
                    vec.erase(std::remove(vec.begin(), vec.end(), entity), vec.end());
                }
            }
        }

        void ProcessRemovals() {
            if (m_pendingRemoval.empty()) return;

            for (Entity* toRemove : m_pendingRemoval) {
                RemoveFromTagIndex(toRemove);
                auto it = std::find_if(m_entities.begin(), m_entities.end(),
                    [toRemove](const std::unique_ptr<Entity>& e) {
                        return e.get() == toRemove;
                    });
                if (it != m_entities.end()) {
                    m_entities.erase(it);
                }
            }
            m_pendingRemoval.clear();
        }

        void SortByRenderLayer() {
            std::stable_sort(m_entities.begin(), m_entities.end(),
                [](const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b) {
                    return a->GetRenderLayer() < b->GetRenderLayer();
                });
        }
    };
}
#endif
