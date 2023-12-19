#pragma once

#include <iostream>

char CardSuit[4] = { 'S', 'D', 'C', 'H' }; //SPADES, DIAMONDS, CLUBS, HEARTS
int CardRank[13] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }; // 11 = JACK, 12 = QUEEN, 13 = KING, 14 = ACE

class Card {
protected:
	int rank;
	char suit;
public:
	Card() : rank(0), suit('\0') {}
	Card(int _rank, char _suit) : rank(_rank), suit(_suit) {}
	Card(const Card& c) {
		rank = c.rank;
		suit = c.suit;
	}

	int get_rank() const {
		return rank;
	}
	char get_suit() const {
		return suit;
	}

	int get_suit_int() const {
		if (suit == 'S') { return 0; }
		if (suit == 'D') { return 1; }
		if (suit == 'C') { return 2; }
		if (suit == 'H') { return 3; }
	}
	//Overloaded operators
	Card& operator=(const Card& c) {
		if (this != &c) {
			rank = c.rank;
			suit = c.suit;
		}
		return *this;
	}

	Card operator-(int value) const {
		Card result = *this;
		result.rank -= value;
		return result;
	}

	bool operator!=(const Card& other) const {
		if (rank != other.rank)
			return true;
		else
			return false;
	}

	bool operator==(const Card& other) const {
		if (rank == other.rank)
			return true;
		else
			return false;
	}

	bool operator<(const Card& other) const {
		if (rank < other.rank)
			return true;
		else if (rank > other.rank)
			return false;
		else
			return suit < other.suit;
	}

	friend std::ostream& operator<<(std::ostream& os, const Card& c);
};

std::ostream& operator<<(std::ostream& os, const Card& c) {
	if (c.rank == 11) {
		os << "J" << " " << c.suit;
	}
	else if (c.rank == 12) {
		os << "Q" << " " << c.suit;
	}
	else if (c.rank == 13) {
		os << "K" << " " << c.suit;
	}
	else if (c.rank == 14) {
		os << "A" << " " << c.suit;
	}
	else
		os << c.rank << " " << c.suit;
	return os;
}