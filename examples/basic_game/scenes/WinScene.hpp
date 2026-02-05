#ifndef WIN_SCENE_H
#define WIN_SCENE_H
#include "engine/Scene.hpp"
#include "engine/SceneManager.hpp"

namespace Scenes {
    class WinScene : public Engine::Scene {
    private:
        Engine::Input* m_input = nullptr;
        Engine::Renderer* m_renderer = nullptr;

    public:
        WinScene(Engine::Input* input) : m_input(input) {}

        void OnEnter() override {
            // Could read data passed from previous scene
            // int score = GetData<int>("score", 0);
        }

        void Init(Engine::Renderer& renderer) override {
            m_renderer = &renderer;
        }

        void Update(float deltaTime) override {
            (void)deltaTime;

            // Press primary button to restart
            if (m_input && m_input->GetPrimary().GetJustPressed()) {
                m_sceneManager->SwitchTo("game");
            }
        }

        void Draw() override {
            if (!m_renderer) return;

            // Draw a simple "You Win" indicator (green rectangle in center)
            m_renderer->SetColor(0, 200, 0);
            m_renderer->DrawFilledRect(80.0f, 60.0f, 160, 60);

            // Draw white border
            m_renderer->SetColor(Engine::Color::White());
            m_renderer->DrawRect(80.0f, 60.0f, 160, 60);
        }
    };
}
#endif
