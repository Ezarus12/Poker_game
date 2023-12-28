#pragma once

#include "game.h"

class Text {
public:
	Text(SDL_Renderer* r, const std::string& font_path, int font_size, const std::string& message, const SDL_Color& color) {
		text_texture = loadFont(r, font_path, font_size, message, color);
		SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
	};

	void draw(int x, int y, SDL_Renderer* r) const {
		text_rect.x = x;
		text_rect.y = y;
		SDL_RenderCopy(r, text_texture, nullptr, &text_rect);
	}

	SDL_Texture* loadFont(SDL_Renderer* r, const std::string& font_path, int font_size, const std::string& message, const SDL_Color& color) {
		TTF_Font* font = TTF_OpenFont(font_path.c_str(), font_size);
		if (!font) {
			cout << "failed to load a font" << endl;
		}
		auto text_surface = TTF_RenderText_Solid(font, message.c_str(), color);
		if (!text_surface) {
			cout << "failed to create text surface" << endl;
		}
		auto text_texture = SDL_CreateTextureFromSurface(r, text_surface);
		if (!text_texture) {
			cout << "failed to create text texture" << endl;
		}
		TTF_CloseFont(font);
		SDL_FreeSurface(text_surface);
		return text_texture;
	}
private:
	SDL_Texture* text_texture = nullptr;
	mutable SDL_Rect text_rect;
};