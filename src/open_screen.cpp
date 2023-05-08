#include "open_screen.hpp"

OpenScreen::OpenScreen(): screen(StartScreenLink), theme(Theme1Link){}

OpenScreen::~OpenScreen(){}

void OpenScreen::init(){
    quit = false;
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
    while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ){ Basic::instance().rageQuit(); return;}
        if( e.type == SDL_KEYDOWN ){
            if( e.key.keysym.sym == SDLK_q ){quit = true; return;}
            if( e.key.keysym.sym == SDLK_1 ){std::cerr<< Mix_VolumeMusic(128-Mix_VolumeMusic(-1)) << '\n';}
            if( e.key.keysym.sym == SDLK_m ){std::cerr<< Mix_VolumeMusic(0) << '\n';}
        }
    }
}

void OpenScreen::draw(){
    //Clear screen
    SDL_RenderClear( Basic::instance().m_renderer );

    screen.draw(0, 0);
//    SDL_SetRenderDrawBlendMode(Basic::instance().m_renderer, SDL_BLENDMODE_BLEND);
//    SDL_SetRenderDrawColor(Basic::instance().m_renderer, 0x00, 0x00, 0x00, 0x8F);
//    SDL_Rect Hehe {0, 0, 1012, 675};
//    SDL_RenderFillRect(Basic::instance().m_renderer, &Hehe);

    //Update screen
    SDL_RenderPresent( Basic::instance().m_renderer );
    SDL_Delay(20);
}
