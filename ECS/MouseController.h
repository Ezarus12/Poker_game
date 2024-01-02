#pragma once

#include "../game.h"
#include "ECS.h"
#include "Components.h"

class MouseController : public Component {
public:
	SpriteComponent* sprite;
	int x, y;
	bool down = false;
	bool hovered = false;

	bool hover = false;
	void init() override
	{
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void setHover() {
		hover = true;
	}

	void update() override
	{
		SDL_GetMouseState(&x, &y);
		x /= r_scale;
		y /= r_scale;
		if (Game::event.type == SDL_MOUSEBUTTONDOWN) { //clicked
			if (x >= sprite->x() && x <= (sprite->x() + sprite->w()) && y >= sprite->y() && y <= (sprite->y() + sprite->h())) { //checking if the coursor location is within the sprite area
				down = true;
			}
		}
		if (hover && x >= sprite->x() && x <= (sprite->x() + sprite->w()) && y >= sprite->y() && y <= (sprite->y() + sprite->h())) { //checking if the coursor location is within the sprite area
			hovered = true;
		}
		else {
			hovered = false;
		}
	}
};