import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
csv_file = "runtimes.csv"
data = pd.read_csv(csv_file)

# Extract the unique functions
functions = data["Function"].unique()

# Plot the runtime for each function
plt.figure(figsize=(10, 6))

for func in functions:
    func_data = data[data["Function"] == func]
    plt.plot(
        func_data["Threads"], 
        func_data["Runtime(s)"], 
        marker="o", 
        label=func
    )

# Customize the plot
plt.title("Runtime vs Threads for Different Functions")
plt.xlabel("Number of Threads")
plt.ylabel("Runtime (s)")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend(title="Functions")
plt.tight_layout()

# Save the plot
output_plot = "runtimes_plot.png"
plt.savefig(output_plot)
print(f"Plot saved as {output_plot}")

# Show the plot
plt.show()
