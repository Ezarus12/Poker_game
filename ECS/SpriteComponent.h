#pragma once

#include "Components.h"
#include "SDL.h"

class SpriteComponent : public Component
{
private:
	PositionComponent* position;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	int width;
	int height;

public:
	SpriteComponent() = default;
	SpriteComponent(const char* path, int w, int h)
	{
		setTex(path);
		width = w;
		height = h;
	}

	void setTex(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{
		position = &entity->getComponent<PositionComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = srcRect.h = 32;
		destRect.w = width;
		destRect.h = height;
		
	}

	void update() override
	{
		destRect.x = position->x();
		destRect.y = position->y();
	}

	void draw() override
	{
		TextureManager::Draw(texture, destRect);
	}

};