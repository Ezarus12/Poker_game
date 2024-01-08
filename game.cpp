#include "Variables.h"
#include "flags.h"
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

Flags flags;

vector<Card> Deck;  //Deck containg all 52 cards

vector<Card> table;  //set of 5 cards on the table

vector<Hand> players; //vector storing hands for all of the player and the enemies (main player == player[0]);

vector<vector<Card>> hands; //vector storing vectors for each player containg hand's cards and table

vector<Score> scores; //vector storing score for each player and corresponding highest card

int money[2] = {100, 100};

int currentBet[2] = { 0,0 };

int bigblind;

int pool = 0;

int bet = 0;

SDL_Color red = { 148,0,0,255 };



#include "Animations.h"

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

	bigblind = Random(0, 1);

	//ECS 

	SDL_ShowCursor(false);
	Mouse.addComponent<PositionComponent>();
	Mouse.addComponent<SpriteComponent>("assets/Cursor.png", 10, 11);

	hand_card1.addComponent<PositionComponent>(62, 148);
	hand_card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, players[0].c1.get_suit_int(), players[0].c1.get_rank() - 2);
	hand_card1.addComponent<MouseController>();
	hand_card1.getComponent<MouseController>().setHover();


	hand_card2.addComponent<PositionComponent>(62 + card_spacing, 148);
	hand_card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, players[0].c2.get_suit_int(), players[0].c2.get_rank() - 2);
	hand_card2.addComponent<MouseController>();
	hand_card2.getComponent<MouseController>().setHover();


	back_card.addComponent<PositionComponent>(12, card_y);
	back_card.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, 0, 13);


	card1.addComponent<PositionComponent>(card_x, card_y);
	card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[0].get_suit_int(), table[0].get_rank() - 2);
	card1.getComponent<SpriteComponent>().hidden();

	card2.addComponent<PositionComponent>(card_x + card_spacing * 1, card_y);
	card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[1].get_suit_int(), table[1].get_rank() - 2);
	card2.getComponent<SpriteComponent>().hidden();

	card3.addComponent<PositionComponent>(card_x + card_spacing * 2, card_y);
	card3.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[2].get_suit_int(), table[2].get_rank() - 2);
	card3.getComponent<SpriteComponent>().hidden();

	card4.addComponent<PositionComponent>(card_x + card_spacing * 3, card_y);
	card4.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[3].get_suit_int(), table[3].get_rank() - 2);
	card4.getComponent<SpriteComponent>().hidden();

	card5.addComponent<PositionComponent>(card_x + card_spacing * 4, card_y);
	card5.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[4].get_suit_int(), table[4].get_rank() - 2);
	card5.getComponent<SpriteComponent>().hidden();



	cowboy.addComponent<PositionComponent>(60, 0);
	cowboy.addComponent<SpriteComponent>("assets/Cowboy_Sheet.png", 60, 60);
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

	money_text.addComponent<PositionComponent>(16, 7);
	money_text.addComponent<TextComponent>("assets/font.ttf", 11, std::to_string(0), red);
	money_text.getComponent<TextComponent>().setNum(&money[0]);

	money_text.addComponent<PositionComponent>(290, 7);
	money_text.addComponent<TextComponent>("assets/font.ttf", 11, std::to_string(0), red);
	money_text.getComponent<TextComponent>().setNum(&money[1]);

	bet_text.addComponent<PositionComponent>(155, 157);
	bet_text.addComponent<TextComponent>("assets/font.ttf", 13, std::to_string(0), red);
	bet_text.getComponent<TextComponent>().setNum(&bet);

	pool_text.addComponent<PositionComponent>(132, 24);
	pool_text.addComponent<TextComponent>("assets/font.ttf", 11, std::to_string(0), red);
	pool_text.getComponent<TextComponent>().setNum(&pool);

	Start_button.addComponent<PositionComponent>(97, 66);
	Start_button.addComponent<SpriteComponent>("assets/Start_button.png", 126, 48);
	Start_button.addComponent<MouseController>();

	Fold_button.addComponent<PositionComponent>(216, 157);
	Fold_button.addComponent<SpriteComponent>("assets/Fold_button.png", 34, 18);
	Fold_button.addComponent<MouseController>();

	if (bigblind) 
	{
		BigBlind.addComponent<PositionComponent>(160, 90);
		BigBlindNote.addComponent<PositionComponent>(139, 3);
		SmallBlindNote.addComponent<PositionComponent>(190, 136);
		SmallBlind.addComponent<PositionComponent>(160, 90);
	}
	else
	{
		BigBlind.addComponent<PositionComponent>(160, 90);
		BigBlindNote.addComponent<PositionComponent>(200, 136);
		SmallBlindNote.addComponent<PositionComponent>(139, 3);
		SmallBlind.addComponent<PositionComponent>(160, 90);
	}
	
	BigBlind.addComponent<SpriteComponent>("assets/BigBlindSpriteSheet.png", 16, 16);
	BigBlind.addComponent<MouseController>();
	BigBlind.getComponent<MouseController>().setHover();

	
	SmallBlind.addComponent<SpriteComponent>("assets/SmallBlindSpriteSheet.png", 16, 16);
	SmallBlind.addComponent<MouseController>();
	SmallBlind.getComponent<MouseController>().setHover();
	
	BigBlindNote.addComponent<SpriteComponent>("assets/BigBlindNote.png", 42, 12);
	BigBlindNote.getComponent<SpriteComponent>().hidden();

	SmallBlindNote.addComponent<SpriteComponent>("assets/SmallBlindNote.png", 51, 12);
	SmallBlindNote.getComponent<SpriteComponent>().hidden();
	
	PokerRanking.addComponent<PositionComponent>(70, 0);
	PokerRanking.addComponent<SpriteComponent>("assets/Poker_hand_ranking.png", 180, 646);
	PokerRanking.getComponent<SpriteComponent>().hidden();
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
					if (y < 0) {
						y += 10;
						PokerRanking.getComponent<PositionComponent>().y(y);
					}
				}
				else if (event.wheel.y < 0) // scroll down
				{
					int y = PokerRanking.getComponent<PositionComponent>().y();
					if (y >= -450) {
						y -= 10;
						PokerRanking.getComponent<PositionComponent>().y(y);
					}

				}
			}
				
		default:
			break;
	}
}


