#pragma once

class Flags {
public:
    bool StartGame = true;
    bool GameEnded = false;
    bool EnemyMove = false;
    bool BlindsCon = false; //Condition to the action related to blinds

    //flags for displaying cards on the table
    bool Show3Cards = false;
    bool Show4Card = false;
    bool Show5Card = false;

    //flags deciding what game stage to load
    bool firstBet = true;
    bool secondBet = false;
    bool thirdBet = false;
    bool fourthBet = false;
    bool endRound = false;

    //Animation flags
    


};