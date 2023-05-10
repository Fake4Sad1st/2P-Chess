#include "texture.hpp"

Texture::Texture(){
    texture = NULL;
}

Texture::Texture(const std::string& filename)
{
    SDL_Surface* loadedSurface = IMG_Load( filename.c_str() );
    if( loadedSurface == NULL ){
		std::cerr << "Unable to load image " << filename << "! SDL Error: " << IMG_GetError() <<'\n';
        throw;
	}

	texture = SDL_CreateTextureFromSurface( Basic::instance().m_renderer, loadedSurface );
    if( texture == NULL ){
        std::cerr << "Unable to create texture from " << filename << "! SDL Error: " << IMG_GetError() <<'\n';
        throw;
    }
    width = loadedSurface->w;
    height = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
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

void Texture::free() {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}
