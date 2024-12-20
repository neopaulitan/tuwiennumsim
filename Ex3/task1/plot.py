import matplotlib.pyplot as plt

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

plt.figure(figsize=(8, 6))

plt.plot(x1, y1, 'o-', color='blue', linewidth=0.5, label='res_eor_data')
plt.plot(x2, y2, 'o-', color='red', linewidth=0.5, label='Anorm_eor_data')
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