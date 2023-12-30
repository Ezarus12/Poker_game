#pragma once
#include <random>

int card_x = 76; // X coordinates of the first card placed on the table
int card_y = 76; // X coordinates of the first card placed on the table
int card_spacing = 34; // X spacing between cards placed on the table

int r_scale = 1;

int x, y = 0;

const int players_num = 2; // Number of individual hands to draw

int Random(int x, int y) { //Generating random number from x to y
    std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(x, y);

    return distrib(gen);
}