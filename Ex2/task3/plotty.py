import numpy as np
import os
import matplotlib.pyplot as plt
import csv

iter = [10, 100, 500, 1000, 5000]
itersize = len(iter)
residtabs = np.zeros((itersize, 3))

for i in range(0,itersize):
	filename = f"normiter_{iter[i]}.csv"
	with open(filename, 'r') as file:
		csvfile = csv.reader(file)
		data = list(csvfile)
		residtabs[i][0] = data[0][0]
		residtabs[i][1] = data[1][0]
		residtabs[i][2] = data[2][0]

numiter = residtabs[:,0]
lognumiter = np.log10(numiter)
twonorm = residtabs[:,1]
infnorm = residtabs[:,2]

coeff1 = np.polyfit(lognumiter, twonorm, 1) 
slope1 = coeff1[0] 
intercept1 = coeff1[1]  

coeff2 = np.polyfit(lognumiter, infnorm, 1) 
slope2 = coeff2[0] 
intercept2 = coeff2[1]  

poly1x = np.linspace(min(lognumiter), max(lognumiter), 10)
poly1y = slope1 * poly1x + intercept1

poly2x = np.linspace(min(lognumiter), max(lognumiter), 10)
poly2y = slope2 * poly2x + intercept2


#fig1, (ax1,ax2) = plt.subplots(2, figsize = (6,12))
fig1, (ax1) = plt.subplots(1, 1)
ax1.scatter(lognumiter, twonorm)
ax1.plot(poly1x, poly1y, color = 'red', label=f'slope = {slope1:.2f}') 
ax1.set_xlabel("Number of Iterations (log)")
#ax1.set_xscale("log")
ax1.set_ylabel("2-Norm Residuals")
ax1.set_title("Plot of 2-Norm by Iterations")
fig1.legend()
#ax1.set_ylim(0.0058, 0.0065)
#ax2.set_ylim(-0.5E-13, 1E-13)

fig1.savefig("2normplot.png")

fig2, (ax1) = plt.subplots(1, 1)
ax1.scatter(lognumiter, infnorm)
ax1.plot(poly2x, poly2y, color = 'red', label=f'slope = {slope1:.2f}') 
ax1.set_xlabel("Number of Iterations (log)")
#ax1.set_xscale("log")
ax1.set_ylabel("Inf-Norm Residuals")
ax1.set_title("Plot of Inf-Norm by Iterations")
fig2.legend()
#ax1.set_ylim(0.0058, 0.0065)
#ax2.set_ylim(-0.5E-13, 1E-13)

fig2.savefig("infnormplot.png")

def plot_and_save(filename):
    arr = np.loadtxt(filename, delimiter=" ", dtype=float)
    fig, ax = plt.subplots()
    ax.set(xlabel='x', ylabel='y',title=f'{filename}')
    plt.imshow(arr, cmap='RdBu', extent=[0, 1, 0, 1], origin="lower")
    plt.colorbar()
    fig.savefig(filename + ".png")
    plt.show()

plot_and_save("iternum_100.csv")
plot_and_save("iternum_5000.csv")

plt.show()
