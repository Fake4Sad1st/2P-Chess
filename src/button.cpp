#include "button.hpp"

///about Button class
Button::Button(): clicked(ClickLink)
{}

Button::Button(SDL_Rect _rect, string path, string text):
    clicked(ClickLink)
{
    rect = _rect;
    image = Texture(path);
    if(sz(text)){
        TTF_Font* font = NULL;
        font = TTF_OpenFont("font/FreeSans.ttf", 20);
        //421CEF
        Text[0] = Texture(font, text, 0x6666FF);
        dis.push_back((rect.h - Text[0].getH())/2);
    }
}

Button::~Button(){
    free();
}

void Button::free(){
    image.free();
    FU(i, 0, sz(dis)) Text[i].free();
    dis.clear();
}

void Button::draw(int effect){
    image.draw(rect);
    FU(i, 0, sz(dis)){
        Text[i].draw(rect.x + (rect.w - Text[i].getW())/2, rect.y + dis[i]);
    }
    if(effect == 1) glow();
    else if(effect == -1) Basic::instance().addColor(0x00000070, rect, 1);
}

void Button::addText(string text, int _sz, int a){
    TTF_Font* font = TTF_OpenFont("font/FreeSans.ttf", _sz);
    Text[sz(dis)] = Texture(font, text, 0x6666FF);
    dis.push_back(a);
}

void Button::glow(){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(insideRect(x, y))
        Basic::instance().addColor(0xFFFFFF20, rect, 1);
}

bool Button::click(int x, int y){
    if(!insideRect(x, y)) return false;
    clicked.play();
    return true;
}

///about Box class
Box::Box(): clicked(ClickLink){}

Box::Box(SDL_Rect _rect, string path):
    clicked(ClickLink)
{
    rect = _rect;
    image = Texture(path);
}

Box::~Box(){
    free();
}

void Box::setRect(const vector<SDL_Rect>& v){vRect = v;}

void Box::free(){
    image.free();
    vRect.clear();
}

void Box::draw(bool drawGlow){
    image.draw(rect);
    if(drawGlow) glow();
}

void Box::glow(){
    int x, y;
    SDL_GetMouseState(&x, &y);
    FU(i, 0, sz(vRect)) if(insideRect(i, x, y)){
        SDL_Rect o = vRect[i];
        o.x += rect.x, o.y += rect.y;
        Basic::instance().addColor(0xFFFFFF20, o, 1);
//        Basic::instance().addColor(0xFFFFFFFF, o);
        return;
    }
}

int Box::click(int x, int y){
    FU(i, 0, sz(vRect)) if(insideRect(i, x, y)){
        clicked.play();
        return i;
    }
    return -1;
}
