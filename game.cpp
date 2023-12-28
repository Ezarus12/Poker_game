#include "Variables.h"
#include "game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "ECS/Components.h"
#include "Cards/Components.h"
#include "text.h"
#include <string>

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

auto& enemy_card1(manager.addEntity());
auto& enemy_card2(manager.addEntity());

auto& back_card(manager.addEntity());

//betting button
auto& BB_center(manager.addEntity());
auto& BB_sub_big(manager.addEntity());
auto& BB_sub_small(manager.addEntity());
auto& BB_add_big(manager.addEntity());
auto& BB_add_small(manager.addEntity());

//Start button
auto& Start_button(manager.addEntity());

vector<Card> table;

vector<Hand> players; //vector storing hands for all of the player and the enemies (main player == player[0]);

vector<vector<Card>> hands; //vector storing vectors for each player containg hand's cards and table

vector<Score> scores; //vector storing score for each player and corresponding highest card

int money[2] = {100, 100};

int currentBet[2] = { 0,0 };

bool bigblind[players_num] = { true, false };

int pool = 0;

int bet = 0;

SDL_Color red = { 148,0,0,255 };

auto& money_text(manager.addEntity());
auto& bet_text(manager.addEntity());
auto& pool_text(manager.addEntity());

bool start_game = false;

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
	vector<Card> Deck;
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

	////TEST!!!!!!
	//vector<Card> cards_test = { Card(5,'D'),Card(5,'H'),Card(6,'C'),Card(8,'S'),Card(14,'D'),Card(14,'H'),Card(14,'S') };
	//vector<Score> scores_test;
	//Score* s = new Score();
	//scores_test.push_back(*s);
	//win(cards_test, scores_test[0]);
	//score(scores_test[0]);

	//ECS 

	hand_card1.addComponent<PositionComponent>(62, 148);
	hand_card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, players[0].c1.get_suit_int(), players[0].c1.get_rank() - 2);
	hand_card2.addComponent<PositionComponent>(62 + card_spacing, 148);
	hand_card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, players[0].c2.get_suit_int(), players[0].c2.get_rank() - 2);

	back_card.addComponent<PositionComponent>(12, card_y);
	back_card.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, 0, 13);

	cowboy.addComponent<PositionComponent>(60, 0);
	cowboy.addComponent<SpriteComponent>("assets/cowboy.png", 60, 60);
	cowboy.addComponent<MouseController>();

	enemy_card1.addComponent<PositionComponent>(192, 10);
	enemy_card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, 0, 13);
	enemy_card2.addComponent<PositionComponent>(192 + card_spacing, 10);
	enemy_card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, 0, 13);

	//Bet button 
	BB_sub_big.addComponent<PositionComponent>(132, 152);
	BB_sub_big.addComponent<SpriteComponent>("assets/Bet_button_sub_big.png", 10, 27);
	BB_sub_big.addComponent<MouseController>();

	BB_sub_small.addComponent<PositionComponent>(142, 152);
	BB_sub_small.addComponent<SpriteComponent>("assets/Bet_button_sub_small.png", 10, 27);
	BB_sub_small.addComponent<MouseController>();

	BB_center.addComponent<PositionComponent>(152, 152);
	BB_center.addComponent<SpriteComponent>("assets/Bet_button_center.png", 34, 27);
	BB_center.addComponent<MouseController>();


	BB_add_small.addComponent<PositionComponent>(186, 152);
	BB_add_small.addComponent<SpriteComponent>("assets/Bet_button_add_small.png", 10, 27);
	BB_add_small.addComponent<MouseController>();

	BB_add_big.addComponent<PositionComponent>(196, 152);
	BB_add_big.addComponent<SpriteComponent>("assets/Bet_button_add_big.png", 10, 27);
	BB_add_big.addComponent<MouseController>();

	money_text.addComponent<PositionComponent>(10, 20);
	money_text.addComponent<TextComponent>("assets/font.ttf", 13, std::to_string(0), red);
	money_text.getComponent<TextComponent>().setNum(&money[0]);

	bet_text.addComponent<PositionComponent>(155, 157);
	bet_text.addComponent<TextComponent>("assets/font.ttf", 13, std::to_string(0), red);
	bet_text.getComponent<TextComponent>().setNum(&bet);

	pool_text.addComponent<PositionComponent>(150, 20);
	pool_text.addComponent<TextComponent>("assets/font.ttf", 13, std::to_string(0), red);
	pool_text.getComponent<TextComponent>().setNum(&pool);

	Start_button.addComponent<PositionComponent>(97, 66);
	Start_button.addComponent<SpriteComponent>("assets/Start_button.png", 126, 48);
	Start_button.addComponent<MouseController>();

	cout << "Player money: " << money[0] << " Enemy money: " << money[1] << endl;
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