//WIP
void DrawHands(vector<Card> &Deck) {
	for (int i = 0; i < players_num; i++) {
		players[i] = (draw(Deck));
		scores[i].handRank = max(players[i].c1.get_rank(), players[i].c2.get_rank()); //Find highest card in hand
	}
}

void DrawTable(vector<Card> &Deck) {
	for (int i = 0; i < 5; i++) {
		uniform_int_distribution<int> distribution(0, Deck.size() - 1);
		int r = distribution(gen);
		table[i] = (Deck[r]);
		Deck.erase(Deck.begin() + r);
	}
}

void NextCards(vector<Card>& Deck) {
	Deck.clear();
	fill_deck(Deck);
	DrawHands(Deck);
	DrawTable(Deck);

	hand_card1.getComponent<SpriteComponent>().changeSprite(players[0].c1.get_suit_int(), players[0].c1.get_rank() - 2);
	hand_card2.getComponent<SpriteComponent>().changeSprite(players[0].c2.get_suit_int(), players[0].c2.get_rank() - 2);

	enemy_card1.getComponent<SpriteComponent>().changeSprite(players[1].c1.get_suit_int(), players[1].c1.get_rank() - 2);
	enemy_card2.getComponent<SpriteComponent>().changeSprite(players[1].c2.get_suit_int(), players[1].c2.get_rank() - 2);

	card1.getComponent<SpriteComponent>().changeSprite(table[0].get_suit_int(), table[0].get_rank() - 2);
	card2.getComponent<SpriteComponent>().changeSprite(table[1].get_suit_int(), table[1].get_rank() - 2);
	card3.getComponent<SpriteComponent>().changeSprite(table[2].get_suit_int(), table[2].get_rank() - 2);
	card4.getComponent<SpriteComponent>().changeSprite(table[3].get_suit_int(), table[3].get_rank() - 2);
	card5.getComponent<SpriteComponent>().changeSprite(table[4].get_suit_int(), table[4].get_rank() - 2);

}


