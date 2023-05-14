#pragma once

#include <iostream>
#include <fstream>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define FU(i,a,b) for(int i=(a);i<(b);i++)
#define cout cerr
#define sz(v) ((int)(v).size())
#define pa pair<int, int>
#define fi first
#define se second

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1012;
const int SCREEN_HEIGHT = 675;

//EXE name
const string EXE_NAME = "2 Player Chess";

//Icon link
const string ICON_LINK = "img/icon/2P_Chess.png";

//Volume
const int DEFAULT_AUDIO_VOL = 40;
const int DEFAULT_SFX_VOL = 120;

//Music when playing
class Music{
public:
    Music();
    Music(const string& filename);
    ~Music();

    Music& operator=(const Music&) = delete;
    Music& operator=(Music&& other){
        free();
        music = other.music;

        other.music = nullptr;

        return *this;
    }

    void free();
    void play();
    void stop();

private:
    Mix_Music *music;
};

//Sound effect
class SFX{
public:
    SFX();
    SFX(const string& filename);
    ~SFX();

    SFX& operator=(const SFX&) = delete;
    SFX& operator=(SFX&& other){
        free();
        chunk = other.chunk;

        other.chunk = nullptr;

        return *this;
    }

    void free();
    void play();

private:
    Mix_Chunk *chunk;
};

//Window and Renderer
class Basic{
private:
    static Basic* s_basic;
    Basic();
    ~Basic();

public:
    static Basic& instance();
    static void free();
    void setIcon();
    void rageQuit();
    bool askQuit();
    void blurScreen();
    void addColor(Uint32 val, SDL_Rect X, bool blend = 0);
    SDL_Renderer* m_renderer = NULL;

private:
    SDL_Window* m_window = NULL;
    bool quitGame = false;
};

