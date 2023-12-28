#pragma once

#include "Components.h"
#include <string>

class TextComponent : public Component {
private:
	SDL_Texture* text_texture = nullptr;
	mutable SDL_Rect text_rect;
	PositionComponent* position;

	std::string font_path_;
	int font_size_;
	std::string message_;
	SDL_Color color_;

	int* num;
	bool isNum = false;

public:
	TextComponent(const std::string& font_path, int font_size, const std::string& message, const SDL_Color& color){
		font_path_ = font_path;
		font_size_ = font_size;
		message_ = message;
		color_ = color;


		text_texture = loadFont(font_path, font_size, message, color);
		SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
	};

	SDL_Texture* loadFont(const std::string& font_path, int font_size, const std::string& message, const SDL_Color& color) {
		message_ = message;
		TTF_Font* font = TTF_OpenFont(font_path.c_str(), font_size);
		if (!font) {
			cout << "Failed to load a font" << endl;
		}
		auto text_surface = TTF_RenderText_Solid(font, message.c_str(), color);
		if (!text_surface) {
			cout << "Failed to create text surface" << endl;
		}
		auto text_texture = SDL_CreateTextureFromSurface(Game::renderer, text_surface);
		if (!text_texture) {
			cout << "Failed to create text texture" << endl;
		}
		TTF_CloseFont(font);
		SDL_FreeSurface(text_surface);
		return text_texture;
	}

	void setNum(int* c) {
		num = c;
		isNum = true;
	}

	void init() override
	{
		position = &entity->getComponent<PositionComponent>();
	}

	void draw() override
	{
		text_rect.x = position->x();
		text_rect.y = position->y();
		if (*num != std::stoi(message_)) {
			text_texture = loadFont(font_path_, font_size_, std::to_string(*num), color_);
			SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
		}
		SDL_RenderCopy(Game::renderer, text_texture, nullptr, &text_rect);
	}
};