int currentBB = 10; //current Big Blind value

int lowestBet;

void HandleBetButtons() {

	//Bet button
	if (BB_center.getComponent<MouseController>().down) {
		BB_center.getComponent<SpriteComponent>().setTex("assets/Bet_button_center_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			BB_center.getComponent<MouseController>().down = false;
			BB_center.getComponent<SpriteComponent>().setTex("assets/Bet_button_center.png");
			pool += bet;
			money[0] -= bet;
			currentBet[0] += bet;
			bet = 0;
			if (flags.firstBet) {
				flags.firstBet = false;
				flags.Show3Cards = true;
				flags.secondBet = true;
			}
			else if (flags.secondBet) {
				flags.secondBet = false;
				flags.Show4Card = true;
				flags.thirdBet = true;
			}
			else if (flags.thirdBet) {
				flags.thirdBet = false;
				flags.Show5Card = true;
				flags.fourthBet = true;
			}
			else if (flags.fourthBet) {
				flags.fourthBet = false;
				flags.endRound = true;
			}
		}
	}

	//Plus 10 button
	if (BB_add_big.getComponent<MouseController>().down) {
		BB_add_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_big_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			BB_add_big.getComponent<MouseController>().down = false;
			BB_add_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_big.png");
			if (bet <= 989) {
				bet += 10;
			}
		}
	}

	//Plus 1 button
	if (BB_add_small.getComponent<MouseController>().down) {
		BB_add_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_small_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			BB_add_small.getComponent<MouseController>().down = false;
			BB_add_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_small.png");
			if (bet <= 998) {
				bet += 1;
			}
		}
	}

	//Minus 10 button
	if (BB_sub_big.getComponent<MouseController>().down) {
		BB_sub_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_big_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			BB_sub_big.getComponent<MouseController>().down = false;
			BB_sub_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_big.png");
			if (bet >= 10) {
				bet -= 10;
			}
		}
	}

	//Minus 1 button
	if (BB_sub_small.getComponent<MouseController>().down) {
		BB_sub_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_small_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			BB_sub_small.getComponent<MouseController>().down = false;
			BB_sub_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_small.png");
			if (bet > lowestBet) {
				bet -= 1;
			}
		}
	}

	//Fold button
	if (Fold_button.getComponent<MouseController>().down) {
		Fold_button.getComponent<SpriteComponent>().setTex("assets/Fold_button_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			Fold_button.getComponent<MouseController>().down = false;
			Fold_button.getComponent<SpriteComponent>().setTex("assets/Fold_button.png");
			flags.GameEnded = true;
			cout << "Player folded";
		}
	}
}

void Blinds(int Blind) {
	switch (Blind) {
		case 0: //Player got the Big Blind
			money[0] -= currentBB;
			currentBet[0] = currentBB;
			money[1] -= currentBB/2;
			currentBet[1] = currentBB/2;
			for (int i = 0; i < players_num; i++) {
				pool += currentBet[i];
			}
			break;
		case 1: //Enemy got the Big Blind
			money[1] -= currentBB;
			currentBet[1] = currentBB;
			money[0] -= currentBB / 2;
			currentBet[0] = currentBB / 2;
			for (int i = 0; i < players_num; i++) {
				pool += currentBet[i];
			}
			bet = currentBet[1] - currentBet[0];
			lowestBet = bet;
			break;
	}

}

