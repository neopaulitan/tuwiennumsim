#!/bin/bash
# Number of cores
#SBATCH -c 20
# Runtime of this jobs is less then 10 minutes
#            (hh:mm:ss)
#sbatch --time=00:10:00
# Clear the environment
module purge > /dev/null 2>&1
# Set OMP_NUM_THREADS to the same value as -c
#export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
# You can start several programs with one script file/submission
#./stenciljacobi 256 10000
#./stenciljacobi 512 10000
#./stenciljacobi 1024 10000
./plotting.sh
#./mc_int sinx -1.57079632679 1.57079632679 1e7
