#include "basic.hpp"

///about Music class
Music::Music(){
    music = NULL;
}

Music::Music(const string& filename){
    music = Mix_LoadMUS( filename.c_str() );
    if( music == NULL ){
        cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << '\n';
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

SFX::SFX(const string& filename){
    chunk = Mix_LoadWAV( filename.c_str() );
    if( chunk == NULL ){
        cerr << "Failed to load chunk! SDL_mixer Error: " << Mix_GetError() << '\n';
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

void Basic::addColor(Uint32 val, SDL_Rect X, bool blend){
    if(!blend) SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
    else SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    Uint8 a[4];
    for(int i = 3; i >= 0; i--) a[i] = val & 255, val >>= 8;
    SDL_SetRenderDrawColor(m_renderer, a[0], a[1], a[2], a[3]);
    SDL_RenderFillRect(m_renderer, &X);
}

void Basic::blurScreen(){
    addColor(0x00000080, SDL_Rect {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 1);
}

//Set icon for window
void Basic::setIcon(){
    SDL_Surface* _icon = IMG_Load( ICON_LINK.c_str() );
    if( _icon == NULL )
    {
        cerr << "Unable to load image " << ICON_LINK << "! SDL_image Error: " << IMG_GetError() << '\n';
        throw;
    }
    SDL_SetWindowIcon(m_window, _icon);
    SDL_FreeSurface(_icon);
}

Basic::Basic(){
    //Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ){
		cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';
		throw;
	}
	else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			cerr << "Warning: Linear texture filtering not enabled!\n";
		}

		//Create window
		m_window = SDL_CreateWindow( EXE_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( m_window == NULL ){
			cerr << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
			throw;
		}
		//Set icon for program
        setIcon();

        //Create renderer for window
        m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED );
        if( m_renderer == NULL ){
            cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
            throw;
        }

        //Initialize renderer color
        SDL_SetRenderDrawColor( m_renderer, 0x00, 0x00, 0x00, 0xFF );

        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) ){
            cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
            throw;
        }

        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
            cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << '\n';
            throw;
        }
        Mix_VolumeMusic(DEFAULT_AUDIO_VOL);
        Mix_MasterVolume(DEFAULT_SFX_VOL);

        //Initialize SDL_ttf
        if( TTF_Init() == -1 ){
            cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';
            throw;
        }
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
