#pragma once
#include "Wait.h"

void DrawHands(vector<Card>& Deck) {
	for (int i = 0; i < players_num; i++) {
		players[i] = (draw(Deck));
		scores[i].handRank = max(players[i].c1.get_rank(), players[i].c2.get_rank()); //Find highest card in hand
	}
}

void DrawTable(vector<Card>& Deck) {
	for (int i = 0; i < 5; i++) {
		uniform_int_distribution<int> distribution(0, Deck.size() - 1);
		int r = distribution(gen);
		table[i] = (Deck[r]);
		Deck.erase(Deck.begin() + r);
	}
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
			player.money -= bet;
			currentBet[0] += bet;
			bet = 0;
			lowestBet = 0;
			//cout << "Current bets: " << currentBet[0] << " " << currentBet[1] << endl;
			if (enemy.get_AllIn()) {
				flags.BigBlindCalled = true;
			}
			else if (!bigblind && currentBet[0] == currentBet[1]) {
				flags.BigBlindCalled = true;
				flags.EnemyMove = false;
				cout << "call as bigblind\n";
			}
			else if (currentBet[0] >= currentBet[1]) {
				flags.EnemyMove = true;
			}
			else {
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
			if (bet <= 989 && bet <= player.money - 10) {
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
			if (bet <= 998 && bet <= player.money - 1) {
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
			Fold_button.getComponent<MouseController>().down = false;
			flags.PlayerFolded = true;
		}
	}
}

void TakeBlindMoney(int p) {
	if (p == 1) //enemy got the big blind
	{
		if (money[p] < currentBB)
		{
			currentBet[p] = money[p];
			money[p] = 0;
		}
		else {
			money[p] -= currentBB;
			currentBet[p] = currentBB;
		}
		if (player.money < currentBB / 2)
		{
			currentBet[p - 1] = player.money;
			player.money = 0;
			lowestBet = 0;
			player.allIn = true;
		}
		else
		{
			player.money -= currentBB / 2;
			currentBet[p - 1] = currentBB / 2;
			bet = lowestBet = currentBB / 2;

		}
	}
	else //player got the big blind
	{
		if (player.money < currentBB)
		{
			currentBet[p] = player.money;
			player.money = 0;
		}
		else {
			player.money -= currentBB;
			currentBet[p] = currentBB;
		}
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
		flags.EnemyMove = true;
	}
	for (int i = 0; i < players_num; i++) {
		pool += currentBet[i];
	}

}

void Blinds(int Blind) {
	TakeBlindMoney(Blind);

}


void NextCards(vector<Card>& Deck) {
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

void EqualBets() {
	if (currentBet[0] != currentBet[1]) {
		return;
	}

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
	if (enemy.get_fold()) {
		if (pool > 0) {
			MoneyTransfer(player.money, deltaTime);
			return;
		}
		else {
			enemy.set_fold(false);
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
		if (BigBlindToken.getComponent<MouseController>().hovered) {
			BigBlindNote.getComponent<SpriteComponent>().shown();

		}
		else {
			BigBlindNote.getComponent<SpriteComponent>().hidden();
		}

		if (SmallBlindToken.getComponent<MouseController>().hovered) {
			SmallBlindNote.getComponent<SpriteComponent>().shown();

		}
		else {
			SmallBlindNote.getComponent<SpriteComponent>().hidden();
		}
	}

	if (flags.firstBet && currentBet[0] == currentBet[1] && flags.BigBlindCalled || player.allIn && flags.firstBet) {
		flags.Show3Cards = true;
		flags.secondBet = true;
		flags.BigBlindCalled = false;
	}
	else if (flags.secondBet && flags.BigBlindCalled) {
		flags.secondBet = false;
		flags.Show4Card = true;
		flags.thirdBet = true;
		flags.BigBlindCalled = false;
	}
	else if (flags.thirdBet && flags.BigBlindCalled) {
		flags.thirdBet = false;
		flags.Show5Card = true;
		flags.fourthBet = true;
		flags.BigBlindCalled = false;
	}
	else if (flags.fourthBet && flags.BigBlindCalled) {
		flags.fourthBet = false;
		flags.endRound = true;
		flags.BigBlindCalled = false;
	}

	//Enemy move
	if (flags.EnemyMove) {
		if (Wait(deltaTime, 1)) {}
		else {
			return;
		}
		cout << "Enemy move: \n";
		if (enemy.get_AllIn()) {
			cout << "Enemy all in";
			flags.EnemyMove = false;
		}
		else {
			static vector<Card> temp; //Vector containing current cards on the table
			for (static int i = 0; i < CardsOnTable; i++) { //Filling temp with cards on the table
				temp.push_back(table[i]);
			}
			vector<Card> cards = combine(temp, players, 1);
			enemy.set_score(win_Simplified(cards));
			currentBet[1] += enemy.Decide(currentBet[0]);
			if (!enemy.get_fold()) {
				if (bigblind && currentBet[0] == currentBet[1]) {
					flags.BigBlindCalled = true;
				}
				if (currentBet[1] > currentBet[0]) {
					lowestBet = currentBet[1] - currentBet[0];
					if (lowestBet >= player.money) {
						lowestBet = player.money;
					}
				}
				else {
					lowestBet = 0;
				}
				if (bigblind && flags.firstBet) {
					bet = lowestBet;
					money[1] -= currentBet[1] - currentBB;
					pool += currentBet[1] - currentBB;
					flags.BigBlindCalled = true;
				}
				else if (!bigblind && flags.firstBet) {
					bet = lowestBet;
					if (currentBet[0] == currentBB) {
						money[1] -= currentBet[1] - currentBB / 2;
						pool += currentBet[1] - currentBB / 2;
					}
					else {
						money[1] -= currentBet[1] - currentBB;
						pool += currentBet[1] - currentBB;
						flags.BigBlindCalled = true;
					}
				}
				else {
					bet = lowestBet;
					money[1] -= currentBet[1];
					pool += currentBet[1];
				}

				flags.EnemyMove = false;
			}
		}
	}

	//Displaying first 3 cards
	if (flags.Show3Cards) {
		if (Wait(deltaTime, 2)) {
			betted_enemy_text.getComponent<PositionComponent>().y(24);
		}
		else {
			betted_enemy_text.getComponent<PositionComponent>().y(betted_enemy_text.getComponent<PositionComponent>().y() + (10 * deltaTime));
			return;
		}
		if (!bigblind) { //small blind starts the round
			flags.EnemyMove = true;
		}
		flags.firstBet = false;
		CardsOnTable = 3;
		currentBet[0] = 0;
		currentBet[1] = 0;
		card1.getComponent<SpriteComponent>().shown();
		card2.getComponent<SpriteComponent>().shown();
		card3.getComponent<SpriteComponent>().shown();
		flags.Show3Cards = false;
		//flags.EnemyMove = true;
	}

	//Display 4th card
	if (flags.Show4Card) {
		if (Wait(deltaTime, 2)) {
			betted_enemy_text.getComponent<PositionComponent>().y(24);
		}
		else {
			betted_enemy_text.getComponent<PositionComponent>().y(betted_enemy_text.getComponent<PositionComponent>().y() + (10 * deltaTime));
			return;
		}
		if (!bigblind) { //small blind starts the round
			flags.EnemyMove = true;
		}
		CardsOnTable = 4;
		currentBet[0] = 0;
		currentBet[1] = 0;

		card4.getComponent<SpriteComponent>().shown();
		flags.Show4Card = false;
		//flags.EnemyMove = true;
	}

	//Display 5th card
	if (flags.Show5Card) {
		if (Wait(deltaTime, 2)) {
			betted_enemy_text.getComponent<PositionComponent>().y(24);
		}
		else {
			betted_enemy_text.getComponent<PositionComponent>().y(betted_enemy_text.getComponent<PositionComponent>().y() + (10 * deltaTime));
			return;
		}
		if (!bigblind) { //small blind starts the round
			flags.EnemyMove = true;
		}
		CardsOnTable = 5;
		currentBet[0] = 0;
		currentBet[1] = 0;

		card5.getComponent<SpriteComponent>().shown();
		flags.Show5Card = false;
		//flags.EnemyMove = true;
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
				MoneyTransfer(player.money, deltaTime);
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
	if (player.money <= 0) {
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
		for (auto& e : Stars1) {
			e->getComponent<SpriteComponent>().shown();
			e->getComponent<AnimationComponent>().LoopAnimation(20, deltaTime);
		}
		Star1.getComponent<SpriteComponent>().shown();
		Star1.getComponent<AnimationComponent>().LoopAnimation(20, deltaTime);
		YouWonBanner.getComponent<SpriteComponent>().shown();

		//main functionality
		flags.HandleButtons = false;
		return;
	}
	for (auto& e : Stars1) {
		e->getComponent<SpriteComponent>().hidden();
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
	RoundCounter++;
	if (!(RoundCounter % 2)) {
		currentBB *= 2;
	}
	cout << currentBB;
	flags.NextRoundFlags();
	flags.NextRound = false;
}