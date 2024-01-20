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


SoundEffects Sound_effects;
#include "ECS/Entities.h"
#include "Sound/Sounds.h"
#include "Animations.h"
#include "UI.h"
Enemy enemy;
Player player;

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
	/*player.hand = draw(Deck);
	player.money = 100;

	enemy.hand = draw(Deck);
	enemy.money = 100;*/


	enemy.set_maxHandRank(scores[1].handRank);
	enemy.set_money(&money[1]);

	bigblind = Random(0, 1);

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

void NextCards(vector<Card> &Deck) {
	Deck.clear();
	fill_deck(Deck);
	DrawHands(Deck);
	DrawTable(Deck);

	hand_card1.getComponent<SpriteComponent>().changeSprite(players[0].c1.get_suit_int(), players[0].c1.get_rank() - 2);
	hand_card2.getComponent<SpriteComponent>().changeSprite(players[0].c2.get_suit_int(), players[0].c2.get_rank() - 2);

	enemy_card1.getComponent<SpriteComponent>().changeSprite(0, 13);
	enemy_card2.getComponent<SpriteComponent>().changeSprite(0, 13);

	card1.getComponent<SpriteComponent>().changeSprite(table[0].get_suit_int(), table[0].get_rank() - 2);
	card2.getComponent<SpriteComponent>().changeSprite(table[1].get_suit_int(), table[1].get_rank() - 2);
	card3.getComponent<SpriteComponent>().changeSprite(table[2].get_suit_int(), table[2].get_rank() - 2);
	card4.getComponent<SpriteComponent>().changeSprite(table[3].get_suit_int(), table[3].get_rank() - 2);
	card5.getComponent<SpriteComponent>().changeSprite(table[4].get_suit_int(), table[4].get_rank() - 2);

}


void HandleBetButtons() {

	//Bet button
	if (BB_center.getComponent<MouseController>().down) {
		BB_center.getComponent<SpriteComponent>().setTex("assets/Bet_button_center_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			Sound_effects.playSoundEffect("ClickButton", 0);
			BB_center.getComponent<MouseController>().down = false;
			BB_center.getComponent<SpriteComponent>().setTex("assets/Bet_button_center.png");
			pool += bet;
			money[0] -= bet;
			currentBet[0] += bet;
			bet = 0;
			lowestBet = 0;
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
		if (flags.buttonClickSFX) {
			Sound_effects.playSoundEffect("ClickButton", 0);
			flags.buttonClickSFX = false;
		}
		BB_add_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_big_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			flags.buttonClickSFX = true;
			BB_add_big.getComponent<MouseController>().down = false;
			BB_add_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_big.png");
			if (bet <= 989 && bet  <= money[0] - 10) {
				bet += 10;
			}
		}
	}

	//Plus 1 button
	if (BB_add_small.getComponent<MouseController>().down) {
		if (flags.buttonClickSFX) {
			Sound_effects.playSoundEffect("ClickButton", 0);
			flags.buttonClickSFX = false;
		}
		BB_add_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_small_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			flags.buttonClickSFX = true;
			BB_add_small.getComponent<MouseController>().down = false;
			BB_add_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_add_small.png");
			if (bet <= 998 && bet <= money[0] - 1) {
				bet += 1;
			}
		}
	}

	//Minus 10 button
	if (BB_sub_big.getComponent<MouseController>().down) {
		if (flags.buttonClickSFX) {
			Sound_effects.playSoundEffect("ClickButton", 0);
			flags.buttonClickSFX = false;
		}
		BB_sub_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_big_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			flags.buttonClickSFX = true;
			BB_sub_big.getComponent<MouseController>().down = false;
			BB_sub_big.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_big.png");
			if (bet >= 10 && bet - 10 >= lowestBet) {
				bet -= 10;
			}
		}
	}

	//Minus 1 button
	if (BB_sub_small.getComponent<MouseController>().down) {
		if (flags.buttonClickSFX) {
			Sound_effects.playSoundEffect("ClickButton", 0);
			flags.buttonClickSFX = false;
		}
		BB_sub_small.getComponent<SpriteComponent>().setTex("assets/Bet_button_sub_small_c.png");
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			flags.buttonClickSFX = true;
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
			Sound_effects.playSoundEffect("ClickButton", 0);
			Fold_button.getComponent<SpriteComponent>().setTex("assets/Fold_button.png");
			cout << "Player folded";
			Fold_button.getComponent<MouseController>().down = false;
			flags.PlayerFolded = true;
		}
	}
}

