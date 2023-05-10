#pragma once

#include "basic.hpp"

class Texture {
public:
    Texture();
    Texture(const std::string& filename);
    ~Texture();

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other){
        free();
        texture = other.texture;
        width = other.width;
        height = other.height;

        other.texture = nullptr;

        return *this;
    }

    void setAlpha(Uint8 a);
    void draw(SDL_Rect rect);
    void draw(int x, int y);
    void free();

private:
    SDL_Texture* texture;
    int width;
    int height;
};
