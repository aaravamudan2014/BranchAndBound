#!/bin/bash
#
## SLURM submission script for OpenMP job helloWorld
## change cpusx`-per-task to change number of threads
#SBATCH --job-name=branchandbound_omp
#SBATCH --output=BBomp.%J.out
#SBATCH --error=BBomp.%J.err
#
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=12
#SBATCH --time=5:00
#SBATCH --partition=class

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
## change n to modify number of bins that can be put into knapsack
## change s to change seed for random number generator
## change w to maximum allowable weight knapsack can take
./knapSack -n 35 -s 12121 -w 40