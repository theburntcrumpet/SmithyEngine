#ifndef GRID_H
#define GRID_H
#include "engine/Entity.hpp"

namespace Entities {
    class Grid : public Engine::Entity {
    private:
        int m_worldWidth = 0;
        int m_worldHeight = 0;
        int m_cellSize = 32;

    public:
        Grid(int worldWidth, int worldHeight, int cellSize = 32)
            : Entity(Engine::Vector2f(0.0f), {"grid", "background"}),
              m_worldWidth(worldWidth), m_worldHeight(worldHeight), m_cellSize(cellSize) {}

        void Init() override {
            SetRenderLayer(0); // Background layer
        }

        void Update(float deltaTime) override {
            (void)deltaTime;
            // Grid doesn't need to update
        }

        void Draw() override {
            if (!m_renderer) return;

            // Draw grid cells
            m_renderer->SetColor(40, 40, 60);
            for (int worldX = 0; worldX < m_worldWidth; worldX += m_cellSize) {
                for (int worldY = 0; worldY < m_worldHeight; worldY += m_cellSize) {
                    Engine::Vector2f pos(static_cast<float>(worldX), static_cast<float>(worldY));
                    if (m_renderer->IsVisible(pos, m_cellSize, m_cellSize)) {
                        m_renderer->DrawRect(pos, m_cellSize - 1, m_cellSize - 1);
                    }
                }
            }

            // Draw corner markers
            m_renderer->SetColor(100, 100, 150);
            m_renderer->DrawFilledRect(0.0f, 0.0f, 16, 16);
            m_renderer->DrawFilledRect(static_cast<float>(m_worldWidth - 16), static_cast<float>(m_worldHeight - 16), 16, 16);
        }
    };
}
#endif
