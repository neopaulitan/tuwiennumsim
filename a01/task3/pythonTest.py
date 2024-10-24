# This is a sanity check to see whether numpy comes to the same result as our cpp code

import numpy as np

def read_matrix_from_file(filename):
    with open(filename, 'r') as file:
        matrix = []
        for line in file:
            row = list(map(float, line.split()))
            matrix.append(row)

    return np.array(matrix)

matrix = read_matrix_from_file('MatrixA.txt')
print(matrix)
print(np.linalg.norm(matrix, ord=1))
