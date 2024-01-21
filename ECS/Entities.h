#pragma once 


SDL_Color red = { 148,0,0,255 };
SDL_Color white = { 255,255,255,255 };


//Adding all of the entities
Manager manager;


/***********************
      L A Y E R  1
***********************/

//Enemy
auto& cowboy(manager.addEntity());

//table cards
auto& card1(manager.addEntity());
auto& card2(manager.addEntity());
auto& card3(manager.addEntity());
auto& card4(manager.addEntity());
auto& card5(manager.addEntity());

//Player hand cards
auto& hand_card1(manager.addEntity());
auto& hand_card2(manager.addEntity());

//Enemy hand cards
auto& enemy_card1(manager.addEntity());
auto& enemy_card2(manager.addEntity());

auto& back_card(manager.addEntity());

auto& CardBorder(manager.addEntity());

//betting button
auto& BB_center(manager.addEntity());
auto& BB_sub_big(manager.addEntity());
auto& BB_sub_small(manager.addEntity());
auto& BB_add_big(manager.addEntity());
auto& BB_add_small(manager.addEntity());

auto& Fold_button(manager.addEntity());

//Text displayed on screen
auto& money_text(manager.addEntity());
auto& enemy_money_text(manager.addEntity());
auto& bet_text(manager.addEntity());
auto& pool_text(manager.addEntity());


//Blinds sprites
auto& BigBlind(manager.addEntity());
auto& BigBlindNote(manager.addEntity());

auto& SmallBlind(manager.addEntity());
auto& SmallBlindNote(manager.addEntity());


/***********************
	  L A Y E R  2
***********************/


//Start button
auto& Start_button(manager.addEntity());


auto& PokerRanking(manager.addEntity());

//End round banners
auto& YouWonBanner(manager.addEntity());

//VFX
auto& Star1(manager.addEntity());

vector<Entity*> Stars1;

//Creating 10 stars in one vector
void InitStars1() {
	for (int i = 0; i < 15; i++) {
		auto& e = manager.addEntity();
		e.addComponent<PositionComponent>();
		e.getComponent<PositionComponent>().x(Random(0, 310));
		e.getComponent<PositionComponent>().y(Random(122, 165));
		e.addComponent<SpriteComponent>("assets/Star2.png", 11, 11);
		e.addComponent<AnimationComponent>(121, 11);
		e.getComponent<SpriteComponent>().hidden();
		e.getComponent<AnimationComponent>().set_LoopSprite(Random(0,10));
		Stars1.push_back(&e);
	}
}

/***********************
	  L A Y E R  3
***********************/
//Cursor
auto& Mouse(manager.addEntity());



