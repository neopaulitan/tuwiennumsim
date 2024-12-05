import numpy as np
import os
import matplotlib.pyplot as plt
import csv

iter = [10, 100, 1000, 10000, 50000]
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

#fig1, (ax1,ax2) = plt.subplots(2, figsize = (6,12))
fig1, (ax1,ax2) = plt.subplots(2, 1)
ax1.scatter(residtabs[:,0], residtabs[:,1])
ax2.scatter(residtabs[:,0], residtabs[:,1])
ax2.set_xlabel("Number of Iterations (log)")
ax1.set_xscale("log")
ax2.set_xscale("log")
ax2.set_ylabel("2-Norm Residuals")
ax1.set_title("Plot of 2-Norm by Iterations")
ax1.set_ylim(0.0058, 0.0065)
ax2.set_ylim(-0.5E-13, 1E-13)

# hide the spines between ax and ax2
ax1.spines.bottom.set_visible(False)
ax2.spines.top.set_visible(False)
ax1.xaxis.tick_top()
ax1.tick_params(labeltop=False)  # don't put tick labels at the top
ax2.xaxis.tick_bottom()
d = .5  # proportion of vertical to horizontal extent of the slanted line
kwargs = dict(marker=[(-1, -d), (1, d)], markersize=12,
              linestyle="none", color='k', mec='k', mew=1, clip_on=False)
ax1.plot([0, 1], [0, 0], transform=ax1.transAxes, **kwargs)
ax2.plot([0, 1], [1, 1], transform=ax2.transAxes, **kwargs)

fig1.savefig("2normplot.png")

fig2, (ax1, ax2) = plt.subplots(2, 1)
ax1.scatter(residtabs[:,0], residtabs[:,2])
ax2.scatter(residtabs[:,0], residtabs[:,2])
ax2.set_xlabel("Number of Iterations")
ax1.set_ylabel("Inf-Norm Residuals")
ax1.set_title("Plot of Inf-Norm by Iterations")
ax1.set_xscale("log")
ax2.set_xscale("log")
ax2.set_ylabel("Inf-Norm Residuals")
ax1.set_ylim(0.0002, 0.0003)
ax2.set_ylim(-2E-15, 8E-15)
# hide the spines between ax and ax2
ax1.spines.bottom.set_visible(False)
ax2.spines.top.set_visible(False)
ax1.xaxis.tick_top()
ax1.tick_params(labeltop=False)  # don't put tick labels at the top
ax2.xaxis.tick_bottom()
d = .5  # proportion of vertical to horizontal extent of the slanted line
kwargs = dict(marker=[(-1, -d), (1, d)], markersize=12,
              linestyle="none", color='k', mec='k', mew=1, clip_on=False)
ax1.plot([0, 1], [0, 0], transform=ax1.transAxes, **kwargs)
ax2.plot([0, 1], [1, 1], transform=ax2.transAxes, **kwargs)


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
plot_and_save("iternum_50000.csv")

plt.show()
