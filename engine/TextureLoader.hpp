#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

namespace Engine {
    class TextureLoader {
    public:
        // Load a texture from file (PNG, JPG, etc.)
        // Returns nullptr on failure
        static SDL_Texture* Load(SDL_Renderer* renderer, const std::string& path) {
            SDL_Surface* surface = IMG_Load(path.c_str());
            if (!surface) {
                SDL_Log("Failed to load image '%s': %s", path.c_str(), IMG_GetError());
                return nullptr;
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (!texture) {
                SDL_Log("Failed to create texture from '%s': %s", path.c_str(), SDL_GetError());
                return nullptr;
            }

            return texture;
        }

        // Load with color key (transparent color)
        static SDL_Texture* LoadWithColorKey(SDL_Renderer* renderer, const std::string& path,
                                              Uint8 r, Uint8 g, Uint8 b) {
            SDL_Surface* surface = IMG_Load(path.c_str());
            if (!surface) {
                SDL_Log("Failed to load image '%s': %s", path.c_str(), IMG_GetError());
                return nullptr;
            }

            SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (!texture) {
                SDL_Log("Failed to create texture from '%s': %s", path.c_str(), SDL_GetError());
                return nullptr;
            }

            return texture;
        }

        // Get texture dimensions
        static bool GetSize(SDL_Texture* texture, int* width, int* height) {
            return SDL_QueryTexture(texture, nullptr, nullptr, width, height) == 0;
        }

        // Initialize SDL_image (call once at startup)
        static bool Init(int flags = IMG_INIT_PNG) {
            if (!(IMG_Init(flags) & flags)) {
                SDL_Log("SDL_image init failed: %s", IMG_GetError());
                return false;
            }
            return true;
        }

        // Cleanup SDL_image (call at shutdown)
        static void Quit() {
            IMG_Quit();
        }
    };
}
#endif
