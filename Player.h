#pragma once
#include "Cards/Components.h"

class Player {
public:
	int money;
	Hand* hand;
	Score* score;
	bool allIn = false;
	bool won;

	Player() {
		money = 0;
		score = nullptr;
		hand = nullptr;
		won = false;
	}

	void InitPlayer(int m, Hand* h) {
		money = m;
		hand = h;
		score = new Score;
	}

};