#ifndef GAME_META_H
#define GAME_META_H
#include "engine/Vector2.hpp"

namespace Engine {
    class GameMeta {
    private:
        Vector2<int> m_worldSize;

    public:
        GameMeta() : m_worldSize(0, 0) {}
        GameMeta(int width, int height) : m_worldSize(width, height) {}
        GameMeta(Vector2<int> size) : m_worldSize(size) {}

        Vector2<int> GetWorldSize() const { return m_worldSize; }
        int GetWorldWidth() const { return m_worldSize.GetX(); }
        int GetWorldHeight() const { return m_worldSize.GetY(); }

        void SetWorldSize(int width, int height) { m_worldSize = Vector2<int>(width, height); }
        void SetWorldSize(Vector2<int> size) { m_worldSize = size; }
    };
}
#endif
