#ifndef SPRITE_H
#define SPRITE_H
#include <SDL2/SDL.h>
#include <string>
#include "engine/Vector2.hpp"
#include "engine/Renderer.hpp"
#include "engine/TextureLoader.hpp"

namespace Engine {
    class Sprite {
    private:
        SDL_Texture* m_texture = nullptr;
        Vector2i m_spriteSize;           // size of each sprite frame in pixels
        Vector2i m_sheetSize;            // size of sprite sheet in columns and rows
        Vector2i m_currentFrame;         // current column/row in sprite sheet
        SDL_RendererFlip m_flip = SDL_FLIP_NONE;

    public:
        Sprite() = default;

        // Single sprite (not a sheet)
        Sprite(SDL_Texture* texture, int width, int height)
            : m_texture(texture), m_spriteSize(width, height), m_sheetSize(1, 1), m_currentFrame(0, 0) {}

        // Sprite sheet
        Sprite(SDL_Texture* texture, int frameWidth, int frameHeight, int columns, int rows)
            : m_texture(texture), m_spriteSize(frameWidth, frameHeight),
              m_sheetSize(columns, rows), m_currentFrame(0, 0) {}

        // Load from file (single sprite)
        bool Load(SDL_Renderer* renderer, const std::string& path, int width, int height) {
            m_texture = TextureLoader::Load(renderer, path);
            if (!m_texture) return false;
            m_spriteSize.Set(width, height);
            m_sheetSize.Set(1, 1);
            m_currentFrame.Set(0, 0);
            return true;
        }

        // Load from file (sprite sheet)
        bool Load(SDL_Renderer* renderer, const std::string& path,
                  int frameWidth, int frameHeight, int columns, int rows) {
            m_texture = TextureLoader::Load(renderer, path);
            if (!m_texture) return false;
            m_spriteSize.Set(frameWidth, frameHeight);
            m_sheetSize.Set(columns, rows);
            m_currentFrame.Set(0, 0);
            return true;
        }

        // Set current frame by column/row
        void SetFrame(int column, int row) {
            m_currentFrame.Set(column % m_sheetSize.GetX(), row % m_sheetSize.GetY());
        }

        // Set current frame by index (left-to-right, top-to-bottom)
        void SetFrameIndex(int index) {
            int col = index % m_sheetSize.GetX();
            int row = index / m_sheetSize.GetX();
            SetFrame(col, row);
        }

        // Get total number of frames
        int GetFrameCount() const {
            return m_sheetSize.GetX() * m_sheetSize.GetY();
        }

        // Get current frame index
        int GetFrameIndex() const {
            return m_currentFrame.GetY() * m_sheetSize.GetX() + m_currentFrame.GetX();
        }

        // Advance to next frame (wraps around)
        void NextFrame() {
            SetFrameIndex((GetFrameIndex() + 1) % GetFrameCount());
        }

        // Flip controls
        void SetFlipHorizontal(bool flip) {
            if (flip) m_flip = static_cast<SDL_RendererFlip>(m_flip | SDL_FLIP_HORIZONTAL);
            else m_flip = static_cast<SDL_RendererFlip>(m_flip & ~SDL_FLIP_HORIZONTAL);
        }

        void SetFlipVertical(bool flip) {
            if (flip) m_flip = static_cast<SDL_RendererFlip>(m_flip | SDL_FLIP_VERTICAL);
            else m_flip = static_cast<SDL_RendererFlip>(m_flip & ~SDL_FLIP_VERTICAL);
        }

        bool IsFlippedHorizontal() const { return m_flip & SDL_FLIP_HORIZONTAL; }
        bool IsFlippedVertical() const { return m_flip & SDL_FLIP_VERTICAL; }

        // Get source rectangle for current frame
        SDL_Rect GetSourceRect() const {
            return {
                m_currentFrame.GetX() * m_spriteSize.GetX(),
                m_currentFrame.GetY() * m_spriteSize.GetY(),
                m_spriteSize.GetX(),
                m_spriteSize.GetY()
            };
        }

        // Draw at world position (uses sprite's native size)
        void Draw(Renderer& renderer, const Vector2f& position) {
            SDL_Rect srcRect = GetSourceRect();
            if (m_flip == SDL_FLIP_NONE) {
                renderer.DrawSprite(m_texture, &srcRect, position,
                                   m_spriteSize.GetX(), m_spriteSize.GetY());
            } else {
                renderer.DrawSpriteEx(m_texture, &srcRect, position,
                                     m_spriteSize.GetX(), m_spriteSize.GetY(), 0.0, m_flip);
            }
        }

        // Draw at world position with custom size
        void Draw(Renderer& renderer, const Vector2f& position, int width, int height) {
            SDL_Rect srcRect = GetSourceRect();
            if (m_flip == SDL_FLIP_NONE) {
                renderer.DrawSprite(m_texture, &srcRect, position, width, height);
            } else {
                renderer.DrawSpriteEx(m_texture, &srcRect, position, width, height, 0.0, m_flip);
            }
        }

        // Draw with rotation
        void Draw(Renderer& renderer, const Vector2f& position, double angle) {
            SDL_Rect srcRect = GetSourceRect();
            renderer.DrawSpriteEx(m_texture, &srcRect, position,
                                 m_spriteSize.GetX(), m_spriteSize.GetY(), angle, m_flip);
        }

        // Getters
        SDL_Texture* GetTexture() const { return m_texture; }
        Vector2i GetSpriteSize() const { return m_spriteSize; }
        Vector2i GetSheetSize() const { return m_sheetSize; }
        Vector2i GetCurrentFrame() const { return m_currentFrame; }

        // Set texture (for sharing textures between sprites)
        void SetTexture(SDL_Texture* texture) { m_texture = texture; }

        // Check if texture is loaded
        bool IsLoaded() const { return m_texture != nullptr; }
    };
}
#endif
