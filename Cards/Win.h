#pragma once
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <iterator>

static bool ignoreHand[10]; //array storing value to ignore specific hand 0-9

int FourOfAKind(const vector<Card>& cards, Score& s) {
	int score = s.score;
	for (const Card& card : cards) {
		// Count occurrences of cards with the same rank
		int count = count_if(cards.begin(), cards.end(), [&](const Card& c) {
			return c.get_rank() == card.get_rank();
			});

		if (count >= 4) {
			score = min(score, 3); //four of kind
			s.fourRank = card.get_rank();
		}
	}

	return score;
}

int Pairs(const std::vector<Card>& cards, Score& s) {
	int score = s.score;
	int pair_cnt = 0;
	for (const Card& card : cards) {
		// Count occurrences of cards with the same rank
		int count = count_if(cards.begin(), cards.end(), [&](const Card& c) {
			return c.get_rank() == card.get_rank();
			});

		if (count == 2) {
			score = min(score, 9); //pair
			s.pairRank = card.get_rank();
			pair_cnt++;
		}
		if (!ignoreHand[7]) {
			if (pair_cnt >= 4) { // 4 because for loop goes through every card
				score = min(score, 8); //two pairs
			}
		}
	}

	return score;
}

int Flush(const vector<Card>& cards, Score& s) {
	int score = s.score;
	int f_cnt[4] = { 0,0,0,0 };
	int maxRank[4] = { 0,0,0,0 };
	for (int i = 0; i <= cards.size() - 1; i++) {
		if (cards[i].get_suit() == 'H') {
			f_cnt[0]++;
			maxRank[0] += cards[i].get_rank(); //Summing all of the flush cards to determin who has the higher flush
		}
		else if (cards[i].get_suit() == 'D') {
			f_cnt[1]++;
			maxRank[1] += cards[i].get_rank(); //Summing all of the flush cards to determin who has the higher flush
		}
		else if (cards[i].get_suit() == 'C') {
			f_cnt[2]++;
			maxRank[2] += cards[i].get_rank(); //Summing all of the flush cards to determin who has the higher flush
		}
		else if (cards[i].get_suit() == 'S') {
			f_cnt[3]++;
			maxRank[3] += cards[i].get_rank(); //Summing all of the flush cards to determin who has the higher flush
		}
	}
	if (f_cnt[0] >= 5) {

		score = min(score, 5); //flush
		s.flushSuit = 'H';
		s.maxFlushRank = maxRank[0];
	}
	if (f_cnt[1] >= 5) {
		score = min(score, 5); //flush
		s.flushSuit = 'D';
		s.maxFlushRank = maxRank[1];
	}
	if (f_cnt[2] >= 5) {
		score = min(score, 5); //flush
		s.flushSuit = 'C';
		s.maxFlushRank = maxRank[2];
	}
	if (f_cnt[3] >= 5) {
		score = min(score, 5); //flush
		s.flushSuit = 'S';
		s.maxFlushRank = maxRank[3];
	}
	return score;
}

int SameRanks(const vector<Card>& cards, Score& s) {
	int score = s.score;
	int index = 0;
	int cnt;
	for (size_t i = 0; i <= cards.size() - 3; ++i) {
		if (cards[i] == cards[i + 1] && cards[i + 1] == cards[i + 2]) {
			// Three of kind found
			s.threeRank = cards[i].get_rank();
			score = min(score, 7);//three of kind

			//check if the cards also contains a pair making it a "full house"
			for (const Card& card : cards) {
				if (card.get_rank() == s.threeRank) { //Skip loop 
					continue;
				}
				else {
					// Count occurrences of cards with the same rank
					int count = count_if(cards.begin(), cards.end(), [&](const Card& c) {
						return c.get_rank() == card.get_rank();
						});

					if (count >= 2) {
						score = min(score, 4); //full house
						s.pairRank = card.get_rank();
					}
				}

			}
		}
	}
	return score;
}

