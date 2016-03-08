#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    int Player::moveScore(Move *toMove);    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

private:
    Board board;
    Side self;
};

#endif
