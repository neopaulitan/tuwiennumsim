#!/bin/python3
import csv
import matplotlib.pyplot as plt

k = []
diag = []
ic = []

with open("out.csv", mode = "r") as file:
   reader = csv.reader(file)
   header = next(reader)
   for row in reader:
       k.append(float(row[0]))
       diag.append(float(row[1]))
       ic.append(float(row[2]))

plt.plot(k[0:50], diag[0:50], label = "DiagonalPreconditioner")
plt.plot(k[0:50], ic[0:50], label = "IncompleteCholesky")
plt.xlabel("k")
plt.ylabel("r_k")
plt.grid()
plt.legend()
plt.yscale("log")
plt.show()
