#include "open_screen.hpp"

OpenScreen::OpenScreen():
    screen(StartScreenLink),
    theme(Theme1Link),
    start(SDL_Rect {291, 250, 430, 120}, "img/start.png"),
    setting(SDL_Rect {291, 390, 430, 120}, "img/settings.png"),
    exit(SDL_Rect {291, 530, 430, 120}, "img/exit.png"),
    settingBox(SDL_Rect {256, 120, 500, 470}, "img/settings_box.png"),
    sfx0(SDL_Rect {327, 199, 350, 120}, "img/sfx_button.png"),
    audio0(SDL_Rect {327, 339, 350, 120}, "img/audio_button.png")

//    start(SDL_Rect {310, 215, 300, 110}, "img/matchSaved_button.png")
//    setting(SDL_Rect {250, 120, 500, 500}, "img/button2.png", "SETTINGS"),
//    exit(SDL_Rect {310, 340, 380, 110}, "img/button.png", "REPLAY"),
//    temp(SDL_Rect {310, 465, 380, 110}, "img/button.png", "EXIT")
{
//    start.addText("Report Saved", 25, 17);
//    start.addText("Successfully!", 25, 55);
    vector<SDL_Rect> v = {SDL_Rect {70, 80, 350, 120},
                          SDL_Rect {70, 220, 350, 120},
                          SDL_Rect {145, 360, 200, 80} };
    settingBox.setRect(v);
}

OpenScreen::~OpenScreen(){}

void OpenScreen::init(){
    quit = false, state = 0;
    theme.play();
}

void OpenScreen::finish(){
    theme.stop();
}

void OpenScreen::mainEvent(){
    init();
    while(!quit){
        draw();
        move();
        if(Basic::instance().askQuit()) return;
    }
    finish();
}

void OpenScreen::move(){
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 ){
        if( e.type == SDL_QUIT ){Basic::instance().rageQuit(); return;}
        if( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ){
            int x = e.motion.x, y = e.motion.y;
            checkClick(x, y);
        }
    }
}

void OpenScreen::draw(){
    //Clear screen
    SDL_RenderClear( Basic::instance().m_renderer );
//    SDL_SetRenderDrawColor( Basic::instance().m_renderer, 0xE8, 0x93, 0x52, 0xFF );

    screen.draw(0, 0);
    drawButton();
//    temp.draw();
//    Basic::instance().addColor(0x00000080, SDL_Rect {310, 215, 300, 110}, 1);

    //Update screen
    SDL_RenderPresent( Basic::instance().m_renderer );
    SDL_Delay(10);
}

void OpenScreen::checkClick(int x, int y){
    if(!state){
        if(start.click(x, y)){SDL_Delay(200); quit = true;}
        if(setting.click(x, y)) state = 1;
        if(exit.click(x, y)){SDL_Delay(200); Basic::instance().rageQuit();}
    }
    else{
        int o = settingBox.click(x, y);
        if(o == 0) Mix_MasterVolume(DEFAULT_SFX_VOL - Mix_MasterVolume(-1));
        if(o == 1) Mix_VolumeMusic(DEFAULT_AUDIO_VOL - Mix_VolumeMusic(-1));
        if(o == 2) state = 0;
    }
}

void OpenScreen::drawButton(){
    start.draw(!state);
    setting.draw(!state);
    exit.draw(!state);
    if(state){
        Basic::instance().blurScreen();
        settingBox.draw();
        if(Mix_MasterVolume(-1) == 0) sfx0.draw();
        if(Mix_VolumeMusic(-1) == 0) audio0.draw();
    }
}
