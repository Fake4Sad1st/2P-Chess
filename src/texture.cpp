#include "texture.hpp"

Texture::Texture(){
    texture = NULL;
}

Texture::Texture(const std::string& filename)
{
    SDL_Surface* loadedSurface = IMG_Load( filename.c_str() );
    if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", filename.c_str(), IMG_GetError() );
        throw;
	}

	texture = SDL_CreateTextureFromSurface( Game::instance().m_Renderer, loadedSurface );
    if( texture == NULL )
    {
        printf( "Unable to create texture from %s! SDL Error: %s\n", filename.c_str(), SDL_GetError() );
        throw;
    }
    width = loadedSurface->w;
    height = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
}

Texture::~Texture(){
    freeTexture();
}

void Texture::setAlpha (Uint8 a){
    SDL_SetTextureAlphaMod(texture, a);
}

void Texture::draw(SDL_Rect& rect){
    SDL_RenderCopy(Game::instance().m_Renderer, texture, NULL, &rect);
}

void Texture::draw(int x, int y){
    SDL_Rect rect{x, y, width, height};
    SDL_RenderCopy(Game::instance().m_Renderer, texture, NULL, &rect);
}

void Texture::freeTexture() {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}
