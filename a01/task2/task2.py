import numpy as np
import matplotlib.pyplot as plt

computer_dat = np.genfromtxt("COMPUTER.dat", delimiter=',')
#print(computer_dat)

fig1 = plt.figure()
ax1 = fig1.add_subplot()
ax1.bar(computer_dat[:,0], computer_dat[:,1])
ax1.set_xlabel("Year")
ax1.set_ylabel("Transistor Count")
# We can use the log-scale for this plot, as the transistor count rises exponentially. This does not apply to the Computer perfomance.
ax1.set_yscale("log")

fig2 = plt.figure()
ax2 = fig2.add_subplot()
ax2.bar(computer_dat[:,0], computer_dat[:,2])
ax2.set_xlabel("Year")
ax2.set_ylabel("Computer perfomance")

plt.show()

fig1.savefig('transistor.png')
fig2.savefig('perfomance.png')
