import numpy as np
import os
import matplotlib.pyplot as plt
import subprocess

numrun = 30
runs = np.zeros((numrun,4))

for i in range(4):
	for j in range(numrun):
		runs[j][i] = subprocess.run([f"./task4_{i}"], capture_output = True).stdout

avgs = [np.mean(runs[:,i]) for i in range(4)]
vari = [np.var(runs[:,i]) for i in range(4)]
titles = ["-O0", "-O1", "-O2", "-O3"]

fig1, (ax1,ax2) = plt.subplots(2, figsize = (6,12))
ax1.scatter(titles, avgs)
ax1.set_xlabel("Optimization Flags")
ax1.set_ylabel("Mean Run Time (microsecond)")

ax2.scatter(titles, vari)
ax2.set_xlabel("Optimization Flags")
ax2.set_ylabel("Variance in Run Time (microsecond)")
plt.show()

fig1.savefig("optimization_flag.png")
