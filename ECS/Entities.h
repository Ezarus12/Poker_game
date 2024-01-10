#pragma once 

Manager manager;

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

//betting button
auto& BB_center(manager.addEntity());
auto& BB_sub_big(manager.addEntity());
auto& BB_sub_small(manager.addEntity());
auto& BB_add_big(manager.addEntity());
auto& BB_add_small(manager.addEntity());

auto& money_text(manager.addEntity());
auto& enemy_money_text(manager.addEntity());
auto& bet_text(manager.addEntity());
auto& pool_text(manager.addEntity());

auto& BigBlind(manager.addEntity());
auto& BigBlindNote(manager.addEntity());

auto& SmallBlind(manager.addEntity());
auto& SmallBlindNote(manager.addEntity());

auto& CardBorder(manager.addEntity());

//Start button
auto& Start_button(manager.addEntity());

auto& Fold_button(manager.addEntity());

auto& PokerRanking(manager.addEntity());






auto& Mouse(manager.addEntity());
