#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "game.hpp"

Game* Game::s_game = nullptr;

Game& Game::instance() {
    if (s_game == nullptr) s_game = new Game();
    return *s_game;
}

void Game::free() {
    if (s_game) {
        delete s_game;
        s_game = nullptr;
    }
}

Game::Game(){
    //Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';
		throw;
	}
	else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			std::cerr << "Warning: Linear texture filtering not enabled!\n";
		}

		//Create window
		m_Window = SDL_CreateWindow( EXE_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( m_Window == NULL ){
			std::cerr <<  "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
			throw;
		}
		else{
			//Create renderer for window
			m_Renderer = SDL_CreateRenderer( m_Window, -1, SDL_RENDERER_ACCELERATED );
			if( m_Renderer == NULL ){
				std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				throw;
			}
			else{
				//Initialize renderer color
				SDL_SetRenderDrawColor( m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
                    throw;
				}
			}
		}
	}
}

Game::~Game(){
	//Destroy window
	SDL_DestroyRenderer( m_Renderer );
	SDL_DestroyWindow( m_Window );

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
	TTF_Quit();
}
