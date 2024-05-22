#pragma once
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <iterator>

int maxStraightRank = 0;
int threeRank = 0;

int FourOfAKind_s(const vector<Card>& cards, int score) {
	for (const Card& card : cards) {
		// Count occurrences of cards with the same rank
		int count = count_if(cards.begin(), cards.end(), [&](const Card& c) {
			return c.get_rank() == card.get_rank();
			});

		if (count >= 4) {
			score = min(score, 3); //four of kind
		}
	}

	return score;
}

int Pairs_s(const std::vector<Card>& cards, int score) {
	int pair_cnt = 0;
	for (const Card& card : cards) {
		// Count occurrences of cards with the same rank
		int count = count_if(cards.begin(), cards.end(), [&](const Card& c) {
			return c.get_rank() == card.get_rank();
			});

		if (count == 2) {
			score = min(score, 9); //pair
			pair_cnt++;
		}
		if (pair_cnt >= 4) { // 4 because for loop goes through every card
			score = min(score, 8); //two pairs
		}
	}

	return score;
}

int Flush_s(const vector<Card>& cards, int score) {
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
	}
	if (f_cnt[1] >= 5) {
		score = min(score, 5); //flush
	}
	if (f_cnt[2] >= 5) {
		score = min(score, 5); //flush
	}
	if (f_cnt[3] >= 5) {
		score = min(score, 5); //flush
	}
	return score;
}

int SameRanks_s(const vector<Card>& cards, int score) {
	if (cards.size() < 3) {
		return score;
	}
	int index = 0;
	int cnt;
	for (size_t i = 0; i <= cards.size() - 3; ++i) {
		if (cards[i] == cards[i + 1] && cards[i + 1] == cards[i + 2]) {
			// Three of kind found
			threeRank = cards[i].get_rank();
			score = min(score, 7);//three of kind

			//check if the cards also contains a pair making it a "full house"
			for (const Card& card : cards) {
				if (card.get_rank() == threeRank) { //Skip loop 
					continue;
				}
				else {
					// Count occurrences of cards with the same rank
					int count = count_if(cards.begin(), cards.end(), [&](const Card& c) {
						return c.get_rank() == card.get_rank();
						});

					if (count >= 2) {
						score = min(score, 4); //full house
					}
				}
			}
		}
	}
	return score;
}

int Straight_s(const vector<Card>& cards, int score) {
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
				maxStraightRank = max(maxStraightRank, cards[i + 1].get_rank());
				straight = true;
			}
		}
		if (straight && (f_cnt[0] >= 5 || f_cnt[1] >= 5 || f_cnt[2] >= 5 || f_cnt[3] >= 5)) {
			if (maxStraightRank == 14) {
					score = min(score, 1); //royal flush
			}
			score = min(score, 2); //straight flush
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

int win_Simplified(const vector<Card>& cards) {

	int score = 10;
	//Straight, Straight Flush or Royal Flush
	score = min(score, Straight_s(cards, score));
	
	//Flush
	score = min(score, Flush_s(cards, score));

	//Three of kinds and full house
	score = min(score, SameRanks_s(cards, score));

	//Four of kind
	score = min(score, FourOfAKind_s(cards, score));

	//Pairs
	score = min(score, Pairs_s(cards, score));

	return score;
}

void score_Simplified(int score) {
	switch (score) {
	case 1:
		cout << "ROYAL POKER!!!" << endl;
		break;
	case 2:
		cout << "Poker! Highest card: " << std::endl;
		break;
	case 3:
		cout << "Four of kind!" << endl;
		break;
	case 4:
		cout << "Full house!" << endl;
		break;
	case 5:
		cout << "Flush";
		cout << endl;
		break;
	case 6:
		cout << "Straight! Highest card: " << maxStraightRank << std::endl;
		break;
	case 7:
		cout << "three of kind!" << endl;
		break;
	case 8:
		cout << "Two Pairs!" << endl;
		break;
	case 9:
		cout << "Pair!" << endl;
		break;
	case 10:
		cout << "High card!" << endl;
		break;
	}
}


