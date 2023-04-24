#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "texture.hpp"

Texture::Texture(SDL_Renderer* renderer, const std::string& filename)
{
    SDL_Surface* loadedSurface = IMG_Load( filename.c_str() );
    if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", filename.c_str(), IMG_GetError() );
        throw;
	}

	texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( texture == NULL )
    {
        printf( "Unable to create texture from %s! SDL Error: %s\n", filename.c_str(), SDL_GetError() );
        throw;
    }
    width = loadedSurface->w;
    height = loadedSurface->h;
    m_renderer = renderer;
    SDL_FreeSurface(loadedSurface);
}

Texture::~Texture(){
    SDL_DestroyTexture(texture);
}

void Texture::draw(int x, int y){
    SDL_Rect rect{x, y, width, height};
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}
