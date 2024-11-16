#!/bin/bash

N_array=(64 128 256 500 512 1000 1024 1500)
N_array=(50 100 150 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950 1000)
modes_array=("CUSTOM" "BLAS" "EIGEN")

echo "CUSTOM, BLAS, EIGEN" > out.csv 
for N in "${N_array[@]}"; do 
  for mode in "${modes_array[@]}"; do
    echo -n "$(./mmm $mode $N)," >> out.csv
  done
  echo "" >> out.csv
done
