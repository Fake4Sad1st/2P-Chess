#include "basic.hpp"

Basic* Basic::s_basic = nullptr;

Basic& Basic::instance() {
    if (s_basic == nullptr) s_basic = new Basic();
    return *s_basic;
}

void Basic::free() {
    if (s_basic) {
        delete s_basic;
        s_basic = nullptr;
    }
}

//Quit the overall game
void Basic::rageQuit(){
    quitGame = true;
}

bool Basic::askQuit(){
    return quitGame;
}

//Set icon for window
void Basic::setIcon(){
    SDL_Surface* _icon = IMG_Load( ICON_LINK.c_str() );
    if( _icon == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", ICON_LINK.c_str(), IMG_GetError() );
        throw;
    }
    SDL_SetWindowIcon(m_window, _icon);
    SDL_FreeSurface(_icon);
}

Basic::Basic(){
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
		m_window = SDL_CreateWindow( EXE_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( m_window == NULL ){
			std::cerr <<  "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
			throw;
		}
		else{
            setIcon();

			//Create renderer for window
			m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED );
			if( m_renderer == NULL ){
				std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				throw;
			}
			else{
				//Initialize renderer color
				SDL_SetRenderDrawColor( m_renderer, 0xFF, 0xFF, 0xFF, 0xFF );

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

Basic::~Basic(){
	//Destroy window
	SDL_DestroyRenderer( m_renderer );
	SDL_DestroyWindow( m_window );

	//Quit SDL subsystems
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}
