#pragma once

#include <SDL.h>

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

