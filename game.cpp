#include "game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "ECS/Components.h"


GameObject* cowboy;

bool card_hovered;
int r_scale;
int x, y = 0;
SDL_Texture* background;

Manager manager;
auto& newplayer(manager.addEntity());

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
	background = TextureManager::LoadTexture("assets/back.png", renderer);
	cowboy = new GameObject("assets/cowboy.png", renderer, 60, 0, 60, 59);

	newplayer.addComponent<PositionComponent>();
	newplayer.getComponent<PositionComponent>().setPos(30,30);
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
	cowboy->Update();
	manager.update();
	std::cout << newplayer.getComponent<PositionComponent>().x() << endl;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, 0);
	cowboy->Render();
	SDL_RenderPresent(renderer);
}


void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game cleaned" << endl;
}