void Round(float deltaTime) {
	CowboyAnim(deltaTime);
	HandCardsHover(deltaTime);
	

	//toosing blind tokens
	if (!flags.BlindsCon) {
		flags.BlindsCon = BlindsToss(bigblind, deltaTime);
	}

	// turning tokens
	if (flags.BlindsCon) {
		TurningBlinds(deltaTime);
	}

	// showing token notes
	if (flags.BlindsCon) {
		//Showing token notes
		if (BigBlind.getComponent<MouseController>().hovered) {
			BigBlindNote.getComponent<SpriteComponent>().shown();

		}
		else {
			BigBlindNote.getComponent<SpriteComponent>().hidden();
		}

		if (SmallBlind.getComponent<MouseController>().hovered) {
			SmallBlindNote.getComponent<SpriteComponent>().shown();

		}
		else {
			SmallBlindNote.getComponent<SpriteComponent>().hidden();
		}
	}

	if (!flags.GameEnded) {
		HandleBetButtons();
	}

	if (flags.EnemyMove) {
		cout << "Enemy move\n";
		flags.EnemyMove = false;
	}
	

	//Displaying first 3 cards
	if (flags.Show3Cards) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		cout << "3cards" << endl;
		card1.getComponent<SpriteComponent>().shown();
		card2.getComponent<SpriteComponent>().shown();
		card3.getComponent<SpriteComponent>().shown();

		flags.Show3Cards = false;
		flags.EnemyMove = true;
	}

	//Enemy move
	if (currentBet[0] > currentBet[1]) {
		while (currentBet[0] != currentBet[1]) {
			money[1]--;
			currentBet[1]++;
			pool++;
		}
	}
	

	//Display 4th card
	if (flags.Show4Card) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		cout << "4 card" << endl;
		
		card4.getComponent<SpriteComponent>().shown();
		flags.Show4Card = false;
		flags.EnemyMove = true;
	}

	//Display 5th card
	if (flags.Show5Card) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		cout << "5 card" << endl;
		
		card5.getComponent<SpriteComponent>().shown();
		flags.Show5Card = false;
		flags.EnemyMove = true;
	}

	if (flags.endRound) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		enemy_card1.getComponent<SpriteComponent>().changeSprite(players[1].c1.get_suit_int(), players[1].c1.get_rank() - 2);
		enemy_card2.getComponent<SpriteComponent>().changeSprite(players[1].c2.get_suit_int(), players[1].c2.get_rank() - 2);
		for (int i = 0; i < players_num; i++) {
			hands.push_back(combine(table, players, i));
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

		flags.endRound = false;
		flags.GameEnded = true;
	}
}

void Game::update(float deltaTime)
{
	SDL_GetMouseState(&x, &y);
	Mouse.getComponent<PositionComponent>().x(x / r_scale - 1);
	Mouse.getComponent<PositionComponent>().y(y / r_scale);
	manager.update();
	if (flags.StartGame) {
		if (Start_button.getComponent<MouseController>().down) {
			Start_button.getComponent<SpriteComponent>().setTex("assets/Start_button_c.png");
			if (event.type == SDL_MOUSEBUTTONUP) {
				Start_button.getComponent<MouseController>().down = false;
				Start_button.getComponent<SpriteComponent>().setTex("assets/Start_button.png");
				flags.StartGame = false;
				Start_button.destroy();
				Blinds(bigblind);
			}
		}
		
		return;
	}

	Round(deltaTime);
	
	/*if (nextround)
	{
		card1.getComponent<SpriteComponent>().hidden();
		card2.getComponent<SpriteComponent>().hidden();
		card3.getComponent<SpriteComponent>().hidden();
		card4.getComponent<SpriteComponent>().hidden();
		card5.getComponent<SpriteComponent>().hidden();

		enemy_card1.getComponent<SpriteComponent>().changeSprite(0, 13);
		enemy_card2.getComponent<SpriteComponent>().changeSprite(0, 13);

		NextCards(Deck);
		nextround = false;
	}*/
	
	


	//SPRITE DRAGGING

	if (cowboy.getComponent<MouseController>().down) {
		SDL_GetMouseState(&x, &y);
		cowboy.getComponent<PositionComponent>().x((x / r_scale - 30));
		cowboy.getComponent<PositionComponent>().y((y / r_scale - 30));
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
