#include "Variables.h"
#include "game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "ECS/Components.h"
#include "Cards/Components.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Texture* background;

Manager manager;
auto& cowboy(manager.addEntity());

auto& card1(manager.addEntity());
auto& card2(manager.addEntity());
auto& card3(manager.addEntity());
auto& card4(manager.addEntity());
auto& card5(manager.addEntity());

auto& hand_card1(manager.addEntity());
auto& hand_card2(manager.addEntity());

auto& back_card(manager.addEntity());

vector<Card> table;

vector<Hand> players; //vector storing hands for all of the player and the enemies (main player == player[0]);

vector<vector<Card>> hands; //vector storing vectors for each player containg hand's cards and table

vector<int> scores;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{

	//Initializing subsystem, window and renderer
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
	}

	//Create Deck
	vector<Card> Deck;
	fill_deck(Deck);

	//Draw hands
	for (int i = 0; i < players_num; i++) {
		players.push_back(draw(Deck));
	}

	//Create table
	for (int i = 0; i < 5; i++) {
		uniform_int_distribution<int> distribution(0, Deck.size() - 1);
		int r = distribution(gen);
		table.push_back(Deck[r]);
		Deck.erase(Deck.begin() + r);
	}


	//ECS 

	hand_card1.addComponent<PositionComponent>(80, 132 + 16);
	hand_card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, players[0].c1.get_suit_int(), players[0].c1.get_rank() - 2);
	hand_card2.addComponent<PositionComponent>(112, 132 + 16);
	hand_card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, players[0].c2.get_suit_int(), players[0].c2.get_rank() - 2);

	back_card.addComponent<PositionComponent>(14, card_y);
	back_card.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, 0, 13);

	cowboy.addComponent<PositionComponent>(60, 0);
	cowboy.addComponent<SpriteComponent>("assets/cowboy.png", 60, 60);
	cowboy.addComponent<MouseController>();
}

void Game::handleEvents()
{
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

int cnt = 0;

void Game::update()
{
	manager.update();

	cnt++;
	//Showing deck on screen
	if (cnt == 1200) {
		card1.addComponent<PositionComponent>(card_x, card_y);
		card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[0].get_suit_int(), table[0].get_rank() - 2);

		card2.addComponent<PositionComponent>(card_x + card_spacing * 1, card_y);
		card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[1].get_suit_int(), table[1].get_rank() - 2);

		card3.addComponent<PositionComponent>(card_x + card_spacing * 2, card_y);
		card3.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[2].get_suit_int(), table[2].get_rank() - 2);

		ignoreHands(4);
	}
	if (cnt >= 1200 && Game::event.type == SDL_MOUSEBUTTONDOWN) {
		card4.addComponent<PositionComponent>(card_x + card_spacing * 3, card_y);
		card4.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[3].get_suit_int(), table[3].get_rank() - 2);
	}
	if (cnt >= 1500 && Game::event.type == SDL_MOUSEBUTTONDOWN) {
		card5.addComponent<PositionComponent>(card_x + card_spacing * 4, card_y);
		card5.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[4].get_suit_int(), table[4].get_rank() - 2);
	}
	if (cnt >= 3000 && Game::event.type == SDL_MOUSEBUTTONDOWN) {
		for (int i = 0; i < players_num; i++) {
			hands.push_back(combine(table, players, i));
			for (int j = 0; j < 7; j++) {
				cout << hands[i][j] << endl;;
			}
			cout << "Next player: " << endl;
		}
		for (int i = 0; i < players_num; i++) {
			scores.push_back(win(hands[i]));
			score(scores[i]);
		}
		if (scores[0] < scores[1]) {
			cout << "Player Won!" << endl;
		}
		else if (scores[0] > scores[1]) {
			cout << "Enemy Won!" << endl;
		}
		else  {
			cout << "Draw" << endl;
			//Compare(scores[0]);
			ignoreHands(scores[0]);
			for (int i = 0; i < players_num; i++) {
				scores[i] = (win(hands[i]));
				score(scores[i]);
			}
			if (scores[0] < scores[1]) {
				cout << "Player Won!" << endl;
			}
			else if (scores[0] > scores[1]) {
				cout << "Enemy Won!" << endl;
			}
		}
	}
	
	
	

	
	//SPRITE DRAGGING

	if (cowboy.getComponent<MouseController>().clicked) {
		SDL_GetMouseState(&x, &y);
		cowboy.getComponent<PositionComponent>().x(x/r_scale- 30);
		cowboy.getComponent<PositionComponent>().y(y / r_scale - 30);
		if (event.type == SDL_MOUSEBUTTONUP) {
			cowboy.getComponent<MouseController>().clicked = false;
		}	
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	//SDL_RenderCopy(renderer, background, NULL, 0);
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
