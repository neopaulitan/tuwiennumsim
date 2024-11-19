import numpy as np
import matplotlib.pyplot as plt
import subprocess

def parse_size(size_str):
    size_str = size_str.upper().strip()
    if size_str.endswith("KIB"):
        return int(size_str[:-3]) * 1024
    elif size_str.endswith("MIB"):
        return int(size_str[:-3]) * 1024 ** 2
    elif size_str.endswith("GIB"):
        return int(size_str[:-3]) * 1024 ** 3
    else:
        return int(size_str)

def get_cache_sizes_in_bytes():
    try:
        output = subprocess.run(["lscpu"], capture_output=True, text=True, check=True).stdout
        cache_sizes = {"L1d": None, "L2": None, "L3": None}

        for line in output.splitlines():
            if "L1d cache" in line:
                cache_sizes["L1d"] = parse_size(line.split(":")[1].strip().split("(")[0])
            elif "L2 cache" in line:
                cache_sizes["L2"] = parse_size(line.split(":")[1].strip().split("(")[0])
            elif "L3 cache" in line:
                cache_sizes["L3"] = parse_size(line.split(":")[1].strip().split("(")[0])

        return cache_sizes
    
    except subprocess.CalledProcessError as e:
        print(f"Error executing lscpu: {e}")
        return None



flop_data = np.loadtxt('triad_data.txt')

i = 0
cmap = ['red', 'blue', 'yellow']

plt.semilogx(flop_data[:,0], (2*flop_data[:,0]/flop_data[:,1]))

for line in flop_data:
    print(2*line[0] / flop_data[1])

for cache in get_cache_sizes_in_bytes().items():
    plt.axvline(x=cache[1]/32, color=cmap[i], label=cache[0])
    i += 1

plt.xlabel('N')
plt.ylabel('FLOPs')
plt.legend()
plt.grid()
plt.show()
