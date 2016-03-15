import numpy as np
from string import maketrans
from sklearn.ensemble import RandomForestRegressor
import pickle
import BobbyTables as bt

# Load data
#data_X = np.loadtxt("new_dataset.txt", dtype="str", delimiter=" ",
#                    usecols=(0,))
#data_Y = np.loadtxt("new_dataset.txt", dtype="int", delimiter=" ",
#                    usecols=(1, 2))

#X = np.array([[int(cell) for cell in list(board)] for board in data_X])
#Y = np.array([8 * move[0] + move[1] for move in data_Y])

data = [line.strip() for line in
        np.loadtxt("BobbyTables_dataset.txt", dtype="str")]
trantab = maketrans("abcdefgh", "12345678")
data = [line.translate(trantab, '",') for line in data]

def game_converter(sequence):
    X = []
    Y = []
    curr_board = bt.init_board()

    for i in range(len(sequence) / 2):
        x = int(sequence[2 * i]) - 1
        y = int(sequence[2 * i + 1]) - 1

        X.append(curr_board[:])
        Y.append(8 * x + y)
        
        if i % 2 == 0:
            bt.update(curr_board, x, y, 2, 1)
        else:
            bt.update(curr_board, x, y, 1, 2)

    return X, Y

X = []
Y = []
for sequence in data:
    converted = game_converter(sequence)
    X.extend(converted[0])
    Y.extend(converted[1])

'''
# Train AI
for i in range(64):
    print "Round %d\n" % i

    cell_clf = RandomForestRegressor()
    cell_Y = np.array([1 if i == move else 0 for move in Y])
    cell_clf.fit(X, cell_Y)

    clf_file = open("BobbyTables_datasets/BobbyTables%d" % i, "w")
    pickle.dump(cell_clf, clf_file)
    clf_file.close()
'''