#pragma once

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define FU(i,a,b) for(int i=(a);i<(b);i++)
#define cout cerr

//Screen dimension constants
const int SCREEN_WIDTH = 1012;
const int SCREEN_HEIGHT = 675;

//EXE name
const std::string EXE_NAME = "2 Player Chess";

//Icon link
const std::string ICON_LINK = "img/icon/2P_Chess.png";

class Basic{
private:
    static Basic* s_basic;
    Basic();
    ~Basic();

public:
    static Basic& instance();
    static void free();
    void rageQuit();
    bool askQuit();
    void setIcon();
    SDL_Renderer* m_renderer = NULL;

private:
    SDL_Window* m_window = NULL;
    bool quitGame = false;
};

