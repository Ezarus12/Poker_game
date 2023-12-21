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
	case 1: //Royal Poker
		ignoreHands(s[0].score);
		Result(s, hands);
		break;
	case 2: //Poker
		if (s[0].maxPokerRank > s[1].maxPokerRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].maxPokerRank < s[1].maxPokerRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 3: //Four of kind
		if (s[0].fourRank > s[1].fourRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].fourRank < s[1].fourRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 4: //Full house
		if (s[0].threeRank > s[1].threeRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].threeRank < s[1].threeRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			if (s[0].pairRank > s[1].pairRank) {
				cout << "Player Won!" << endl;
			}
			else if (s[0].pairRank < s[1].pairRank) {
				cout << "Enemy Won!" << endl;
			}
			else {
				ignoreHands(s[0].score);
				Result(s, hands);
		
		}
		break;
	case 5: //Flush
		if (s[0].maxFlushRank > s[1].maxFlushRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].maxFlushRank < s[1].maxFlushRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 6: //Straight
		if (s[0].maxStraightRank > s[1].maxStraightRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].maxStraightRank < s[1].maxStraightRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 7: //Three of kinds
		if (s[0].threeRank > s[1].threeRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].threeRank < s[1].threeRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 8: //Two pairs
		if (s[0].pairRank > s[1].pairRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].pairRank < s[1].pairRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 9: //Pairs
		if (s[0].pairRank > s[1].pairRank) {
			cout << "Player Won!" << endl;
		}
		else if (s[0].pairRank < s[1].pairRank) {
			cout << "Enemy Won!" << endl;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
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