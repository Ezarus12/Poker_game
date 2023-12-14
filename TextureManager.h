#ifndef TEXTUREM_H
#define TEXTUREM_H

#include "game.h"

class TextureManager {

public:
	static SDL_Texture* LoadTexture(const char* file, SDL_Renderer* ren);
};

#endif