void InitEntities() {
	
	//Mouse cursor
	Mouse.addComponent<PositionComponent>();
	Mouse.addComponent<SpriteComponent>("assets/Cursor.png", 10, 11);

	//Cards displayed on the screen
	hand_card1.addComponent<PositionComponent>(62, 148);
	hand_card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, players[0].c1.get_suit_int(), players[0].c1.get_rank() - 2);
	hand_card1.addComponent<MouseController>();
	hand_card1.getComponent<MouseController>().setHover();

	hand_card2.addComponent<PositionComponent>(62 + card_spacing, 148);
	hand_card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, players[0].c2.get_suit_int(), players[0].c2.get_rank() - 2);
	hand_card2.addComponent<MouseController>();
	hand_card2.getComponent<MouseController>().setHover();

	back_card.addComponent<PositionComponent>(12, card_y);
	back_card.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, 0, 13);
	back_card.addComponent<MouseController>();
	back_card.getComponent<MouseController>().setHover();

	CardBorder.addComponent<PositionComponent>(11, card_y - 1);
	CardBorder.addComponent<SpriteComponent>("assets/Card_Border.png", 34, 50);
	CardBorder.getComponent<SpriteComponent>().hidden();

	card1.addComponent<PositionComponent>(card_x, card_y);
	card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[0].get_suit_int(), table[0].get_rank() - 2);
	card1.getComponent<SpriteComponent>().hidden();

	card2.addComponent<PositionComponent>(card_x + card_spacing * 1, card_y);
	card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[1].get_suit_int(), table[1].get_rank() - 2);
	card2.getComponent<SpriteComponent>().hidden();

	card3.addComponent<PositionComponent>(card_x + card_spacing * 2, card_y);
	card3.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[2].get_suit_int(), table[2].get_rank() - 2);
	card3.getComponent<SpriteComponent>().hidden();

	card4.addComponent<PositionComponent>(card_x + card_spacing * 3, card_y);
	card4.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[3].get_suit_int(), table[3].get_rank() - 2);
	card4.getComponent<SpriteComponent>().hidden();

	card5.addComponent<PositionComponent>(card_x + card_spacing * 4, card_y);
	card5.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, table[4].get_suit_int(), table[4].get_rank() - 2);
	card5.getComponent<SpriteComponent>().hidden();

	enemy_card1.addComponent<PositionComponent>(192, 10);
	enemy_card1.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, 0, 13);
	enemy_card2.addComponent<PositionComponent>(192 + card_spacing, 10);
	enemy_card2.addComponent<SpriteComponent>("assets/Deck.png", 32, 48, 0, 13);

	//Enemies
	cowboy.addComponent<PositionComponent>(60, 0);
	cowboy.addComponent<SpriteComponent>("assets/Sheriff_Sheet.png", 60, 60);
	cowboy.addComponent<MouseController>();
	cowboy.addComponent<AnimationComponent>(120, 60);

	//Bet buttons 
	BB_sub_big.addComponent<PositionComponent>(132, 152);
	BB_sub_big.addComponent<SpriteComponent>("assets/Bet_button_sub_big.png", 10, 27);
	BB_sub_big.addComponent<MouseController>();

	BB_sub_small.addComponent<PositionComponent>(142, 152);
	BB_sub_small.addComponent<SpriteComponent>("assets/Bet_button_sub_small.png", 10, 27);
	BB_sub_small.addComponent<MouseController>();

	BB_center.addComponent<PositionComponent>(152, 152);
	BB_center.addComponent<SpriteComponent>("assets/Bet_button_center.png", 34, 27);
	BB_center.addComponent<MouseController>();

	BB_add_small.addComponent<PositionComponent>(186, 152);
	BB_add_small.addComponent<SpriteComponent>("assets/Bet_button_add_small.png", 10, 27);
	BB_add_small.addComponent<MouseController>();

	BB_add_big.addComponent<PositionComponent>(196, 152);
	BB_add_big.addComponent<SpriteComponent>("assets/Bet_button_add_big.png", 10, 27);
	BB_add_big.addComponent<MouseController>();

	//Start/Fold buttons
	Start_button.addComponent<PositionComponent>(97, 66);
	Start_button.addComponent<SpriteComponent>("assets/Start_button.png", 126, 48);
	Start_button.addComponent<MouseController>();

	Fold_button.addComponent<PositionComponent>(216, 157);
	Fold_button.addComponent<SpriteComponent>("assets/Fold_button.png", 34, 18);
	Fold_button.addComponent<MouseController>();


	//Text

	money_text.addComponent<PositionComponent>(16, 7);
	money_text.addComponent<TextComponent>("assets/font.ttf", 11, std::to_string(0), red);
	money_text.getComponent<TextComponent>().setNum(&money[0]);

	money_text.addComponent<PositionComponent>(132, 24);
	money_text.addComponent<TextComponent>("assets/font.ttf", 11, std::to_string(0), red);
	money_text.getComponent<TextComponent>().setNum(&money[1]);

	bet_text.addComponent<PositionComponent>(155, 157);
	bet_text.addComponent<TextComponent>("assets/font.ttf", 13, std::to_string(0), red);
	bet_text.getComponent<TextComponent>().setNum(&bet);

	pool_text.addComponent<PositionComponent>(290, 7);
	pool_text.addComponent<TextComponent>("assets/font.ttf", 11, std::to_string(0), red);
	pool_text.getComponent<TextComponent>().setNum(&pool);

	//Blinds
	if (bigblind)
	{
		BigBlind.addComponent<PositionComponent>(160, 90);
		BigBlindNote.addComponent<PositionComponent>(139, 3);
		SmallBlindNote.addComponent<PositionComponent>(190, 136);
		SmallBlind.addComponent<PositionComponent>(160, 90);
	}
	else
	{
		BigBlind.addComponent<PositionComponent>(160, 90);
		BigBlindNote.addComponent<PositionComponent>(200, 136);
		SmallBlindNote.addComponent<PositionComponent>(139, 3);
		SmallBlind.addComponent<PositionComponent>(160, 90);
	}

	BigBlind.addComponent<SpriteComponent>("assets/BigBlindSpriteSheet.png", 16, 16);
	BigBlind.addComponent<MouseController>();
	BigBlind.getComponent<MouseController>().setHover();


	SmallBlind.addComponent<SpriteComponent>("assets/SmallBlindSpriteSheet.png", 16, 16);
	SmallBlind.addComponent<MouseController>();
	SmallBlind.getComponent<MouseController>().setHover();

	BigBlindNote.addComponent<SpriteComponent>("assets/BigBlindNote.png", 42, 12);
	BigBlindNote.getComponent<SpriteComponent>().hidden();

	SmallBlindNote.addComponent<SpriteComponent>("assets/SmallBlindNote.png", 51, 12);
	SmallBlindNote.getComponent<SpriteComponent>().hidden();

	
	

	//PokerRanking

	PokerRanking.addComponent<PositionComponent>(70, 0);
	PokerRanking.addComponent<SpriteComponent>("assets/Poker_hand_ranking.png", 180, 646);
	PokerRanking.getComponent<SpriteComponent>().hidden();

	//Round end banners
	YouWonBanner.addComponent<PositionComponent>(0, 118);
	YouWonBanner.addComponent<SpriteComponent>("assets/YouWonBanner.png", 320, 62);
	YouWonBanner.getComponent<SpriteComponent>().hidden();

	//VFX
	Star1.addComponent<PositionComponent>(40, 140);
	Star1.addComponent<SpriteComponent>("assets/Star2.png", 11, 11);
	Star1.addComponent<AnimationComponent>(121, 11);
	Star1.getComponent<SpriteComponent>().hidden();

	InitStars1();
}

