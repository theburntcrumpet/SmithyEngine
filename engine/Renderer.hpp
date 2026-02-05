#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include "engine/Vector2.hpp"
#include "engine/Camera.hpp"
namespace Engine {

    struct Color {
        Uint8 r, g, b, a;
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) : r(r), g(g), b(b), a(a) {}

        static Color White() { return Color(255, 255, 255); }
        static Color Black() { return Color(0, 0, 0); }
        static Color Red() { return Color(255, 0, 0); }
        static Color Green() { return Color(0, 255, 0); }
        static Color Blue() { return Color(0, 0, 255); }
        static Color Yellow() { return Color(255, 255, 0); }
        static Color Cyan() { return Color(0, 255, 255); }
        static Color Magenta() { return Color(255, 0, 255); }
    };

    class Renderer {
        private:
            SDL_Renderer* m_sdlRenderer = nullptr;
            Camera* m_camera = nullptr;

        public:
            Renderer() = default;

            void SetSDLRenderer(SDL_Renderer* renderer) { m_sdlRenderer = renderer; }
            void SetCamera(Camera* camera) { m_camera = camera; }

            SDL_Renderer* GetSDLRenderer() { return m_sdlRenderer; }
            Camera* GetCamera() { return m_camera; }

            // Set draw color
            void SetColor(const Color& color) {
                SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.g, color.b, color.a);
            }

            void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
                SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, a);
            }

            // Clear screen
            void Clear() {
                SDL_RenderClear(m_sdlRenderer);
            }

            void Clear(const Color& color) {
                SetColor(color);
                SDL_RenderClear(m_sdlRenderer);
            }

            // Draw filled rectangle (world coordinates - uses camera)
            void DrawFilledRect(const Vector2<float>& worldPos, int width, int height) {
                Vector2<float> screenPos = WorldToScreen(worldPos);
                SDL_Rect rect = {
                    static_cast<int>(screenPos.GetX()),
                    static_cast<int>(screenPos.GetY()),
                    width,
                    height
                };
                SDL_RenderFillRect(m_sdlRenderer, &rect);
            }

            void DrawFilledRect(float worldX, float worldY, int width, int height) {
                DrawFilledRect(Vector2<float>(worldX, worldY), width, height);
            }

            // Draw rectangle outline (world coordinates - uses camera)
            void DrawRect(const Vector2<float>& worldPos, int width, int height) {
                Vector2<float> screenPos = WorldToScreen(worldPos);
                SDL_Rect rect = {
                    static_cast<int>(screenPos.GetX()),
                    static_cast<int>(screenPos.GetY()),
                    width,
                    height
                };
                SDL_RenderDrawRect(m_sdlRenderer, &rect);
            }

            void DrawRect(float worldX, float worldY, int width, int height) {
                DrawRect(Vector2<float>(worldX, worldY), width, height);
            }

            // Draw filled rectangle (screen coordinates - ignores camera)
            void DrawFilledRectScreen(int x, int y, int width, int height) {
                SDL_Rect rect = { x, y, width, height };
                SDL_RenderFillRect(m_sdlRenderer, &rect);
            }

            // Draw rectangle outline (screen coordinates - ignores camera)
            void DrawRectScreen(int x, int y, int width, int height) {
                SDL_Rect rect = { x, y, width, height };
                SDL_RenderDrawRect(m_sdlRenderer, &rect);
            }

            // Draw line (world coordinates)
            void DrawLine(const Vector2<float>& worldStart, const Vector2<float>& worldEnd) {
                Vector2<float> screenStart = WorldToScreen(worldStart);
                Vector2<float> screenEnd = WorldToScreen(worldEnd);
                SDL_RenderDrawLine(m_sdlRenderer,
                    static_cast<int>(screenStart.GetX()),
                    static_cast<int>(screenStart.GetY()),
                    static_cast<int>(screenEnd.GetX()),
                    static_cast<int>(screenEnd.GetY())
                );
            }

            // Draw line (screen coordinates)
            void DrawLineScreen(int x1, int y1, int x2, int y2) {
                SDL_RenderDrawLine(m_sdlRenderer, x1, y1, x2, y2);
            }

            // Draw point (world coordinates)
            void DrawPoint(const Vector2<float>& worldPos) {
                Vector2<float> screenPos = WorldToScreen(worldPos);
                SDL_RenderDrawPoint(m_sdlRenderer,
                    static_cast<int>(screenPos.GetX()),
                    static_cast<int>(screenPos.GetY())
                );
            }

            // Draw sprite (world coordinates)
            // srcRect: portion of texture to draw (nullptr for entire texture)
            // width/height: destination size on screen
            void DrawSprite(SDL_Texture* texture, const SDL_Rect* srcRect,
                           const Vector2<float>& worldPos, int width, int height) {
                Vector2<float> screenPos = WorldToScreen(worldPos);
                SDL_Rect destRect = {
                    static_cast<int>(screenPos.GetX()),
                    static_cast<int>(screenPos.GetY()),
                    width,
                    height
                };
                SDL_RenderCopy(m_sdlRenderer, texture, srcRect, &destRect);
            }

            // Draw sprite with rotation and flip (world coordinates)
            void DrawSpriteEx(SDL_Texture* texture, const SDL_Rect* srcRect,
                             const Vector2<float>& worldPos, int width, int height,
                             double angle, SDL_RendererFlip flip = SDL_FLIP_NONE) {
                Vector2<float> screenPos = WorldToScreen(worldPos);
                SDL_Rect destRect = {
                    static_cast<int>(screenPos.GetX()),
                    static_cast<int>(screenPos.GetY()),
                    width,
                    height
                };
                SDL_RenderCopyEx(m_sdlRenderer, texture, srcRect, &destRect, angle, nullptr, flip);
            }

            // Draw sprite (screen coordinates - ignores camera)
            void DrawSpriteScreen(SDL_Texture* texture, const SDL_Rect* srcRect,
                                  int x, int y, int width, int height) {
                SDL_Rect destRect = { x, y, width, height };
                SDL_RenderCopy(m_sdlRenderer, texture, srcRect, &destRect);
            }

            // Check if a world rectangle is visible on screen (for culling)
            bool IsVisible(const Vector2<float>& worldPos, int width, int height) const {
                if (!m_camera) return true;

                Vector2<float> screenPos = m_camera->WorldToScreen(worldPos);
                Vector2<float> camSize = m_camera->GetSize();

                return screenPos.GetX() + width > 0 &&
                       screenPos.GetX() < camSize.GetX() &&
                       screenPos.GetY() + height > 0 &&
                       screenPos.GetY() < camSize.GetY();
            }

        private:
            Vector2<float> WorldToScreen(const Vector2<float>& worldPos) const {
                if (m_camera) {
                    return m_camera->WorldToScreen(worldPos);
                }
                return worldPos;
            }
    };
}
#endif
