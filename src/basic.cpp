#include "basic.hpp"

///about Music class
Music::Music(){
    music = NULL;
}

Music::Music(const std::string& filename){
    music = Mix_LoadMUS( filename.c_str() );
    if( music == NULL ){
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << '\n';
        throw;
    }
}

Music::~Music(){
    free();
}

void Music::free() {
    if (music != NULL) {
        Mix_FreeMusic(music);
        music = NULL;
    }
}

void Music::play(){
    Mix_PlayMusic(music, -1);
}

void Music::stop(){
    Mix_HaltMusic();
}

///about SFX class
SFX::SFX(){
    chunk = NULL;
}

SFX::SFX(const std::string& filename){
    chunk = Mix_LoadWAV( filename.c_str() );
    if( chunk == NULL ){
        std::cerr << "Failed to load chunk! SDL_mixer Error: " << Mix_GetError() << '\n';
        throw;
    }
}

SFX::~SFX(){
    free();
}

void SFX::free() {
    if (chunk != NULL) {
        Mix_FreeChunk(chunk);
        chunk = NULL;
    }
}

void SFX::play(){
    Mix_PlayChannel(-1, chunk, 0);
}

///about Basic class
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
        std::cerr << "Unable to load image " << ICON_LINK << "! SDL_image Error: " << IMG_GetError() << '\n';
        throw;
    }
    SDL_SetWindowIcon(m_window, _icon);
    SDL_FreeSurface(_icon);
}

Basic::Basic(){
    //Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ){
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
			std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
			throw;
		}
		//Set icon for program
        setIcon();

        //Create renderer for window
        m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED );
        if( m_renderer == NULL ){
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
            throw;
        }

        //Initialize renderer color
        SDL_SetRenderDrawColor( m_renderer, 0xFF, 0xFF, 0xFF, 0xFF );

        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) ){
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
            throw;
        }

        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << '\n';
            throw;
        }
        Mix_VolumeMusic(32);
	}
}

Basic::~Basic(){
	//Destroy window and renderer
	SDL_DestroyRenderer( m_renderer );
	SDL_DestroyWindow( m_window );

	//Quit SDL subsystems
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}
