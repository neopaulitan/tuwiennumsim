import pandas as pd
import matplotlib.pyplot as plt

csv_file = "runtimes.csv"
data = pd.read_csv(csv_file)

functions = data["Function"].unique()

plt.figure(figsize=(10, 6))

for func in functions:
    func_data = data[data["Function"] == func]
    plt.plot(
        func_data["Threads"], 
        func_data["Runtime(s)"], 
        marker="o", 
        label=func
    )

plt.title("Runtime vs Threads for Different Functions")
plt.xlabel("Number of Threads")
plt.ylabel("Runtime (s)")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend(title="Functions")
plt.tight_layout()
plt.savefig("runtimes_plot.png")

plt.show()
