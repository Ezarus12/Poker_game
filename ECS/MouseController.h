#pragma once

#include "../game.h"
#include "ECS.h"
#include "Components.h"

class MouseController : public Component {
public:
	SpriteComponent* sprite;
	int x, y;
	bool clicked = false;
	void init() override
	{
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(&x, &y);
			x /= r_scale;
			y /= r_scale;
			if (x >= sprite->x() && x <= (sprite->x() + sprite->w()) && y >= sprite->y() && y <= (sprite->y() + sprite->h())) { //checking if the coursor location is within the sprite area
				cout << x << "  " << y << endl;
				clicked = true;
			}
			

		}
	}




};