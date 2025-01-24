#ifndef TEXTUREM_H
#define TEXTUREM_H

#include "game.h"

class TextureManager : protected Game {

public:
	static SDL_Texture* LoadTexture(const char* file);

	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};


SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	//Checking if the texture has been loaded
	if (tempSurface == 0) {
		std::cout << "Cannot load texture: " << texture << endl;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}
#endif