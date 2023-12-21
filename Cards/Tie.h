#pragma once

#include <iostream>
#include <vector>
#include "Card.h"

void ignoreHands(int ignored) { //Set the specific hand to true and all of the higher ones too
	for (int i = 0; i <= ignored; i++) {
		ignoreHand[i] = true;
	}
}

void Compare(vector<Score>& s, vector<vector<Card>>& hands) {
	switch (s[0].score)
	{
	case 1:

		break;
	case 2:

		break;
	case 3:

		break;
	case 4:

		break;
	case 5:

		break;
	case 6:

		break;
	case 7:

		break;
	case 8:

		break;
	case 9: //Pairs
		if (s[0].pairRank < s[1].pairRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].pairRank > s[1].pairRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			ignoreHands(s[0].score);
			for (int i = 0; i < players_num; i++) {
				win(hands[i], s[i]);
				score(s[i]);
			}
			if (s[0].score < s[0].score) {
				cout << "Player Won!" << endl;
			}
			else if (s[0].score > s[0].score) {
				cout << "Enemy Won!" << endl;
			}
			else {
				Compare(s, hands);
			}
			break;
	case 10: //High Card
		if (s[0].handRank > s[1].handRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].handRank < s[1].handRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			cout << "Draw";
		}
		break;
		}
	}
}