void TakeBlindMoney(int p) {
	if (money[p] < currentBB) 
	{
		currentBet[p] = money[p];
		money[p] = 0;
	}
	else {
		money[p] -= currentBB;
		currentBet[p] = currentBB;
	}
	if (p == 1) //enemy got the big blind
	{
		if (money[p - 1] < currentBB / 2)
		{
			currentBet[p - 1] = money[p - 1];
			money[p - 1] = 0;
			lowestBet = 0;
		}
		else
		{
			money[p - 1] -= currentBB / 2;
			currentBet[p - 1] = currentBB / 2;
			bet = lowestBet = currentBB / 2;

		}
	}
	else //player got the big blind
	{
		if (money[p + 1] < currentBB / 2)
		{
			currentBet[p + 1] = money[p + 1];
			money[p + 1] = 0;
		}
		else
		{
			money[p + 1] -= currentBB / 2;
			currentBet[p + 1] = currentBB / 2;
		}
		
	}
	for (int i = 0; i < players_num; i++) {
		pool += currentBet[i];
	}

}

void Blinds(int Blind) {
	TakeBlindMoney(Blind);

}





void Round(float deltaTime) {
	cowboy.getComponent<AnimationComponent>().LoopAnimation(1, deltaTime);
	HandCardsHover(deltaTime);
	
	//Folds
	if (flags.PlayerFolded) {
		if (pool > 0) {
			MoneyTransfer(money[1], deltaTime);
			return;
		}
		else {
			flags.PlayerFolded = false;
			flags.NextRound = true;
		}
	}
	if (flags.EnemyFolded) {
		if (pool > 0) {
			MoneyTransfer(money[0], deltaTime);
			return;
		}
		else {
			flags.EnemyFolded = false;
			flags.NextRound = true;
		}
	}

	//toosing blind tokens
	if (!flags.BlindsCon) {
		flags.BlindsCon = BlindsToss(bigblind, deltaTime);
	}

	// turning tokens
	if (flags.BlindsCon) {
		TurningBlinds(deltaTime);
	}
	if (flags.TakeBlinds) { 
		Blinds(bigblind);
		flags.TakeBlinds = false;	
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


	//Enemy move
	if (flags.EnemyMove) {
		enemy.set_score(win_Simplified(table));
		vector<Card> cards = combine(table, players, 1);
		enemy.set_score(win_Simplified(cards));
		int Bet_enemy = enemy.Decide(currentBet[0]);
		money[1] -= Bet_enemy;
		currentBet[1] += Bet_enemy;
		pool += Bet_enemy;
		flags.EnemyMove = false;
	}

	/*while (currentBet[0] != currentBet[1]) {
		money[1]--;
		currentBet[1]++;
		pool++;
	}*/

	//Displaying first 3 cards
	if (flags.Show3Cards) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		card1.getComponent<SpriteComponent>().shown();
		card2.getComponent<SpriteComponent>().shown();
		card3.getComponent<SpriteComponent>().shown();
		flags.Show3Cards = false;
		flags.EnemyMove = true;
	}

	//Display 4th card
	if (flags.Show4Card) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		
		card4.getComponent<SpriteComponent>().shown();
		flags.Show4Card = false;
		flags.EnemyMove = true;
	}

	//Display 5th card
	if (flags.Show5Card) {
		currentBet[0] = 0;
		currentBet[1] = 0;

		card5.getComponent<SpriteComponent>().shown();
		flags.Show5Card = false;
		flags.EnemyMove = true;
	}

	if (flags.endRound) {
		currentBet[0] = 0;
		currentBet[1] = 0;
		enemy_card1.getComponent<SpriteComponent>().changeSprite(players[1].c1.get_suit_int(), players[1].c1.get_rank() - 2);
		enemy_card2.getComponent<SpriteComponent>().changeSprite(players[1].c2.get_suit_int(), players[1].c2.get_rank() - 2);
		if (!flags.ResultCalc) { //Calculate the results of the round
			for (int i = 0; i < players_num; i++) {
				hands.push_back(combine(table, players, i));
			}
			Result(scores, hands);
			flags.ResultCalc = true;
		}
		if (scores[0].score < scores[1].score || scores[0].tieWin) {
			if (pool > 0) {
				MoneyTransfer(money[0], deltaTime);
				return;
			}
			else {
				cout << "Player Won!" << endl;
			}
		}
		else if (scores[0].score > scores[1].score || scores[1].tieWin) {
			if (pool > 0) {
				MoneyTransfer(money[1], deltaTime);
				return;
			}
			else {
				cout << "Enemy Won!" << endl;
			}
		}
		pool = 0;

		flags.ResultCalc = false;
		flags.endRound = false;
		flags.NextRound = true;
	}
}

