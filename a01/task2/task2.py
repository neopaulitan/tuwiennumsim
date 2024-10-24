import numpy as np
import matplotlib.pyplot as plt

plot_log = True

computer_dat = np.genfromtxt("COMPUTER.dat", delimiter=',')
#print(computer_dat)

fig1 = plt.figure()
ax1 = fig1.add_subplot()
ax1.bar(computer_dat[:,0], computer_dat[:,1])
ax1.set_xlabel("Year")
ax1.set_ylabel("Transistor Count")

if plot_log:
	ax1.set_yscale("log")
else:
	ax1.set_yscale("linear")

fig2 = plt.figure()
ax2 = fig2.add_subplot()
ax2.bar(computer_dat[:,0], computer_dat[:,2])
ax2.set_xlabel("Year")
ax2.set_ylabel("Computer perfomance")

plt.show()

fig1.savefig('transistor.png')
fig2.savefig('perfomance.png')
