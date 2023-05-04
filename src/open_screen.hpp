#pragma once

#include "texture.hpp"

//open screen image
const std::string StartScreenLink = "img/background/start_screen.png";

//song to play
const std::string Theme1Link = "music/song/pigstep.mp3";

class OpenScreen{
public:
    OpenScreen();
    ~OpenScreen();
    void init();
    void finish();
    void mainEvent();
    void move();
    void draw();

private:
    Texture screen;
    Music theme;
    bool quit;
};