bool ding = true;
bool ding1 = true;
bool ding2 = true;
bool ding3 = true;
bool ding4 = true;
bool ding5 = true;
bool ding6 = true;
bool ding7 = true;
bool ding8 = true;
bool ding9 = true;
bool ding10 = true;
bool ding11 = true;
bool ding12 = true;
bool ding13 = true;
bool ding14 = true;
bool ding15 = true;
bool ding16 = true;

bool game_ended = false;

void Game::update()
{
	manager.update();
	
	
	if(ding){
		if (Start_button.getComponent<MouseController>().down) {
			Start_button.getComponent<SpriteComponent>().setTex("assets/Start_button_c.png");
			if (event.type == SDL_MOUSEBUTTONUP) {
				Start_button.getComponent<MouseController>().down = false;
				Start_button.getComponent<SpriteComponent>().setTex("assets/Start_button.png");
				start_game = true;
				ding = false;
				Start_button.destroy();

			}
		}
	}
	if (!start_game) {
		return;
	}

	cnt++;

	//Big and small blinds
	if (ding1) {
		if (bigblind[0]) {
			money[0] -= 10;
			currentBet[0] = 10;
			money[1] -= 5;
			currentBet[1] = 5;
			pool += 15;
		}
		ding1 = false;
	}

	//Enemy move
	while (currentBet[0] != currentBet[1]) {
		money[1]--;
		currentBet[1]++;
		pool++;
	}

	if (ding8) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		ding8 = false;
	}
	if (!game_ended) {
		if (BB_center.getComponent<MouseController>().down) {
			BB_center.getComponent<SpriteComponent>().setTex("assets/Bet_button_center_c.png");
			if (event.type == SDL_MOUSEBUTTONUP) {
				BB_center.getComponent<MouseController>().down = false;
				BB_center.getComponent<SpriteComponent>().setTex("assets/Bet_button_center.png");
				pool += bet;
				money[0] -= bet;
				currentBet[0] = bet;
				bet = 0;
				ding2 = false;
				if (!ding4) {
				ding5 = false;
				}
				if (!ding12) {
					ding13 = false;
				}
				if (!ding14) {
					ding15 = false;
				}
			}
		}
	
		if (BB_add_big.getComponent<MouseController>().down) {
			BB_add_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_big_c.png");
			if (event.type == SDL_MOUSEBUTTONUP) {
				BB_add_big.getComponent<MouseController>().down = false;
				BB_add_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_big.png");
				if (bet <= 989) {
					bet += 10;
				}
			}
		}


		if (BB_add_small.getComponent<MouseController>().down) {
			BB_add_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_small_c.png");
			if (event.type == SDL_MOUSEBUTTONUP) {
				BB_add_small.getComponent<MouseController>().down = false;
				BB_add_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_small.png");
				if (bet <= 998) {
					bet += 1;
				}
			}
		}


		if (BB_sub_big.getComponent<MouseController>().down) {
			BB_sub_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_big_c.png");
			if (event.type == SDL_MOUSEBUTTONUP) {
				BB_sub_big.getComponent<MouseController>().down = false;
				BB_sub_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_big.png");
				if (bet >= 10) {
					bet -= 10;
				}
			}
		}


		if (BB_sub_small.getComponent<MouseController>().down) {
			BB_sub_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_small_c.png");
			if (event.type == SDL_MOUSEBUTTONUP) {
				BB_sub_small.getComponent<MouseController>().down = false;
				BB_sub_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_small.png");
				if (bet > 0) {
					bet -= 1;
				}
			}
		}
	}

	//Enemy move
	while (currentBet[0] != currentBet[1]) {
		money[1]--;
		currentBet[1]++;
		pool++;
	}

	if (ding2) {
		return;
	}	

	//Displaying first 3 cards
	if (ding3) {

		currentBet[0] = 0;
		currentBet[1] = 0;
		cout << "Player money: " << money[0] << " Enemy money: " << money[1] << endl;
		card1.addComponent<PositionComponent>(card_x, card_y);
		card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[0].get_suit_int(), table[0].get_rank() - 2);

		card2.addComponent<PositionComponent>(card_x + card_spacing * 1, card_y);
		card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[1].get_suit_int(), table[1].get_rank() - 2);

		card3.addComponent<PositionComponent>(card_x + card_spacing * 2, card_y);
		card3.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[2].get_suit_int(), table[2].get_rank() - 2);

		cout << scores[0].handRank << " Drugi gracz: " << scores[1].handRank << endl;
		ding3 = false;
		ding4 = false;
	}
	if (ding5) {
		return;
	}

	//Enemy move
	while (currentBet[0] != currentBet[1]) {
		cout << "wchodze";
		money[1]--;
		currentBet[1]++;
		pool++;
	}
	ding6 = false;
	
	
	if (ding6)
		return;

	if (ding7) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		cout << "Player money: " << money[0] << " Enemy money: " << money[1] << endl;
		card4.addComponent<PositionComponent>(card_x + card_spacing * 3, card_y);
		card4.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[3].get_suit_int(), table[3].get_rank() - 2);
		ding7 = false;
		ding12 = false;
	}
	if (ding13) {
		return;
	}

	while (currentBet[0] != currentBet[1]) {
		cout << "wchodze";
		money[1]--;
		currentBet[1]++;
		pool++;
	}
	ding9 = false;

	if (ding9)
		return;

	if (ding10) {
		cout << "Player money: " << money[0] << " Enemy money: " << money[1] << endl;
		card5.addComponent<PositionComponent>(card_x + card_spacing * 4, card_y);
		card5.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[4].get_suit_int(), table[4].get_rank() - 2);
		ding10 = false;
		ding14 = false;
	}
	if (ding15)
		return;

	if (ding16) {
		enemy_card1.getComponent<SpriteComponent>().changeSprite(players[1].c1.get_suit_int(), players[1].c1.get_rank() - 2);
		enemy_card2.getComponent<SpriteComponent>().changeSprite(players[1].c2.get_suit_int(), players[1].c2.get_rank() - 2);
		for (int i = 0; i < players_num; i++) {
			hands.push_back(combine(table, players, i));
			for (int j = 0; j < 7; j++) {
				cout << hands[i][j] << endl;;
			}
			cout << "Next player: " << endl;
		}
		Result(scores, hands);
		cout << scores[0].score << "  " << scores[1].score << endl;
		if (scores[0].score < scores[1].score) {
			cout << "Player Won!" << endl;
			money[0] += pool;
		}
		else if (scores[0].score > scores[1].score) {
			cout << "Enemy Won!" << endl;
			money[1] += pool;
		}
		cout << "Player money: " << money[0] << " Enemy money: " << money[1] << endl;
		cout << "Player money: " << money[0] << " Enemy money: " << money[1] << endl;
		ding16 = false;
		game_ended = true;
	}


	
	
	


	//SPRITE DRAGGING

	if (cowboy.getComponent<MouseController>().down) {
		SDL_GetMouseState(&x, &y);
		cowboy.getComponent<PositionComponent>().x(x / r_scale - 30);
		cowboy.getComponent<PositionComponent>().y(y / r_scale - 30);
		if (event.type == SDL_MOUSEBUTTONUP) {
			cowboy.getComponent<MouseController>().down = false;
		}
	}


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
