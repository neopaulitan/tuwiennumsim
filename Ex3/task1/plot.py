import matplotlib.pyplot as plt
import numpy as np

def read_data(filename):
    x = []
    y = []
    with open(filename, 'r') as file:
        for line in file:
            if line.strip():  # Skip empty lines
                key, value = line.split(':')  # Split by colon
                x.append(float(key.strip()))  # Add x value
                y.append(float(value.strip()))  # Add y value
    return x, y

x1, y1 = read_data('res_eor.txt')
x2, y2 = read_data('Anorm_eor.txt')
# convergence tendency
coef1 = np.polyfit(np.log10(x1), np.log10(y1), 1)
line1 = np.poly1d(coef1)
coef2 = np.polyfit(np.log10(x2), np.log10(y2), 1)
line2 = np.poly1d(coef2)

plt.figure(figsize=(16, 12))

plt.plot(x1, y1, 'o', color='blue', linewidth=0.5, label='res_eor_data')
plt.plot(x2, y2, 'o-', color='red', linewidth=0.5, label='Anorm_eor_data')
x_range = np.linspace(min(x1), max(x1), 500)
plt.plot(x_range, 10**line1(np.log10(x_range)), 'blue', linestyle='--', label='_nolegend_')
# plt.plot(x_range, 10**line2(np.log10(x_range)), 'blue', linestyle='--', label='Trendline 2')
for i in range(len(x1)):
    plt.text(x1[i], y1[i]*2, f'{y1[i]:.2e}', fontsize=5, color='blue', ha='center')
for i in range(len(x2)):
    plt.text(x2[i], y2[i]*2, f'{y2[i]:.2e}', fontsize=5, color='red', ha='center')
plt.xscale('log')  # Log scale for x-axis
plt.yscale('log')  # Log scale for y-axis
plt.xlabel('iterations (log scale)', fontsize=12)
plt.ylabel('error values (log scale)', fontsize=12)
plt.title('errors', fontsize=14)
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.legend()

# plt.show()
plt.savefig('plot.png', dpi=300)