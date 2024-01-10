#pragma once


class Score {
public:
	int score = 10; //Hand combination 1-Royal Poker... 10-Highest card
	bool tieWin = false; //Does the player won in the Tie (same hands)
	int maxStraightRank = 0; //Highest card in the Straight
	int maxPokerRank = 0; //Highest card in the Poker
	int pairRank = 0; //Highest pair
	int threeRank = 0; //Highest three of kind
	int fourRank = 0; //Highest four of kind
	char flushSuit = '\0';
	int maxFlushRank = 0; //Highest card in the flush
	int handRank = 0; //Highest card in the hand

	Score() = default;

	void ResetScore() {
		score = 10; 
		tieWin = false; 
		maxStraightRank = 0; 
		maxPokerRank = 0; 
		pairRank = 0; 
		threeRank = 0; 
		fourRank = 0; 
		flushSuit = '\0';
		maxFlushRank = 0; 
		handRank = 0;
	}
};