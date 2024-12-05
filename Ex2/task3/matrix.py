import csv
import matplotlib.pyplot as plt
import numpy as np

def generate_heatmap(csv_file, N, output_image='matrix.png'):
    data = []
    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            if row:  
            	data.append([float(value) if value else 0 for value in row[:N]])

    data = data[:N]  
    while len(data) < N:
        data.append([0] * N) 
        
    matrix = np.array(data)
    plt.figure(figsize=(10, 8))
    plt.imshow(matrix, cmap='viridis', aspect='auto')
    plt.colorbar()  
    plt.title(f'Visualization of "A" Matrix', fontsize=16)
    plt.xlabel('Column Index')
    plt.ylabel('Row Index')
    
    plt.savefig(output_image, dpi=300)
    plt.show()

N = 10**2  
csv_file_path = 'A.csv'  
generate_heatmap(csv_file_path, N)

