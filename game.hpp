#pragma once

#include <SDL.h>

//Screen dimension constants
const int SCREEN_WIDTH = 672;
const int SCREEN_HEIGHT = 678;

// EXE name
const std::string EXE_NAME = "Chess.com";

//class laRenderer{
//public:
//    laRenderer();
//    ~laRenderer();
//
//private:
//};


class Game{
public:
    Game();
    ~Game();
//    laRenderer* gameRenderer;
    SDL_Renderer* gameRenderer = NULL;

private:
    SDL_Window* gameWindow = NULL;

};

