#!/bin/bash
#SBATCH --job-name=PAULKANGRUN
#SBATCH -c 1
# Number of cores
#SBATCH --time=01:10:00
# Runtime of this jobs is less then 10 minutes
#            (hh:mm:ss)


# Clear the environment
module purge > /dev/null 2>&1
# Set OMP_NUM_THREADS to the same value as -c
# export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

threadcount=(1 2 4 8 10 20 40)

for threads in "${threadcount[@]}"; do
    export OMP_NUM_THREADS=$threads

    ./mainD thing1 4096 1000 > "dyn4096_${threads}.txt" 
done




