#!/bin/bash

# N_array=(0.064e3 0.128e3 0.256e3 0.5e3 0.512e3 1e3 1.024e3 1.5e3)
# N_array=(0.05e3 0.1e3 0.15e3 0.2e3 0.25e3 0.3e3 0.35e3 0.4e3 0.45e3 0.5e3 0.55e3 0.6e3 0.65e3 0.7e3 0.75e3 0.8e3 0.85e3 0.9e3 0.95e3 1e3)
N_array=(50 100 150 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950 1000)
modes_array=("CUSTOM" "BLAS" "EIGEN")

echo "CUSTOM, BLAS, EIGEN" > out.csv 
for N in "${N_array[@]}"; do 
  for mode in "${modes_array[@]}"; do
    echo -n "$(./mmm $mode $N)," >> out.csv
  done
  echo "" >> out.csv
done
