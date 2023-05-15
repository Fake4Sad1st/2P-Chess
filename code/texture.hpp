#pragma once

#include "basic.hpp"

class Texture {
public:
    Texture();
    Texture(const string& filename);
    Texture(TTF_Font* font, string text, Uint32 X);
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
    int getW();
    int getH();

private:
    SDL_Texture* texture;
    int width;
    int height;
};
