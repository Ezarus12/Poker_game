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
		score_table = s;
	}
	
	void Fold() {
		flags.EnemyFolded = true;
	}

	int Call() {
		return 0;
	}
	int Decide(int bet) {
		if (!bigblind && flags.firstBet) { // enemy got SmallBlind
			if (maxHandRank <= 3 && score == 10) { 
				flags.EnemyFolded = true;
				return 0;
			}
			cout << "here";
			return currentBB / 2;
		}
		if (bet == 0) {
			if (Random(0, 100) <= 70) { //70% chance to call
				cout << "Called" << endl;
				return Call();
			}
			else if (maxHandRank < 10 && score > 9) { //In the other 30% bet 5-10% of the money
				cout << "Betted 5-10%" << endl;
				return *mmoney * (Random(5, 10) *0.01);
			}
			else if (maxHandRank > 10 && score > 9) { //If the handcard is greater then 10 bet 10-20% of the money
				cout << "Betted 10-20%" << endl;
				return *mmoney * (Random(10, 20) * 0.01);
			}
			else if (score <= 9 && score >= 6) {
				cout << "Betted 15-30%" << endl;
				return *mmoney * (Random(15, 30) * 0.01);
			}
			else {
				cout << "Betted 30-50%, score: " << score << "Max hand rank: " << maxHandRank << endl;
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