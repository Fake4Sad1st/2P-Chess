#pragma once

#include <SDL.h>
#include <SDL_image.h>

class Texture {
public:
    ~Texture();
    Texture(SDL_Renderer* renderer, const std::string& filename);

    void draw(int x, int y);

private:
    SDL_Texture* texture = NULL;
    SDL_Renderer* m_renderer;
    int width;
    int height;
};
