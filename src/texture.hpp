#pragma once

#include "game.hpp"

class Texture {
public:
    Texture();
    Texture(const std::string& filename);
    ~Texture();

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other){
        freeTexture();
        texture = other.texture;
        width = other.width;
        height = other.height;

        other.texture = nullptr;

        return *this;
    }

    void draw(int x, int y);
    void freeTexture();

private:
    SDL_Texture* texture;
    int width;
    int height;
};
