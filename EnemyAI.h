#pragma once
#include "flags.h"

extern Flags flags;
extern int currentBB;
extern void MoneyTransfer(int& i, float deltaTime);

class Enemy {
private:
	int score;
	int score_table;
	int maxHandRank;
	int* mmoney;
public:
	Enemy() = default;

	void set_money(int* m) {
		mmoney = m;
	}
	void set_maxHandRank(int r) {
		maxHandRank = r;
	}
	void set_score(int s) {
		score = s;
	}
	void set_score_table(int s) {
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
				return *mmoney * (Random(5, 10) *0.01);
			}
			else if (maxHandRank > 10 && score > 9) { //If the handcard is greater then 10 bet 10-20% of the money
				return *mmoney * (Random(10, 20) * 0.01);
			}
			else if (score <= 9 && score >= 6) {
				return *mmoney * (Random(15, 30) * 0.01);
			}
			else {
				return *mmoney * (Random(30, 50) * 0.01);
			}
		}
		else if (bet == 10) {
			cout << "Dyszka";
		}
		else {
			return bet;
		}

	}


	
};