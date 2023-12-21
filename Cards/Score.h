#pragma once


class Score {
public:
	int score = 10; //Hand combination 1-Royal Poker... 10-Highest card

	int maxStraightRank = 0; //Highest card in the Straight
	int maxPokerRank = 0; //Highest card in the Poker
	int pairRank = 0; //Highest pair
	int threeRank = 0; //Highest three of kind
	int fourRank = 0; //Highest four of kind
	char flushSuit = '\0';
	int maxFlushRank = 0; //Highest card in the flush
	int handRank = 0; //Highest card in the hand

	Score() = default;
};