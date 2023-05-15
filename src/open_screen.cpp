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
{
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
            checkButton(x, y);
        }
    }
}

void OpenScreen::draw(){
    //Clear screen
    SDL_RenderClear( Basic::instance().m_renderer );

    screen.draw(0, 0);
    drawButton();

    //Update screen
    SDL_RenderPresent( Basic::instance().m_renderer );
    SDL_Delay(10);
}

void OpenScreen::checkButton(int x, int y){
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
