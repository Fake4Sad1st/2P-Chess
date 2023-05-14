#include "texture.hpp"

Texture::Texture(){
    texture = NULL;
}

Texture::Texture(const string& filename){
    SDL_Surface* loadedSurface = IMG_Load( filename.c_str() );
    if( loadedSurface == NULL ){
		cerr << "Unable to load image " << filename << "! SDL Error: " << IMG_GetError() <<'\n';
        throw;
	}

	texture = SDL_CreateTextureFromSurface( Basic::instance().m_renderer, loadedSurface );
    if( texture == NULL ){
        cerr << "Unable to create texture from " << filename << "! SDL Error: " << IMG_GetError() <<'\n';
        throw;
    }
    width = loadedSurface->w;
    height = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
}

Texture::Texture(TTF_Font* font, string text, Uint32 X){
	//Render text surface
	Uint8 B = X & 255; X >>= 8;
	Uint8 G = X & 255; X >>= 8;
	Uint8 R = X;
	SDL_Color textColor {R, G, B};
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if(textSurface == NULL){
		cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
		throw;
	}

    //Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface( Basic::instance().m_renderer, textSurface );
    if(texture == NULL){
        cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << '\n';
        throw;
    }
    //Get image dimensions
    width = textSurface->w;
    height = textSurface->h;

    //Get rid of old surface
    SDL_FreeSurface( textSurface );
}

Texture::~Texture(){
    free();
}

void Texture::setAlpha (Uint8 a){
    SDL_SetTextureAlphaMod(texture, a);
}

void Texture::draw(SDL_Rect rect){
    SDL_RenderCopy(Basic::instance().m_renderer, texture, NULL, &rect);
}

void Texture::draw(int x, int y){
    SDL_Rect rect{x, y, width, height};
    draw(rect);
}

void Texture::free(){
    if(texture != NULL){
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

int Texture::getW(){return width;}
int Texture::getH(){return height;}
