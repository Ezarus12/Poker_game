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
				PokerRankingBorder.getComponent<SpriteComponent>().shown();

			}
			else {
				PokerRanking.getComponent<SpriteComponent>().hidden();
				PokerRankingBorder.getComponent<SpriteComponent>().hidden();
				ScrollBar.getComponent<SpriteComponent>().hidden();
			}
		}

	}
}

inline void PokerRankingUpdate() {
	float yn = (((ScrollBar.getComponent<PositionComponent>().y() - 5) * 100) / 155) / 100;
	PokerRanking.getComponent<PositionComponent>().y(-468 * yn);
}

inline void JokerPU() {
	if (!PokerRanking.getComponent<SpriteComponent>().get_show()) {
		if (PU_Joker.getComponent<MouseController>().hovered) {
			PU_Border.getComponent<SpriteComponent>().shown();
		}
		else {
			PU_Border.getComponent<SpriteComponent>().hidden();

		}
		if (PU_Joker.getComponent<MouseController>().down) {
			PU_Border.getComponent<SpriteComponent>().hidden();
			if (Game::event.type == SDL_MOUSEBUTTONUP) {
				PU_Joker.getComponent<MouseController>().down = false;
				flags.HandleButtons = !flags.HandleButtons;
			}

		}
	}
}