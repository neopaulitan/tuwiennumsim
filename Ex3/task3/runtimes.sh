#!/bin/bash

# Define the functions and thread counts
functions=("sinx" "cos2xinv" "x4m5")
num_threads=(1 5 10 20 40 80)

# Output file to store runtimes (optional, remove if not needed)
output_file="runtimes.csv"
echo "Function,Threads,Runtime(s)" > $output_file

# Loop through each function
for func in "${functions[@]}"; do
    # Loop through each thread count
    for threads in "${num_threads[@]}"; do
        # Set the environment variable
        export OMP_NUM_THREADS=$threads
        
        # Run the mc_int program and capture the output
        output=$(./mc_int $func -1.57079632679 1.57079632679 1e7)
        
        # Extract the runtime from the output
        runtime=$(echo "$output" | grep "Elapsed time:" | awk -F':' '{print $2}' | tr -d 's ')
        
        # Print the runtime to the terminal
        echo "Function: $func, Threads: $threads, Runtime: $runtime s"
        
        # Save the results to the output file
        echo "$func,$threads,$runtime" >> $output_file
    done
done
