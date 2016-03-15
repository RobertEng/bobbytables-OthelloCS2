/******************************
 * Andrew Kang and Robert Eng
 * Othello CS2 Final Project
 ******************************/

#include "player.h"
#define BAD_SCORE -1000
#include <iostream>
#include <fstream>
using namespace std;

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
 * Constructor for the player with a given board.
 */
Player::Player(Side side, Board givenBoard) {
    testingMinimax = false;
    self = side;
    board = givenBoard;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

// Array that indicates heuristic scoring of each cell.
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

/*
 * Scores a given move on the current board using the heuristic.
 */
int Player::heuristicScore(Board *tempBoard) {
    // Do given move on a board copy.
    int boardScore = 0;

    // Iterate over all cells.
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            // Add score if AI occupies cell.
            boardScore += tempBoard->get(self, x, y) * heuristic[y][x];
            // Subtract score if opponent occupies cell.
            boardScore -= tempBoard->get((Side) !self, x, y) * heuristic[y][x];
        }
    }

    return boardScore;
}

Move *Player::heuristicMove(Board *tempBoard) {
    Move *bestMove = NULL;
    int bestScore = BAD_SCORE; // Initialize to unachievably bad score.

    // Iterate over all possible moves.
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            Move *m = new Move(x, y);

            if(board.checkMove(m, self)) {
                // Do move m on a board copy.
                Board *tempBoard = board.copy();
                tempBoard->doMove(m, self);
                // Score the resulting board.
                int curScore = heuristicScore(tempBoard);

                if (curScore > bestScore) {
                    delete bestMove;
                    bestMove = m;
                    bestScore = curScore;
                } else {
                    delete m;
                }

                delete tempBoard;
            } else {
                delete m;
            }
        }
    }

    return bestMove;
}

/*
 * Scores a given move on the current board using minimax.
 */
int Player::minimaxScore(Board *tempBoard, int curLayer, Side side) {
    // At the leaves of the minimax tree, return the board score.
    if (curLayer == 0) {
        int boardScore = 0;

        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 8; y++) {
                // Add AI cells, subtract opponent cells.
                boardScore += tempBoard->get(self, x, y)
                              - tempBoard->get((Side) !self, x, y);
            }
        }

        // Prints flipped board for debugging purposes.

        /*
        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 8; y++) {
                if (tempBoard->get((Side) !self, x, y) == 1)
                    cerr << 2;
                else if (tempBoard->get(self, x, y) == 1)
                    cerr << 1;
                else
                    cerr << 0;
            }
            cerr << endl;
        }
        cerr << endl;
        */

        return boardScore;
    } else {
        // If the opponent is the current player at the node, take the minimum
        // score of the child nodes; otherwise, take the maximum score.

        int bestScore = BAD_SCORE; // Initialize to unachievably bad score.

        // Iterate over all possible moves.
        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 8; y++) {
                Move *m = new Move(x, y);

                if(tempBoard->checkMove(m, side)) {
                    // Do move m on a board copy.
                    Board *tempBoard2 = tempBoard->copy();
                    tempBoard2->doMove(m, side);
                    // Score the resulting board.
                    int curScore = minimaxScore(tempBoard2, curLayer - 1,
                                                (Side) !side);

                    if (bestScore == BAD_SCORE) {
                        bestScore = curScore;
                    }

                    // Checks either that it is the opponent's turn and the
                    // current score is less than the minimum score, or that
                    // it is the AI's turn and the current score is greater
                    // than the maximum score.
                    if ((bestScore == BAD_SCORE)
                        || (side != self && curScore < bestScore)
                        || (side == self && curScore > bestScore)) {
                        bestScore = curScore;
                    }

                    delete tempBoard2;
                }

                delete m;
            }
        }

        return bestScore;        
    }

}

/*
 * Finds the best move on the current board using minimax.
 */
Move *Player::minimax(Board *tempBoard, int numLayers) {
    Move *bestMove = NULL;
    int bestScore = BAD_SCORE; // Initialize to unachievably bad score.

    // Iterate over all possible moves.
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            Move *m = new Move(x, y);

            if(tempBoard->checkMove(m, self)) {
                // Do move m on a board copy if it is a valid move.
                Board *tempBoard2 = tempBoard->copy();
                tempBoard2->doMove(m, self);
                // Score the resulting board.
                int curScore = minimaxScore(tempBoard2, numLayers - 1,
                                            (Side) !self);

                if (curScore > bestScore) {
                    // If the current score is greater than the best score so
                    // far, set the best move to move m.
                    delete bestMove;
                    bestMove = m;
                    bestScore = curScore;
                } else {
                    delete m;
                }

                delete tempBoard2;
            } else {
                delete m;
            }
        }
    }

    return bestMove;
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
    // Update local board with opponent's move
    board.doMove(opponentsMove, (Side) !self);

    // For neural network training.
    ofstream outfile;
    outfile.open("ml/dataset.txt", ios::app);

    if (opponentsMove != NULL) {
        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 8; y++) {
                if (board.get((Side) !self, x, y) == 1)
                    outfile << 1;
                else if (board.get(self, x, y) == 1)
                    outfile << 2;
                else
                    outfile << 0;
            }
        }
        outfile << " ";

        outfile << opponentsMove->x << " " << opponentsMove->y << endl;
    }
    // End neural network portion.

    Move *bestMove = NULL;
    Board *tempBoard = board.copy();

    // Using the below line instead of the if/else clause allows doMove to use
    // heuristic scoring instead of minimax.

    // bestMove = heuristicMove(tempBoard);

    if(testingMinimax) {
        bestMove = minimax(tempBoard, 2);
    } else {
        bestMove = minimax(tempBoard, 4);
    }

    // For neural network training.
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            if (board.get((Side) !self, x, y) == 1)
                outfile << 2;
            else if (board.get(self, x, y) == 1)
                outfile << 1;
            else
                outfile << 0;
        }
    }
    outfile << " ";

    outfile << bestMove->x << " " << bestMove->y << endl;

    outfile.close();
    // End neural network portion.

    board.doMove(bestMove, self);
    return bestMove;
}
