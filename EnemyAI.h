#pragma once
#include "flags.h"

extern Flags flags;
extern int currentBB;
extern void MoneyTransfer(int& i, float deltaTime);

class Enemy {
private:
	int score;
	int maxHandRank;
	int* money;
public:
	Enemy() = default;

	void set_money(int* m) {
		money = m;
	}
	void set_maxHandRank(int r) {
		maxHandRank = r;
	}
	void set_score(int s) {
		score = s;
	}
	
	void Fold() {
		flags.EnemyFolded = true;
	}

	int Call() {
		return 0;
	}
	int Decide(int bet) {
		if (bet == 0) {
			if (Random(0, 100) <= 70 && maxHandRank < 10 && score > 9) { //70% chance to call if the highest hand card is lower then 10 and current score i high hand
				return Call();
			}
			else if (maxHandRank < 10 && score > 9) { //In the other 30% bet 5-10% of the money
				return *money * (Random(5, 10) *0.01);
			}
			else if (maxHandRank > 10 && score > 9) { //If the handcard is greater then 10 bet 10-20% of the money
				return *money * (Random(10, 20) * 0.01);
			}
			else if (score <= 9 && score >= 6) {
				return *money * (Random(15, 30) * 0.01);
			}
			else {
				return *money * (Random(30, 50) * 0.01);
			}
		}
		else if (bet <= currentBB) {

		}

	}


	
};