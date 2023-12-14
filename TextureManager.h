#ifndef TEXTUREM_H
#define TEXTUREM_H

#include "game.h"

class TextureManager : protected Game{

public:
	static SDL_Texture* LoadTexture(const char* file);

	static void Draw(SDL_Texture* tex, SDL_Rect dest);
};

#endif