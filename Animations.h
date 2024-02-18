#pragma once

//Cowboy breathing animation
float cowboy_anim_timer = 0;
int cowboy_anim_sprite = 1;

inline void CowboyAnim(float deltaTime) {
	cowboy_anim_timer += deltaTime;
	if (cowboy_anim_timer >= 1) {
		cowboy.getComponent<SpriteComponent>().changeSprite(cowboy_anim_sprite, 0);
		cowboy_anim_timer = 0;
		if (cowboy_anim_sprite == 1) {
			cowboy_anim_sprite = 0;
		}
		else if (cowboy_anim_sprite == 0) {
			cowboy_anim_sprite = 1;
		}
	}
}


//Tossing blinds tokens animation

float xB = 160;
float yB = 90;

float xS = 160;
float yS = 90;

inline bool BlindsToss(int Blind, float deltaTime) {
	int animRate = 80;
	bool con[4] = { 0,0,0,0 };
	if (Blind == 1)
	{
		xB -= deltaTime * animRate / 2;
		yB -= deltaTime * animRate;
		if (xB >= 122)
		{
			BigBlindToken.getComponent<PositionComponent>().x(xB);
		}
		else {
			con[0] = true;
		}
		if (yB >= 1)
		{
			BigBlindToken.getComponent<PositionComponent>().y(yB);
		}
		else {
			con[1] = true;
		}

		xS += deltaTime * animRate;
		yS += deltaTime * animRate / 2;
		if (xS <= 242)
		{
			SmallBlindToken.getComponent<PositionComponent>().x(xS);
		}
		else {
			con[2] = true;
		}
		if (yS <= 134)
		{
			SmallBlindToken.getComponent<PositionComponent>().y(yS);
		}
		else {
			con[3] = true;
		}

		if (con[0] && con[1] && con[2] && con[3]) { //Setting final position of the blinds in case the float numbers exceed the desired position
			BigBlindToken.getComponent<PositionComponent>().x(122);
			BigBlindToken.getComponent<PositionComponent>().y(1);
			SmallBlindToken.getComponent<PositionComponent>().x(242);
			SmallBlindToken.getComponent<PositionComponent>().y(134);
			flags.BlindsConTurn = true;
			return true;
		}
	}
	else if (Blind == 0)
	{
		xB += deltaTime * animRate;
		yB += deltaTime * animRate / 2;
		if (xB <= 242)
		{
			BigBlindToken.getComponent<PositionComponent>().x(xB);
		}
		else {
			con[0] = true;
		}

		if (yB <= 134)
		{
			BigBlindToken.getComponent<PositionComponent>().y(yB);
		}
		else {
			con[1] = true;
		}

		xS -= deltaTime * animRate / 2;
		yS -= deltaTime * animRate;
		if (xS >= 122)
		{
			SmallBlindToken.getComponent<PositionComponent>().x(xS);
		}
		else {
			con[2] = true;
		}
		if (yS >= 1)
		{
			SmallBlindToken.getComponent<PositionComponent>().y(yS);
		}
		else {
			con[3] = true;
		}

		if (con[0] && con[1] && con[2] && con[3]) { //Setting final position of the blinds in case the float numbers exceed the desired position
			BigBlindToken.getComponent<PositionComponent>().x(242);
			BigBlindToken.getComponent<PositionComponent>().y(135);
			SmallBlindToken.getComponent<PositionComponent>().x(122);
			SmallBlindToken.getComponent<PositionComponent>().y(1);
			flags.BlindsConTurn = true;
			return true;
		}
	}
	return false;
}


//Turing blinds tokens animation

bool Turn_row = true; //change row in bigblind animation

bool Turn_stop = false; // stop bigblind animation

float Turn_spirte = 0;

