#include "Variables.h"
#include "flags.h"
#include "game.h"
#include "TxtManager.h"
#include "ECS/Components.h"
#include "Cards/Components.h"
#include <string>
#include "Player.h"
#include "Sound/SoundEffects.h"
#include "EnemyAI.h"
#include "Wait.h"
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Texture* background;

Flags flags; 

vector<Card> Deck;  //Deck containg all 52 cards

vector<Card> table;  //set of 5 cards on the table

vector<Hand> players; //vector storing hands for all of the player and the enemies (main player == player[0]);

vector<vector<Card>> hands; //vector storing vectors for each player containg hand's cards and table

vector<Score> scores; //vector storing score for each player and corresponding highest card

//Player player;
Enemy enemy;
Player player;

SoundEffects Sound_effects;
#include "ECS/Entities.h"
#include "Sound/Sounds.h"
#include "Animations.h"
#include "UI.h"
#include "Round.h"


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

		if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0) {
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
	Deck.reserve(sizeof(Card)* 52);
	fill_deck(Deck);
	//Draw hands and create score for each of the players
	

	for (int i = 0; i < players_num; i++) {
		players.push_back(draw(Deck));
		Score* s = new Score();
		scores.push_back(*s);
		scores[i].handRank = max(players[i].c1.get_rank(), players[i].c2.get_rank()); //Find highest card in hand
	}

	//Create table
	for (int i = 0; i < 5; i++) {
		uniform_int_distribution<int> distribution(0, Deck.size() - 1);
		int r = distribution(gen);
		table.push_back(Deck[r]);
		Deck.erase(Deck.begin() + r);
	}

	//Init player
	player.InitPlayer(100, &players[0]);

	/*enemy.hand = draw(Deck);
	enemy.money = 100;*/


	enemy.set_maxHandRank(scores[1].handRank);
	enemy.set_money(&money[1]);

	bigblind = 0;
	//bigblind = Random(0, 1);

	InitSounds();

	SDL_ShowCursor(false);

	InitEntities();
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEWHEEL:
			if (PokerRanking.getComponent<SpriteComponent>().get_show()) {
				if (event.wheel.y > 0) // scroll up
				{
					int y = PokerRanking.getComponent<PositionComponent>().y();
					float y2 = ScrollBar.getComponent<PositionComponent>().y();
					if (y < 0) {
						y += 10;
						PokerRanking.getComponent<PositionComponent>().y(y);
					}
					if (y2 > 0) {
						y2 -= 3.6;
						ScrollBar.getComponent<PositionComponent>().y(y2);
					}
				}
				else if (event.wheel.y < 0) // scroll down
				{
					int y = PokerRanking.getComponent<PositionComponent>().y();
					float y2 = ScrollBar.getComponent<PositionComponent>().y();
					if (y >= -450) {
						y -= 10;
						PokerRanking.getComponent<PositionComponent>().y(y);
					}
					if (y2 < 162) {
						y2 += 3.6;
						ScrollBar.getComponent<PositionComponent>().y(y2);
					}

				}
			}
				
		default:
			break;
	}
}

void Game::update(float deltaTime)
{
	manager.update();
	UpdateCursor();

	if (flags.GameEnded) {
		BigBlindToken.getComponent<SpriteComponent>().hidden();
		SmallBlindToken.getComponent<SpriteComponent>().hidden();
		return;
	}

	if (flags.HandleButtons) {
		HandleBetButtons();
	}

	if (flags.StartGame) {
		if (Start_button.getComponent<MouseController>().down) {
			Start_button.getComponent<SpriteComponent>().setTex("assets/Start_button_c.png");
			if (event.type == SDL_MOUSEBUTTONUP) {
				Sound_effects.playMusic("Music", -1);
;				Sound_effects.playSoundEffect("StartButton", 0);
				Start_button.getComponent<MouseController>().down = false;
				Start_button.getComponent<SpriteComponent>().setTex("assets/Start_button.png");
				flags.StartGame = false;
				Start_button.destroy();
			}
		}
		
		return;
	}

	if (flags.NextRound)
	{
		NextRound(deltaTime);
	}

	PokerRankingTable();

	Round(deltaTime);
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
