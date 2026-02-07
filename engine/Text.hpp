#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include "engine/Renderer.hpp"
#include "engine/Vector2.hpp"

namespace Engine {

    class Text {
    private:
        TTF_Font* m_font = nullptr;
        SDL_Texture* m_texture = nullptr;
        SDL_Renderer* m_sdlRenderer = nullptr;
        std::string m_text;
        Color m_color = Color::White();
        int m_width = 0;
        int m_height = 0;
        bool m_dirty = true;

    public:
        Text() = default;

        Text(Renderer* renderer, const std::string& fontPath, int fontSize) {
            Init(renderer, fontPath, fontSize);
        }

        ~Text() {
            Free();
        }

        // Initialize with renderer and font
        bool Init(Renderer* renderer, const std::string& fontPath, int fontSize) {
            if (!renderer || !renderer->GetSDLRenderer()) {
                std::cout << "warn: null renderer passed to Text::Init" << std::endl;
                return false;
            }
            m_sdlRenderer = renderer->GetSDLRenderer();

            // Initialize TTF if not already done
            if (!TTF_WasInit()) {
                if (TTF_Init() == -1) {
                    std::cout << "warn: failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
                    return false;
                }
            }

            m_font = TTF_OpenFont(fontPath.c_str(), fontSize);
            if (!m_font) {
                std::cout << "warn: failed to load font: " << fontPath << " - " << TTF_GetError() << std::endl;
                return false;
            }

            return true;
        }

        // Set the text content
        void SetText(const std::string& text) {
            if (m_text != text) {
                m_text = text;
                m_dirty = true;
            }
        }

        // Set the text color
        void SetColor(const Color& color) {
            if (m_color.r != color.r || m_color.g != color.g ||
                m_color.b != color.b || m_color.a != color.a) {
                m_color = color;
                m_dirty = true;
            }
        }

        void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
            SetColor(Color(r, g, b, a));
        }

        // Get text dimensions
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        Vector2<int> GetSize() const { return Vector2<int>(m_width, m_height); }

        // Get text content
        const std::string& GetText() const { return m_text; }

        // Check if font is loaded
        bool IsLoaded() const { return m_font != nullptr; }

        // Draw at world position (uses camera)
        void Draw(Renderer* renderer, const Vector2<float>& worldPos) {
            if (!renderer) return;
            UpdateTexture();
            if (!m_texture) return;

            renderer->DrawSprite(m_texture, nullptr, worldPos, m_width, m_height);
        }

        void Draw(Renderer* renderer, float worldX, float worldY) {
            Draw(renderer, Vector2<float>(worldX, worldY));
        }

        // Draw at screen position (ignores camera)
        void DrawScreen(Renderer* renderer, int x, int y) {
            if (!renderer) return;
            UpdateTexture();
            if (!m_texture) return;

            renderer->DrawSpriteScreen(m_texture, nullptr, x, y, m_width, m_height);
        }

        // Draw centered at world position
        void DrawCentered(Renderer* renderer, const Vector2<float>& worldPos) {
            Draw(renderer, Vector2<float>(
                worldPos.GetX() - m_width / 2.0f,
                worldPos.GetY() - m_height / 2.0f
            ));
        }

        // Draw centered at screen position
        void DrawCenteredScreen(Renderer* renderer, int x, int y) {
            DrawScreen(renderer, x - m_width / 2, y - m_height / 2);
        }

        // Free resources
        void Free() {
            if (m_texture) {
                SDL_DestroyTexture(m_texture);
                m_texture = nullptr;
            }
            if (m_font) {
                TTF_CloseFont(m_font);
                m_font = nullptr;
            }
            m_width = 0;
            m_height = 0;
        }

    private:
        void UpdateTexture() {
            if (!m_dirty || !m_font || !m_sdlRenderer) return;
            if (m_text.empty()) {
                if (m_texture) {
                    SDL_DestroyTexture(m_texture);
                    m_texture = nullptr;
                }
                m_width = 0;
                m_height = 0;
                m_dirty = false;
                return;
            }

            // Free old texture
            if (m_texture) {
                SDL_DestroyTexture(m_texture);
                m_texture = nullptr;
            }

            // Render text to surface
            SDL_Color sdlColor = { m_color.r, m_color.g, m_color.b, m_color.a };
            SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_text.c_str(), sdlColor);
            if (!surface) {
                std::cout << "warn: failed to render text: " << TTF_GetError() << std::endl;
                return;
            }

            // Create texture from surface
            m_texture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
            m_width = surface->w;
            m_height = surface->h;
            SDL_FreeSurface(surface);

            if (!m_texture) {
                std::cout << "warn: failed to create text texture: " << SDL_GetError() << std::endl;
                m_width = 0;
                m_height = 0;
            }

            m_dirty = false;
        }
    };
}
#endif
