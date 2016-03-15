#!/home/akang/Programs/anaconda2/bin/python

import sys
import numpy as np
from sklearn.ensemble import RandomForestRegressor

####################
### PREPROCESSING
####################

# Load data
data_X = np.loadtxt("dataset.txt", dtype="str", delimiter=" ", usecols=(0,))
data_Y = np.loadtxt("dataset.txt", dtype="int", delimiter=" ", usecols=(1, 2))

X = np.array([[int(cell) for cell in list(board)] for board in data_X])
Y = np.array([8 * move[0] + move[1] for move in data_Y])

# Train AI
BobbyTables = []
for i in range(64):
    cell_clf = RandomForestRegressor()
    cell_Y = np.array([1 if i == move else 0 for move in Y])
    cell_clf.fit(X, cell_Y)
    BobbyTables.append(cell_clf)

# Debugging log
debug = open("debug_log.txt", "w")

####################
### FUNCTIONS
####################

# Board functions
def init_board():
    board = [0] * 64
    update(board, 3, 3, 1)
    update(board, 4, 4, 1)
    update(board, 3, 4, 2)
    update(board, 4, 3, 2)
    return board

def on_board(x, y):
    if (x < 0) or (x >= 8) or (y < 0) or (y >= 8):
        return False
    return True

def access(board, x, y):
    return board[x * 8 + y]

def update(board, x, y, side):
    if (x, y) != -1:
        board[x * 8 + y] = side

# Move functions
def valid_move(board, X, Y, side, opp_side):
    for dx in range(-1, 2):
        for dy in range (-1, 2):
            if dx == 0 and dy == 0:
                continue

            x = X + dx
            y = Y + dy

            if on_board(x, y) and access(board, x, y) == opp_side:
                x += dx
                y += dy
                while on_board(x, y) and access(board, x, y) == opp_side:
                    x += dx
                    y += dy

                if on_board(x, y) and access(board, x, y) == side:
                    return True

    return False

def make_move(board, probs, side, opp_side):
    best_moves = np.argsort(probs)[::-1]
    #print >> debug, best_moves
    #for move in best_moves:
    #    debug.write("%f\n" % probs[move])
    #    debug.write("%d\n" % move)

    #print >> debug, "START\n"
    #print >> debug, probs 
    #print >> debug, "OK\n"
    #print >> debug, best_moves
    for move in best_moves:
        x = move / 8
        y = move % 8
        if valid_move(board, x, y, side, opp_side):
            return (x, y)

    #print >> debug, "END\n"
    return (-1, -1)

def ai_turn(board, ai_side):
    return [0 if cell == 0 else 1 if cell == ai_side else 2 for cell in board]

# Play functions
def sides(state):
    if state == "White":
        side = 1
        opp_side = 2
    else:
        side = 2
        opp_side = 1

    return side, opp_side

def play(board, ai_side, opp_side):
    start = True
    sys.stdout.write("Init done\n")
    sys.stdout.flush()

    while True:
        debug.write("OK1\n")
        if (start == True and opp_side == 1) or (start == False):
            input_move = sys.stdin.readline()
            debug.write("OKwhatever\n")
            opp_x, opp_y = tuple([int(coord) for coord
                                  in input_move.split()][:2])
            update(board, opp_x, opp_y, opp_side)
        debug.write("OK2\n")

        move_probs = [cell_clf.predict([ai_turn(board, ai_side)])[0]
                      for cell_clf in BobbyTables]
        ai_x, ai_y = make_move(board, move_probs, ai_side, opp_side)

        debug.write("%d %d\n" % (ai_x, ai_y))

        update(board, ai_x, ai_y, ai_side)

        sys.stdout.write("%d %d\n" % (ai_x, ai_y))
        sys.stdout.flush()
        debug.write("OK0\n")

        start = False

####################
### GAMEPLAY
####################
board = init_board()
ai_side, opp_side = sides(sys.argv[1])
play(board, ai_side, opp_side)
debug.close()