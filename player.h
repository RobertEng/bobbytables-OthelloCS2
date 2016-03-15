#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    Player(Side side, Board board);
    ~Player();

    int heuristicScore(Board *tempBoard);
    Move *heuristicMove(Board *tempBoard);
    int minimaxScore(Board *tempBoard, int curLayer, Side side);
    Move *minimax(Board *tempBoard, int numLayers);
    int alphabetaScore(Board *tempBoard, int curLayer, Side side, 
                                    Move *bestMove, int alpha, int beta);
    Move * alphabeta(Board *tempBoard, int numLayers);
    // int alphabetaScore(Board *tempBoard, int curLayer, Side side);
    // Move * alphabeta(Board *tempBoard, int numLayers);

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

private:
    Board board;
    Side self;
};

#endif
