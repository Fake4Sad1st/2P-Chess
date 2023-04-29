#pragma once

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define FU(i,a,b) for(int i=(a);i<(b);i++)
#define cout cerr

//Screen dimension constants
const int SCREEN_WIDTH = 672;
const int SCREEN_HEIGHT = 678;

// EXE name
const std::string EXE_NAME = "Chess.com";


class Game{
private:
    static Game* s_game;
    Game();
    ~Game();
public:
    static Game& instance();
    static void free();
    SDL_Renderer* m_Renderer = NULL;

private:
    SDL_Window* m_Window = NULL;

};

