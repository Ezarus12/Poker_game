#pragma once

#include "Components.h"

class AnimationComponent : public Component
{
private:
	SpriteComponent* sprite;
	int ImgWidth;
	int ImgHeight;
	int SpriteFrames;
	int SpriteColumns;

	float LoopAnimTimer = 0;
	int LoopSprite = 0;
public:
	AnimationComponent() = default;
	AnimationComponent(int width, int height): ImgWidth(width), ImgHeight(height) {}
	void init() override
	{
		sprite = &entity->getComponent<SpriteComponent>();
		SpriteFrames = ImgWidth / sprite->w();
		SpriteColumns = (ImgHeight / sprite->h()) -1;
	}

	void set_LoopSprite(int i) {
		LoopSprite = i;
	}

	void LoopAnimation(int FPS, float deltaTime) {
		LoopAnimTimer += deltaTime;
		if (LoopAnimTimer >= (1000/FPS)* 0.001) {
			sprite->changeSprite(LoopSprite++, 0);
			LoopAnimTimer = 0; {
				if (LoopSprite >= SpriteFrames) {
					LoopSprite = 0;
				}
			}
		}
	}

	void update() override
	{
	}

	void draw() override
	{
	}
};