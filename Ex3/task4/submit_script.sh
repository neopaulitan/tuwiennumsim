#!/bin/bash
#SBATCH --job-name=PAULKANGRUN
#SBATCH -c 1
# Number of cores
#SBATCH --time=00:10:00
# Runtime of this jobs is less then 10 minutes
#            (hh:mm:ss)


# Clear the environment
module purge > /dev/null 2>&1
# Set OMP_NUM_THREADS to the same value as -c
# export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK


for threads in 1 2 4 8 10 20 40; do
    export OMP_NUM_THREADS=$threads

    ./mainS thing 2048 1000 > "static2048_${threads}.txt" 
    ./mainS thing 10000 1000 > "static10k_${threads}.txt" 

    ./mainS1 thing 2048 1000 > "sone2048_${threads}.txt" 
    ./mainS1 thing 10000 1000 > "sone10k_${threads}.txt" 

    ./mainD thing 2048 1000 > "dynamic2048_${threads}.txt" 
    ./mainD thing 10000 1000 > "dynamic10k_${threads}.txt" 

done