int Straight(const vector<Card>& cards, Score& s) {
	int score = s.score;
	int cnt = 1;
	bool straight = false;
	int f_cnt[4] = { 1,1,1,1 };

	for (int i = 0; i < cards.size() - 1; ++i) {
		if (cards[i] == cards[i + 1]) {
			if (cards[i].get_suit() == 'H') {
				f_cnt[0]++;
			}
			else if (cards[i].get_suit() == 'D') {
				f_cnt[1]++;
			}
			else if (cards[i].get_suit() == 'C') {
				f_cnt[2]++;
			}
			else if (cards[i].get_suit() == 'S') {
				f_cnt[3]++;
			}
			continue;
		}
		if (cards[i] == cards[i + 1] - 1) {
			cnt++;
			if (cards[i].get_suit() == 'H') {
				f_cnt[0]++;
			}
			else if (cards[i].get_suit() == 'D') {
				f_cnt[1]++;
			}
			else if (cards[i].get_suit() == 'C') {
				f_cnt[2]++;
			}
			else if (cards[i].get_suit() == 'S') {
				f_cnt[3]++;
			}
			if (cnt >= 5) {
				s.maxStraightRank = max(s.maxStraightRank, cards[i + 1].get_rank());
				straight = true;
			}
		}
		if (straight && (f_cnt[0] >= 5 || f_cnt[1] >= 5 || f_cnt[2] >= 5 || f_cnt[3] >= 5)) {
			if (!ignoreHand[0]) {
				if (s.maxStraightRank == 14) {
					score = min(score, 1); //royal flush
				}
			}
			if (!ignoreHand[1]) {
				score = min(score, 2); //straight flush
				s.maxPokerRank = s.maxStraightRank;
			}
		}

		if (straight) {
			score = min(score, 6); //straight
		}
		if (cards[i] != cards[i + 1] - 1) {
			cnt = 1;
			f_cnt[0] = 1;
			f_cnt[1] = 1;
			f_cnt[2] = 1;
			f_cnt[3] = 1;
		}
	}
	return score;
}

void win(const vector<Card>& cards, Score& s) {

	s.score = 10;
	//straight
	if (!ignoreHand[5]) {
		s.score = min(s.score, Straight(cards, s));
	}
	//Flush
	if (!ignoreHand[4]) {
		s.score = min(s.score, Flush(cards, s));
	}

	//Three of kinds and full house
	if (!ignoreHand[6]) {
		s.score = min(s.score, SameRanks(cards, s));
	}

	//Four of kind
	if (!ignoreHand[2]) {
		s.score = min(s.score, FourOfAKind(cards, s));
	}

	//Pairs
	if (!ignoreHand[8]) {
		s.score = min(s.score, Pairs(cards, s));
	}
}

void score(Score &s) {
	switch (s.score) {
	case 1:
		cout << "ROYAL POKER!!!" << endl;
		break;
	case 2:
		cout << "Poker! Highest card: " << s.maxPokerRank << std::endl;
		break;
	case 3:
		cout << "Four of kind!" << endl;
		break;
	case 4:
		cout << "Full house!" << endl;
		break;
	case 5:
		cout << "Flush";
		if (s.flushSuit == 'H') {
			cout << "Hearts";
		}
		if (s.flushSuit == 'D') {
			cout << "Diamonds";
		}
		if (s.flushSuit == 'C') {
			cout << "Clubs";
		}
		if (s.flushSuit == 'S') {
			cout << "Spades";
		}
		cout << endl;
		break;
	case 6:
		cout << "Straight! Highest card: " << s.maxStraightRank << std::endl;
		break;
	case 7:
		cout << "three of kind!" << endl;
		break;
	case 8:
		cout << "Two Pairs!" << endl;
		break;
	case 9:
		cout << "Pair!" << " " << s.pairRank << endl;
		break;
	case 10:
		cout << "High card!" << endl;
		break;
	}

	//Reset all the variables for the next player
	
}


void Compare(vector<Score>& s, vector<vector<Card>>& hands);

void Result(vector<Score>& s, vector<vector<Card>>& hands) {
	for (int i = 0; i < players_num; i++) {
		win(hands[i], s[i]);
		score(s[i]);
	}
	if (s[0].score < s[1].score) {
		cout << "Player Won!" << endl;
	}
	else if (s[0].score > s[1].score) {
		cout << "Enemy Won!" << endl;	
	}
	else {
		Compare(s, hands);
	}
}