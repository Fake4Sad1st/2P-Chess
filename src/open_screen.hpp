#pragma once

#include "texture.hpp"

//open screen image
const std::string StartScreenLink = "img/background/start_screen.png";

class OpenScreen{
public:
    OpenScreen();
    ~OpenScreen();
    void init();
    void mainEvent();
    void move();
    void draw();

private:
    Texture screen;
    bool quit;
};
