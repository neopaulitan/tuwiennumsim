import numpy as np
import matplotlib.pyplot as plt

flop_data = np.loadtxt('triad_data.txt')
#print(flop_data)
plt.loglog(flop_data[:,0], (2*flop_data[:,0]/flop_data[:,1]))
plt.show()
