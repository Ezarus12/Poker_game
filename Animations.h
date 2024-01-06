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
			BigBlind.getComponent<PositionComponent>().x(xB);
		}
		else {
			con[0] = true;
		}
		if (yB >= 1)
		{
			BigBlind.getComponent<PositionComponent>().y(yB);
		}
		else {
			con[1] = true;
		}

		xS += deltaTime * animRate;
		yS += deltaTime * animRate / 2;
		if (xS <= 242)
		{
			SmallBlind.getComponent<PositionComponent>().x(xS);
		}
		else {
			con[2] = true;
		}
		if (yS <= 134)
		{
			SmallBlind.getComponent<PositionComponent>().y(yS);
		}
		else {
			con[3] = true;
		}

		if (con[0] && con[1] && con[2] && con[3]) { //Setting final position of the blinds in case the float numbers exceed the desired position
			BigBlind.getComponent<PositionComponent>().x(122);
			BigBlind.getComponent<PositionComponent>().y(1);
			SmallBlind.getComponent<PositionComponent>().x(242);
			SmallBlind.getComponent<PositionComponent>().y(134);
			return true;
		}
	}
	else if (Blind == 0)
	{
		xB += deltaTime * animRate;
		yB += deltaTime * animRate / 2;
		if (xB <= 242)
		{
			BigBlind.getComponent<PositionComponent>().x(xB);
		}
		else {
			con[0] = true;
		}

		if (yB <= 134)
		{
			BigBlind.getComponent<PositionComponent>().y(yB);
		}
		else {
			con[1] = true;
		}

		xS -= deltaTime * animRate / 2;
		yS -= deltaTime * animRate;
		if (xS >= 122)
		{
			SmallBlind.getComponent<PositionComponent>().x(xS);
		}
		else {
			con[2] = true;
		}
		if (yS >= 1)
		{
			SmallBlind.getComponent<PositionComponent>().y(yS);
		}
		else {
			con[3] = true;
		}

		if (con[0] && con[1] && con[2] && con[3]) { //Setting final position of the blinds in case the float numbers exceed the desired position
			BigBlind.getComponent<PositionComponent>().x(242);
			BigBlind.getComponent<PositionComponent>().y(135);
			SmallBlind.getComponent<PositionComponent>().x(122);
			SmallBlind.getComponent<PositionComponent>().y(1);
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
			BigBlind.getComponent<SpriteComponent>().changeSprite(int(Turn_spirte), 0);
			SmallBlind.getComponent<SpriteComponent>().changeSprite(int(Turn_spirte), 0);
		}
		else {
			BigBlind.getComponent<SpriteComponent>().changeSprite(int(Turn_spirte), 1);
			SmallBlind.getComponent<SpriteComponent>().changeSprite(int(Turn_spirte), 1);
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