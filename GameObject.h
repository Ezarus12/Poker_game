#pragma once
#include "game.h"

class GameObject {
private:
	int xpos;
	int ypos;

	int height;
	int width;

	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;

public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y, int h, int w);
	~GameObject();

	void Update();
	void Render();
};