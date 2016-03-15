#!/home/akang/Programs/anaconda2/bin/python

import sys
import numpy as np
from string import maketrans
from sklearn.ensemble import RandomForestRegressor
import pickle

####################
### FUNCTIONS
####################

# Board functions
def init_board():
    board = [0] * 64
    set(board, 3, 3, 1)
    set(board, 4, 4, 1)
    set(board, 3, 4, 2)
    set(board, 4, 3, 2)
    return board

def on_board(x, y):
    if (x < 0) or (x >= 8) or (y < 0) or (y >= 8):
        return False
    return True

def access(board, x, y):
    return board[x * 8 + y]

def set(board, x, y, side):
    board[x * 8 + y] = side

def update(board, X, Y, side, opp_side):
    if (X, Y) != (-1, -1):
        board[X * 8 + Y] = side

        for dx in range(-1, 2):
            for dy in range (-1, 2):
                if dx == 0 and dy == 0:
                    continue

                x = X + dx
                y = Y + dy

                while on_board(x, y) and access(board, x, y) == opp_side:
                    x += dx
                    y += dy

                if on_board(x, y) and access(board, x, y) == side:
                    x = X + dx
                    y = Y + dy

                    while on_board(x, y) and access(board, x, y) == opp_side:
                        set(board, x, y, side)
                        x += dx
                        y += dy

# Move functions
def valid_move(board, X, Y, side, opp_side):
    if access(board, X, Y) == 0:
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

    for move in best_moves:
        x = move / 8
        y = move % 8
        if valid_move(board, x, y, side, opp_side):
            return (x, y)

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
        if (start == True and opp_side == 1) or (start == False):
            input_move = sys.stdin.readline()
            opp_x, opp_y = tuple([int(coord) for coord
                                  in input_move.split()][:2])
            update(board, opp_x, opp_y, opp_side, ai_side)

        move_probs = [cell_clf.predict([ai_turn(board, ai_side)])[0]
                      for cell_clf in BobbyTables]
        ai_x, ai_y = make_move(board, move_probs, ai_side, opp_side)
        update(board, ai_x, ai_y, ai_side, opp_side)

        sys.stdout.write("%d %d\n" % (ai_x, ai_y))
        sys.stdout.flush()

        start = False

####################
### PREPROCESSING
####################

data = [line.strip() for line in
        np.loadtxt("BobbyTables_reduced.txt", dtype="str")]
trantab = maketrans("abcdefgh", "12345678")
data = [line.translate(trantab, '",') for line in data]

def game_converter(sequence):
    X = []
    Y = []
    curr_board = init_board()

    for i in range(len(sequence) / 2):
        x = int(sequence[2 * i]) - 1
        y = int(sequence[2 * i + 1]) - 1

        X.append(curr_board[:])
        Y.append(8 * x + y)
        
        if i % 2 == 0:
            update(curr_board, x, y, 2, 1)
        else:
            update(curr_board, x, y, 1, 2)

    return X, Y

X = []
Y = []
for sequence in data:
    converted = game_converter(sequence)
    X.extend(converted[0])
    Y.extend(converted[1])

# Train AI
BobbyTables = []
for i in range(64):
    cell_clf = RandomForestRegressor()
    cell_Y = np.array([1 if i == move else 0 for move in Y])
    cell_clf.fit(X, cell_Y)

    BobbyTables.append(cell_clf)

####################
### GAMEPLAY
####################
if __name__ == "__main__":
    debug = open("debug_log.txt", "w")

    '''
    # Load AI
    BobbyTables = []
    for i in range(64):
        clf_file = open("BobbyTables_datasets/BobbyTables%d" % i, "r")
        cell_clf = pickle.load(clf_file)
        clf_file.close()

        BobbyTables.append(cell_clf)
        debug.write("%d\n" % i)
    '''

    board = init_board()
    ai_side, opp_side = sides(sys.argv[1])
    play(board, ai_side, opp_side)

    debug.close()