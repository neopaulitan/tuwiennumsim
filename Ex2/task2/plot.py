#!/bin/python3

import csv
import matplotlib.pyplot as plt

custom = []
blas = []
eigen = []
calc = []
# N_array = [64,128,256,500,512,1000,1024,1500]
N_array = [50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000]

def t_approx(N):
    return 3 * N**3 / (1.504 * 10**10)

for n in N_array:
    calc.append(t_approx(n))

with open("out.csv", mode = "r") as file:
    reader = csv.reader(file)
    header = next(reader)

    for row in reader:
      custom.append(float(row[0]))
      blas.append(float(row[1]))
      eigen.append(float(row[2]))

plt.plot(N_array, custom, label="custom")
plt.plot(N_array, blas, label="blas")
plt.plot(N_array, eigen, label="eigen")
plt.plot(N_array, calc, label="theoretical value", linestyle="dashed")
plt.plot()
plt.grid()
plt.yscale('log')
plt.xlabel("size N of the matrix")
plt.ylabel("runtime in seconds")
plt.title("runtime of NxN matrix multiplications")
plt.legend()
plt.savefig("plot.png")
plt.show()
