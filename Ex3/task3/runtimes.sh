#!/bin/bash

# Define the functions and thread counts
functions=("sinx" "cos2xinv" "x4m5")
num_threads=(1 5 10 20 40 80)

# Output file to store runtimes 
output_file="runtimes.csv"
echo "Function,Threads,Runtime(s)" > $output_file

for func in "${functions[@]}"; do
    for threads in "${num_threads[@]}"; do
        # Set the environment variable
        export OMP_NUM_THREADS=$threads
        
        output=$(./mc_int $func -1.57079632679 1.57079632679 1e7)
        
        # Extract the runtime from the output
        runtime=$(echo "$output" | grep "Elapsed time:" | awk -F':' '{print $2}' | tr -d 's ')
        echo "Function: $func, Threads: $threads, Runtime: $runtime s"
        echo "$func,$threads,$runtime" >> $output_file
    done
done
