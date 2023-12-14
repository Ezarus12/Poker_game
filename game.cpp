#include "game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "ECS/Components.h"

SDL_Renderer* Game::renderer = nullptr;

int r_scale;
int x, y = 0;
SDL_Texture* background;

Manager manager;
auto& player(manager.addEntity());
auto& cowboy(manager.addEntity());


Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	r_scale = width / 320;
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "Subsystem Initialised" << endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			cout << "Window created" << endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_RenderSetLogicalSize(renderer, 320, 180);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			cout << "Renderer created" << endl;
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}
	background = TextureManager::LoadTexture("assets/back.png");

	//ECS 

	player.addComponent<PositionComponent>();
	player.addComponent<SpriteComponent>("assets/coin.png", 27, 30);

	cowboy.addComponent<PositionComponent>(60, 0);
	cowboy.addComponent<SpriteComponent>("assets/cowboy.png", 59, 60);
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&x, &y);
			
			//cout << "Clicked";
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x, &y);
		default:
			break;
	}
}

void Game::update()
{
	manager.update();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, 0);
	manager.draw();
	SDL_RenderPresent(renderer);
}


void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game cleaned" << endl;
}