inline void TurningBlinds(float deltaTime)
{
	if (!Turn_stop) {
		if (Turn_row) {
			BigBlindToken.getComponent<SpriteComponent>().changeSprite(int(Turn_spirte), 0);
			SmallBlindToken.getComponent<SpriteComponent>().changeSprite(int(Turn_spirte), 0);
		}
		else {
			BigBlindToken.getComponent<SpriteComponent>().changeSprite(int(Turn_spirte), 1);
			SmallBlindToken.getComponent<SpriteComponent>().changeSprite(int(Turn_spirte), 1);
		}

		Turn_spirte += deltaTime * 17;
		if (int(Turn_spirte) == 9 && !Turn_row) {
			Turn_stop = true;
		}
		if (int(Turn_spirte) >= 9) {
			Turn_spirte = 0;
			Turn_row = !Turn_row;
		}
	}
	else {
		flags.HandleButtons = true;
		flags.BlindsConTurn = false;
	}
}

inline void ResetBlinds() {
	BigBlindToken.getComponent<SpriteComponent>().changeSprite(0, 0);
	BigBlindToken.getComponent<PositionComponent>().x(160);
	BigBlindToken.getComponent<PositionComponent>().y(90);

	SmallBlindToken.getComponent<SpriteComponent>().changeSprite(0, 0);
	SmallBlindToken.getComponent<PositionComponent>().x(160);
	SmallBlindToken.getComponent<PositionComponent>().y(90);

	xB = 160;
	yB = 90;

	xS = 160;
	yS = 90;
	Turn_row = true; 

	Turn_stop = false; 
	Turn_spirte = 0;
}
//Hand cards hover animation

float Hand_card_h1 = 148;
float Hand_card_h2 = 148;

inline void HandCardsHover(float deltaTime) {
	if (hand_card1.getComponent<MouseController>().hovered) {
		if (Hand_card_h1 >= 133) {
			Hand_card_h1 -= deltaTime * 60; //move the card up to the y(133) coordinate
		}
		hand_card1.getComponent<PositionComponent>().y(int(Hand_card_h1));
	}
	else {
		if (Hand_card_h1 < 148) {
			Hand_card_h1 += deltaTime * 60; //move the card up to the y(148) Origin coordinate
		}
		else {
			Hand_card_h1 = 148; //set the base coordinate in case deltaTime has changed it
		}
		hand_card1.getComponent<PositionComponent>().y(int(Hand_card_h1));
	}

	if (hand_card2.getComponent<MouseController>().hovered) {
		if (Hand_card_h2 >= 133) {
			Hand_card_h2 -= deltaTime * 60; //move the card up to the y(133) coordinate
		}
		hand_card2.getComponent<PositionComponent>().y(int(Hand_card_h2));
	}
	else {
		if (Hand_card_h2 < 148) {
			Hand_card_h2 += deltaTime * 60; //move the card up to the y(148) Origin coordinate
		}
		else {
			Hand_card_h2 = 148; //set the base coordinate in case deltaTime has changed it
		}
		hand_card2.getComponent<PositionComponent>().y(int(Hand_card_h2));

	}

}

float money_transfer_anim;

inline void MoneyTransfer(int& i, float deltaTime) {
	if (flags.PlayCoinLoop) {
		Sound_effects.playSoundEffectTime("CoinDrop", ((0.02f) * pool * 1000) - 40);
		flags.PlayCoinLoop = false;
	}
	money_transfer_anim += deltaTime;
	if (pool == 20) {
		Sound_effects.playSoundEffect("SingleCoinDrop", 0);
	}
	if (money_transfer_anim >= (0.02f)) { //Transfer money from pool to player/enemy. Larger the pool faster the transfer occurs
		money_transfer_anim = 0;
		pool--;
		i++;
	}
}

inline void UpdateCursor() {
	SDL_GetMouseState(&x, &y);
	Mouse.getComponent<PositionComponent>().x(x / r_scale - 1);
	Mouse.getComponent<PositionComponent>().y(y / r_scale);
}

//VFX Animation
float star1_timer;
int star1_sprite = 0;

inline void Star1_Animation(float deltaTime) {
	star1_timer += deltaTime;
	if (star1_timer >= 0.05) {
		Star1.getComponent<SpriteComponent>().changeSprite(star1_sprite++, 0);
		star1_timer = 0;
		if (star1_sprite >= 12) {
			star1_sprite = 0;
			Star1.getComponent<PositionComponent>().x(Random(2, 50));
			Star1.getComponent<PositionComponent>().y(Random(25, 35));
		}
	}
}