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
	int* money;
	bool allIn = false;
	bool fold = false;
	bool raised = false;
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
	void set_score_table(int s) {
		score_table = s;
	}

	bool get_AllIn() {
		return allIn;
	}

	void Fold() {
		fold = true;
	}
	bool get_fold() {
		return fold;
	}
	void set_fold(bool f) {
		fold = f;
	}

	void set_raised(bool f) {
		raised = f;
	}

	bool get_raised() {
		return raised;
	}

	int Call() {
		return 0;
	}
	int Decide(int bet) {
		if (bet > *money) {
			if (score <= 9 || maxHandRank >= 11) {
				cout << "All in\n";
				allIn = true;
				return *money;
			}
			else {
				cout << "Fold";
				Fold();
				return 0;
			}

		}

		if (raised && bet == currentBet[1]) {
			flags.BigBlindCalled = true;
			return 0;
		}

		//Enemy got bigblind
		if (bigblind && flags.firstBet) {
			//Player called
			if (bet == currentBet[1]) {
				flags.BigBlindCalled = true;
				cout << "No raise\n";
				return 0;
			}
			//Player raised
			else if (bet >= currentBet[1]) {
				flags.BigBlindCalled = true;
				cout << "Called as big blind\n";
				return currentBet[0] - currentBet[1];
			}

		}

		//Enemy got smallblind
		if (!bigblind && flags.firstBet) {
			//Player have not raised
			if (bet == currentBB) {
				if (maxHandRank <= 3 && score == 10) {
					cout << "Fold";
					Fold();
					return 0;
				}
				else { //enemy calls the bigblind
					cout << "Polowa bigblinda" << endl;
					return currentBB / 2;
				}
			}
			else if (bet > currentBet[1]) { //player raised
				if (bet >= 0.5 * (*money)) {
					cout << "Fold";
					Fold();
					return 0;
				}
				else {
					cout << "Called as small blind\n";
					return currentBet[0] - currentBet[1];
				}
			}
		}

		if (bet == 0) {
			if (Random(0, 100) <= 70) { //70% chance to check
				cout << "Checked" << endl;
				if (bigblind) {
					flags.BigBlindCalled = true;
				}
				
				return Call();
			}
			else if (maxHandRank < 10 && score > 9) { //In the other 30% bet 5-10% of the money
				cout << "Betted 5-10%" << endl;
				raised = true;
				return *money * (Random(5, 10) * 0.01);
			}
			else if (maxHandRank > 10 && score > 9) { //If the handcard is greater then 10 bet 10-20% of the money
				cout << "Betted 10-20%" << endl;
				raised = true;
				return *money * (Random(10, 20) * 0.01);
			}
			else if (score <= 9 && score >= 6) {
				cout << "Betted 15-30%" << endl;
				raised = true;
				return *money * (Random(15, 30) * 0.01);
			}
			else {
				cout << "Betted 30-50%, score: " << score << "Max hand rank: " << maxHandRank << endl;
				raised = true;
				return *money * (Random(30, 50) * 0.01);
			}
		}
		else if (bet < 0.5 * (*money)) { // Call if player has betted less then 50& of the enemy's money
			cout << "Call";
			if (bigblind) {
				flags.BigBlindCalled = true;
			}
			return bet;
		}
		else { // Fold if player betted >= 50% of the enemy's money
			cout << "Fold";
			Fold();
			return 0;
		}
	}
};