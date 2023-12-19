#pragma once
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <iterator>

static int maxStraightRank = 0;
static int maxPokerRank = 0;
static int pairRank = 0;
static int threeRank = 0;
static int fourRank = 0;
static char flushSuit = '\0';

int FourOfAKind(const vector<Card>& cards, int score) {
	for (const Card& card : cards) {
		// Count occurrences of cards with the same rank
		int count = count_if(cards.begin(), cards.end(), [&](const Card& c) {
			return c.get_rank() == card.get_rank();
			});

		if (count >= 4) {
			score = min(score, 3); //four of kind
			fourRank = card.get_rank();
		}
	}

	return score;
}

int Pairs(const std::vector<Card>& cards, int score) {
	int pair_cnt = 0;
	for (const Card& card : cards) {
		// Count occurrences of cards with the same rank
		int count = count_if(cards.begin(), cards.end(), [&](const Card& c) {
			return c.get_rank() == card.get_rank();
			});

		if (count == 2) {
			score = min(score, 9); //pair
			pairRank = card.get_rank();
			pair_cnt++;
		}
		if (pair_cnt >= 4) { // 4 because for loop goes through every card
			score = min(score, 8); //two pairs
		}
	}

	return score;
}

int Flush(const vector<Card>& cards, int score) {
	int f_cnt[4] = { 0,0,0,0 };
	for (int i = 0; i <= cards.size() - 1; i++) {
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
	}
	if (f_cnt[0] >= 5) {
		score = min(score, 5); //flush
		flushSuit = 'H';
	}
	if (f_cnt[1] >= 5) {
		score = min(score, 5); //flush
		flushSuit = 'D';
	}
	if (f_cnt[2] >= 5) {
		score = min(score, 5); //flush
		flushSuit = 'C';
	}
	if (f_cnt[3] >= 5) {
		score = min(score, 5); //flush
		flushSuit = 'S';
	}
	return score;
}

int SameRanks(const vector<Card>& cards, int score) {
	int index = 0;
	int cnt;
	while (cards[index] != cards[6]) {
		cnt = 1;
		//check in a sorted vector if the next element is the same as the index element else move index to the next element
		for (int i = index; i < cards.size() - 1; i++) {
			if (cards[index] == cards[i + 1]) {
				cnt++;
				continue;
			}
			index = i + 1;
			break;
		}
		if (cnt == 3) {
			score = min(score, 7);//three of kind
			int cnt_fh = 1;
			//check if the cards also contains a pair making it a "full house"
			for (int i = 0; i < cards.size() - 1; i++) {
				if (cards[i] == cards[index - 1]) {
					continue;
				}
				if (cards[i] == cards[i + 1]) {
					cnt_fh++;
				}
			}
			if (cnt_fh >= 2) {
				score = min(score, 4); //full house
			}

		}
		return score;
	}
}

int Straight(const vector<Card>& cards, int score) {
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
			maxPokerRank = maxStraightRank;
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

int win(const vector<Card>& cards) {
	//reset static values for every check
	maxStraightRank = 0;
	flushSuit = '\0';
	int score = 10;

	//straight
	score = min(score, Straight(cards, score));

	//Flush
	score = min(score, Flush(cards, score));

	//three of kinds, full house and pairs
	score = min(score, SameRanks(cards, score));

	score = min(score, FourOfAKind(cards, score));

	score = min(score, Pairs(cards, score));
	return score;
}

void score(int s) {
	switch (s) {
	case 1:
		cout << "ROYAL POKER!!!" << endl;
		break;
	case 2:
		cout << "Poker! Highest card: " << maxPokerRank << std::endl;
		break;
	case 3:
		cout << "Four of kind!" << endl;
		break;
	case 4:
		cout << "Full house!" << endl;
		break;
	case 5:
		cout << "Flush";
		if (flushSuit == 'H') {
			cout << "Hearts";
		}
		if (flushSuit == 'D') {
			cout << "Diamonds";
		}
		if (flushSuit == 'C') {
			cout << "Clubs";
		}
		if (flushSuit == 'S') {
			cout << "Spades";
		}
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
		cout << "Pair!" << " " << pairRank << endl;
		break;
	case 10:
		cout << "High card!" << endl;
		break;
	}

	//Reset all the variables for the next player
	maxStraightRank = 0;
	maxPokerRank = 0;
	pairRank = 0;
	threeRank = 0;
	fourRank = 0;
	flushSuit = '\0';
}