float licznik;

void NextRound(float deltaTime) {
	RoundCounter++;
	if (money[0] <= 0) {
		cout << "Player Lost";
		flags.GameEnded = true;
	}
	if (money[1] <= 0) {
		cout << "Player Won";
		flags.GameEnded = true;
	}
	licznik += deltaTime;
	if (licznik < 3) {
		//Add after implementing completed Player class
		/*if (player.won) {

		}
		else {
		EnemyWonBanner.getComponent<SpriteComponent>().shown();
		}*/
		Star1.getComponent<SpriteComponent>().shown();
		Star1.getComponent<AnimationComponent>().LoopAnimation(20, deltaTime);
		YouWonBanner.getComponent<SpriteComponent>().shown();

		//main functionality
		flags.HandleButtons = false;
		return;
	}
	Star1.getComponent<SpriteComponent>().hidden();
	YouWonBanner.getComponent<SpriteComponent>().hidden();
	flags.HandleButtons = true;
	licznik = 0;
	pool = 0;
	card1.getComponent<SpriteComponent>().hidden();
	card2.getComponent<SpriteComponent>().hidden();
	card3.getComponent<SpriteComponent>().hidden();
	card4.getComponent<SpriteComponent>().hidden();
	card5.getComponent<SpriteComponent>().hidden();
	scores[0].ResetScore();
	scores[1].ResetScore();
	hands.clear();
	ResetIgnoreHands();
	NextCards(Deck);
	ResetBlinds();

	//Swap the location of the blinds tokens and notes
	if (bigblind == 0) {
		bigblind = 1;
		BigBlindNote.getComponent<PositionComponent>().x(139);
		BigBlindNote.getComponent<PositionComponent>().y(3);
		SmallBlindNote.getComponent<PositionComponent>().x(190);
		SmallBlindNote.getComponent<PositionComponent>().y(136);
	}
	else if (bigblind == 1) {
		bigblind = 0;
		BigBlindNote.getComponent<PositionComponent>().x(200);
		BigBlindNote.getComponent<PositionComponent>().y(136);
		SmallBlindNote.getComponent<PositionComponent>().x(139);
		SmallBlindNote.getComponent<PositionComponent>().y(3);
	}

	//doubling big blind every two rounds
	if (!(RoundCounter % 2)) { 
		currentBB *= 2;
	}

	flags.NextRoundFlags();
	flags.NextRound = false;
}

void Game::update(float deltaTime)
{
	manager.update();
	UpdateCursor();
	if (flags.GameEnded) {
		BigBlind.getComponent<SpriteComponent>().hidden();
		SmallBlind.getComponent<SpriteComponent>().hidden();
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
