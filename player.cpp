/******************************
 * Andrew Kang and Robert Eng
 * Othello CS2 Final Project
 ******************************/

#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    self = side;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

int heuristic[8][8] = 
    {
        { 3, -2, 2, 2, 2, 2, -2,  3},
        {-2, -3, 1, 1, 1, 1, -3, -2},
        { 2,  1, 1, 1, 1, 1,  1,  2},
        { 2,  1, 1, 1, 1, 1,  1,  2},
        { 2,  1, 1, 1, 1, 1,  1,  2},
        { 2,  1, 1, 1, 1, 1,  1,  2},
        {-2, -3, 1, 1, 1, 1, -3, -2},
        { 3, -2, 2, 2, 2, 2, -2,  3}        
    };

int Player::moveScore(Move *toMove) {
    Board *tempBoard = board.copy();
    *tempBoard.doMove(toMove, self);

    int boardScore = 0;

    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            if(tempBoard.occupied(x, y) == self) {
                boardScore += 
            }
            else if(tempBoard.occupied(x, y) !=)
        }
    }

    delete tempBoard;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    // Do opponents move to update our local version
    board.doMove(opponentsMove, (Side) !self);

    Move *bestMove;
    int bestScore;

    // Loop through all available spaces and check if there is a move.
    // If there is a valid move, do it immediately bc we're making a stupid AI
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            Move *m = new Move(x, y);

            if(board.checkMove(m, self)) {
                board.doMove(m, self);
                return m;
            }

            delete m;
        }
    }

    return NULL;
}
