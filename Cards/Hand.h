#pragma once

#include <iostream>
#include <random>
#include <vector>
#include "Card.h"

random_device rd;
mt19937 gen(rd());


class Hand : protected Card {
public:
	Card c1;
	Card c2;

	Hand();
	Hand(Card _c1, Card _c2) : c1(_c1), c2(_c2) {
	
	}
	/*Hand(Hand& h) {
		c1 = h.c1;
		c2 = h.c2;
	}*/

	friend std::ostream& operator<<(std::ostream& os, const Hand& h);


};

std::ostream& operator<<(std::ostream& os, const Hand& h) {
	os << h.c1 << " " << h.c2;
	return os;
}

Hand draw(vector<Card>& Deck) {
	// Define the range [0, Deck.size()]
	uniform_int_distribution<int> distribution(0, Deck.size() - 1);

	// Generate a random card and erase it from the deck
	int r1 = distribution(gen);
	int r2 = distribution(gen);
	if (r2 == r1) {
		r2 = distribution(gen);
	}

	Hand hand(Deck[r1], Deck[r2]);

	Deck.erase(remove(Deck.begin(), Deck.end(), hand.c1), Deck.end());
	Deck.erase(remove(Deck.begin(), Deck.end(), hand.c2), Deck.end());


	return hand;
}

vector<Card> combine(const vector<Card>& table, const vector<Hand>& players, int i) { //args = cards on the table, player's hands and number of players
	vector<Card> cards = table;
	cards.push_back(players[i].c1);
	cards.push_back(players[i].c2);
	sort(cards.begin(), cards.end());
	return cards;
}