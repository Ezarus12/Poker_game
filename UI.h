#pragma once

inline void PokerRankingTable() {
	if (back_card.getComponent<MouseController>().hovered) {
		CardBorder.getComponent<SpriteComponent>().shown();
	}
	else {
		CardBorder.getComponent<SpriteComponent>().hidden();

	}
	if (back_card.getComponent<MouseController>().down) {
		CardBorder.getComponent<SpriteComponent>().hidden();
		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			back_card.getComponent<MouseController>().down = false;
			flags.RankingShown = !flags.RankingShown;
			if (flags.RankingShown) {
				PokerRanking.getComponent<SpriteComponent>().shown();
				ScrollBar.getComponent<SpriteComponent>().shown();
			}
			else {
				PokerRanking.getComponent<SpriteComponent>().hidden();
				ScrollBar.getComponent<SpriteComponent>().hidden();
			}
		}

	}
}