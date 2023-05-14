#pragma once

#include "button.hpp"

//open screen image
const string StartScreenLink = "img/background/start_screen.png";

//song to play
const string Theme1Link = "music/song/pigstep.mp3";

class OpenScreen{
public:
    OpenScreen();
    ~OpenScreen();
    void init();
    void finish();
    void mainEvent();
    void move();
    void draw();
    void checkClick(int x, int y);
    void drawButton();

private:
    Texture screen;
    Music theme;
    Button start, setting, exit;
    Box settingBox;
    Button sfx0, audio0;
    bool quit, state;


    Button temp;
};
