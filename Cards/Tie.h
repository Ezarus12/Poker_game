#pragma once

#include <iostream>
#include <vector>
#include "Card.h"

void ignoreHands(int ignored) { //Set the specific hand to true and all of the higher ones too
	for (int i = 0; i <= ignored; i++) {
		ignoreHand[i] = true;
	}
}

void ResetIgnoreHands() {
	for (int i = 0; i <= 9; i++) {
		ignoreHand[i] = false;
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
			s[0].tieWin = true;
		}
		else if (s[0].maxPokerRank < s[1].maxPokerRank) {
			s[1].tieWin = true;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 3: //Four of kind
		if (s[0].fourRank > s[1].fourRank) {
			s[0].tieWin = true;
		}
		else if (s[0].fourRank < s[1].fourRank) {
			s[1].tieWin = true;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 4: //Full house
		if (s[0].threeRank > s[1].threeRank) {
			s[0].tieWin = true;
		}
		else if (s[0].threeRank < s[1].threeRank) {
			s[1].tieWin = true;
		}
		else {
			if (s[0].pairRank > s[1].pairRank) {
				s[0].tieWin = true;
			}
			else if (s[0].pairRank < s[1].pairRank) {
				s[1].tieWin = true;
			}
			else {
				ignoreHands(s[0].score);
				Result(s, hands);
		
		}
		break;
	case 5: //Flush
		if (s[0].maxFlushRank > s[1].maxFlushRank) {
			s[0].tieWin = true;
		}
		else if (s[0].maxFlushRank < s[1].maxFlushRank) {
			s[1].tieWin = true;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 6: //Straight
		if (s[0].maxStraightRank > s[1].maxStraightRank) {
			s[0].tieWin = true;
		}
		else if (s[0].maxStraightRank < s[1].maxStraightRank) {
			s[1].tieWin = true;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 7: //Three of kinds
		if (s[0].threeRank > s[1].threeRank) {
			s[0].tieWin = true;
		}
		else if (s[0].threeRank < s[1].threeRank) {
			s[1].tieWin = true;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 8: //Two pairs
		if (s[0].pairRank > s[1].pairRank) {
			s[0].tieWin = true;
		}
		else if (s[0].pairRank < s[1].pairRank) {
			s[1].tieWin = true;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 9: //Pairs
		if (s[0].pairRank > s[1].pairRank) {
			s[0].tieWin = true;
		}
		else if (s[0].pairRank < s[1].pairRank) {
			s[1].tieWin = true;
		}
		else {
			ignoreHands(s[0].score);
			Result(s, hands);
		}
		break;
	case 10: //High Card
		if (s[0].handRank > s[1].handRank) {
			s[0].tieWin = true;
		}
		else if (s[0].handRank < s[1].handRank) {
			s[1].tieWin = true;
		}
		else {
			cout << "Draw";
		}
		break;
		}
	}
}