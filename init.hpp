#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "texture.hpp"
#include "game.hpp"

using namespace std;

//Image link
const string chessboardLink = "img/chessboard.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string whitePawnLink = "img/piece/WhitePawn.png";
//const string blackBishopLink = "img/piece/BlackBishop.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";
//const string blackPawnLink = "img/piece/BlackPawn.png";


class Chessboard{
public:
    Chessboard(Game& game);
    ~Chessboard();
    void initValue();
    void draw();

private:
    Texture chessboard;
    SDL_Renderer* m_renderer;
    vector< vector<int> > value;

};
