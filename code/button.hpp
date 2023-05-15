#pragma once

#include <vector>

#include "texture.hpp"

const string ClickLink = "source/music/sfx/click.wav";

class Button{
public:
    bool insideRect(int x, int y){
        return rect.x <= x && x <= rect.x + rect.w
            && rect.y <= y && y <= rect.y + rect.h;
    }

public:
    Button();
    Button(SDL_Rect _rect, string path, string text = "");
    ~Button();

    void draw(int drawGlow = 1);
    void addText(string text, int _sz, Uint32 Color, int distH = -1);
    void glow();
    void free();
    bool click(int x, int y);

private:
    Texture image, Text[10];
    vector<int> dis;
    SDL_Rect rect;
    SFX clicked;
};

class Box{
private:
    bool insideRect(int pos, int x, int y){
        SDL_Rect o = vRect[pos];
        return rect.x + o.x <= x && x <= rect.x + o.x + o.w
            && rect.y + o.y <= y && y <= rect.y + o.y + o.h;
    }

public:
    Box();
    Box(SDL_Rect _rect, string path);
    ~Box();

    void setRect(const vector<SDL_Rect>& v);
    void draw(bool drawGlow = 1);
    void glow();
    void free();
    int click(int x, int y);

private:
    Texture image;
    SDL_Rect rect;
    vector<SDL_Rect> vRect;
    SFX clicked;
};
