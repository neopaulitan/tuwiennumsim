import numpy as np
import os
import matplotlib.pyplot as plt
import csv

threadcount = [1, 2, 4, 8, 10, 20, 40]
staticrun1 = np.zeros(7)
sonerun1 = np.zeros(7)
dynrun1 = np.zeros(7)
staticrun2 = np.zeros(7)
sonerun2 = np.zeros(7)
dynrun2 = np.zeros(7)

for i in range(0,7):
	fn1a = f"static2048_{threadcount[i]}.txt"
	fn1b = f"static4096_{threadcount[i]}.txt"
	fn2a = f"sone2048_{threadcount[i]}.txt"
	fn2b = f"sone4096_{threadcount[i]}.txt"
	fn3a = f"dyn2048_{threadcount[i]}.txt"
	fn3b = f"dyn4096_{threadcount[i]}.txt"
	with open(fn1a, 'r') as file:
		for j, line in enumerate(file, start = 1): 
			if j == 4:
				print(line)
				number = line.strip().split()[-2]
				print(number)
				staticrun1[i] = float(number)
				
	with open(fn1b, 'r') as file:
		for j, line in enumerate(file, start = 1): 
			if j == 4:
				number = line.strip().split()[-2]
				staticrun2[i] = float(number)
				
	with open(fn2a, 'r') as file:
		for j, line in enumerate(file, start = 1): 
			if j == 4:
				number = line.strip().split()[-2]
				sonerun1[i] = float(number)
				
	with open(fn2b, 'r') as file:
		for j, line in enumerate(file, start = 1): 
			if j == 4:
				number = line.strip().split()[-2]
				sonerun2[i] = float(number)
				
	with open(fn3a, 'r') as file:
		for j, line in enumerate(file, start = 1): 
			if j == 4:
				number = line.strip().split()[-2]
				dynrun1[i] = float(number)
				
	with open(fn3b, 'r') as file:
		for j, line in enumerate(file, start = 1): 
			if j == 4:
				number = line.strip().split()[-2]
				dynrun2[i] = float(number)

fig1, (ax1, ax2) = plt.subplots(2, 1, sharex=True)
ax1.scatter(threadcount, staticrun1/4000, label = 'res = 2048')
ax1.scatter(threadcount, staticrun2/4000, label = 'res = 4096') 
ax2.scatter(threadcount, staticrun1/4000, label = 'res = 2048')
ax2.scatter(threadcount, staticrun2/4000, label = 'res = 4096') 
ax1.spines['bottom'].set_visible(False)
ax2.spines['top'].set_visible(False)
ax2.tick_params(labeltop=False) 
ax2.set_ylim(75000/4000, 77000/4000)
ax1.set_ylim(290000/4000, 300000/4000)
ax2.set_xlabel("Thread Count")
ax2.set_ylabel("Runtime (s)")
ax2.set_xticks(threadcount)
fig1.suptitle("Runtime using 'schedule(static)' for increasing thread count")
ax1.legend()

fig1.savefig("staticrun.png")

fig2, (ax1,ax2) = plt.subplots(2, 1, sharex=True)
ax1.scatter(threadcount, sonerun1/4000, label = 'res = 2048')
ax1.scatter(threadcount, sonerun2/4000, label = 'res = 4096') 
ax2.scatter(threadcount, sonerun1/4000, label = 'res = 2048')
ax2.scatter(threadcount, sonerun2/4000, label = 'res = 4096') 
ax1.spines['bottom'].set_visible(False)
ax2.spines['top'].set_visible(False)
ax2.tick_params(labeltop=False) 
ax2.set_ylim(75000/4000, 77000/4000)
ax1.set_ylim(290000/4000, 300000/4000)
ax2.set_xlabel("Thread Count")
ax2.set_ylabel("Runtime (s)")
ax2.set_xticks(threadcount)
fig2.suptitle("Runtime using 'schedule(static,1)' for increasing thread count")
ax1.legend()

fig2.savefig("sonerun.png")

fig3, (ax1,ax2) = plt.subplots(2, 1, sharex=True)
ax1.scatter(threadcount, dynrun1/4000, label = 'res = 2048')
ax1.scatter(threadcount, dynrun2/4000, label = 'res = 4096') 
ax2.scatter(threadcount, dynrun1/4000, label = 'res = 2048')
ax2.scatter(threadcount, dynrun2/4000, label = 'res = 4096') 
ax1.spines['bottom'].set_visible(False)
ax2.spines['top'].set_visible(False)
ax2.tick_params(labeltop=False) 
ax2.set_ylim(75000/4000, 77000/4000)
ax1.set_ylim(290000/4000, 300000/4000)
ax2.set_xlabel("Thread Count")
ax2.set_ylabel("Runtime (s)")
ax2.set_xticks(threadcount)
fig3.suptitle("Runtime using 'schedule(dynamic)' for increasing thread count")
ax1.legend()

fig3.savefig("dynrun.png")

print(staticrun1)
print(staticrun2)

plt.show()
