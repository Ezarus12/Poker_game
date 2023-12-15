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

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:
	SpriteComponent() = default;
	SpriteComponent(const char* path, int w, int h)
	{
		setTex(path);
		width = w;
		height = h;
	}

	SpriteComponent(const char* path, int w, int h, int frames_, int speed_)
	{
		animated = true;
		frames = frames_;
		speed = speed_;
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

		/*if (animated) {
			srcRect.x = width * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}*/

		destRect.x = position->x();
		destRect.y = position->y();
	}

	void draw() override
	{
		TextureManager::Draw(texture, destRect);
	}

	int x() { return position->x(); }
	int y() { return position->y(); }
	int w() { return width; }
	int h() { return height; }

};