#pragma once

#include <iostream>
#include "Card.h"

void fill_deck(vector<Card>& d) {
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 4; j++) {
			Card temp(CardRank[i], CardSuit[j]);
			d.push_back(temp);
		}
	}
}