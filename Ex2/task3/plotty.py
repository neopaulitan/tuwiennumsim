import numpy as np
import os
import matplotlib.pyplot as plt

iter = [10, 100, 1000, 100000, 200000, 500000]
residtab = np.zeros(6, 3)

for i in range(0,6):
	file = f"normiter_{iter[i]}.csv"
	csvfile = csv.reader(file)
	data = list(reader)
	residtab[i][0] = data[0]
	residtab[i][1] = data[1]
	residtab[i][2] = data[2]
	file.close()
	
#solndata = (128, 2)
#file = csv.reader("iternum_500000.csv")
#solndata[:,1] = file 
#solndata[:,0] = np.arange(0,1,128)
#file.close()

fig1, (ax1,ax2) = plt.subplots(2, figsize = (6,12))
ax1.scatter(residtabs[:,0], residtabs[:,1])
ax1.set_xlabel("Number of Iterations")
#ax1.set_yscale("log")
ax1.set_ylabel("2-Norm Residuals")
ax1.set_title("2-Norm Residuals x Number of Iterations for N = 128 and Dirichlet and Neumann boundaries = 0 for a Gaussian point source centered at (0.5,0.5) with a deviation of 0.1")

ax2.scatter(residtabs[:,0], residtabs[:,1])
ax2.set_xlabel("Number of Iterations")
ax2.set_ylabel("Inf-Norm Residuals")
ax2.set_title("Inf-Norm Residuals x Number of Iterations for N = 128 and Dirichlet and Neumann boundaries = 0 for a Gaussian point source centered at (0.5,0.5) with a deviation of 0.1")

fig.savefig("resplots.png")

def plot_and_save(filename):
    arr = np.loadtxt(filename, delimiter=" ", dtype=float)
    fig, ax = plt.subplots()
    ax.set(xlabel='x', ylabel='y',title=f'{filename}')
    plt.imshow(arr, cmap='RdBu', extent=[0, 1, 0, 1], origin="lower")
    plt.colorbar()
    fig.savefig(filename + ".png")
    plt.show()

plot_and_save("iternum_500000.csv")

#ax3.scatter(solndata[:,0],solndata[:,1])
#ax3.set_xlabel("x")
#ax3.set_ylabel("u(x,y)")
#ax3.set_title("Generated Solution for Poisson Problem using Jacobi Solver after 500000 iterations")
plt.show()
