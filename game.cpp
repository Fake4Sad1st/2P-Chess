#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "game.hpp"

//laRenderer::laRenderer(){
//    //Create renderer for window
//    xRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
//    if( xRenderer == NULL ){
//        cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
//        throw;
//    }
//    else{
//        //Initialize renderer color
//        SDL_SetRenderDrawColor( xRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
//    }
//}
//
//laRenderer::~laRenderer(){
//	//Destroy window
//	SDL_DestroyRenderer( xRenderer );
//	xRenderer = NULL;
//}

Game::Game(){
    //Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		throw;
	}
	else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gameWindow = SDL_CreateWindow( EXE_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gameWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			throw;
		}
		else{
			//Create renderer for window
			gameRenderer = SDL_CreateRenderer( gameWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gameRenderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				throw;
			}
			else{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    throw;
				}
			}
		}
	}

}

Game::~Game(){
	//Destroy window
	SDL_DestroyRenderer( gameRenderer );
	SDL_DestroyWindow( gameWindow );

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
	TTF_Quit();
}
