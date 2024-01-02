#include "game.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

Game* game = nullptr;

int main(int argc, char* args[])
{
	const int FPS = 1000;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime = 0;
	float deltaTime = 1.0f / 60.0f;

	game = new Game();

	game->init("Poker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false); // 1 for FullScreen

	while (game->running()) {

		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update(deltaTime);
		game->render();

		
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
		deltaTime = (SDL_GetTicks() - frameStart) / 1000.f;
	}

	game->clean();

	return 0;
}