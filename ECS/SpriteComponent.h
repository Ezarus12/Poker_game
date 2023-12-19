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

	int src_x = 0;
	int src_y = 0;

	int cnt = 0;

public:
	SpriteComponent() = default;
	SpriteComponent(const char* path, int w, int h)
	{
		setTex(path);
		width = w;
		height = h;
	}

	SpriteComponent(const char* path, int w, int h, int src_x_ , int src_y_)
	{
		setTex(path);
		width = w;
		height = h;
		src_x = src_x_;
		src_y = src_y_;
	}

	void setTex(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{
		position = &entity->getComponent<PositionComponent>();

		srcRect.x = width * src_x;
		srcRect.y = height * src_y;
		srcRect.w = width;
		srcRect.h = height;

		destRect.w = width;
		destRect.h = height;
		
	}

	void update() override
	{
		//scrolling through deck
	/*	cnt++;
		if (cnt % 300 == 0) {
			src_x++;
			if (src_x == 4) {
				src_x = 0;
				src_y++;
			}
		}*/
		srcRect.y = height * src_y;
		srcRect.x = width * src_x;

		destRect.x = position->x();
		destRect.y = position->y();
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect);
	}

	int x() { return position->x(); }
	int y() { return position->y(); }
	int w() { return width; }
	int h() { return height; }

};