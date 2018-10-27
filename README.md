# BranchAndBound

This repository contains the code for the implementation of a parallelized branch and bound algorithm. The code is written for a CPU with atleast 12 cores. the implementation uses OpenMP to parallelize the search among different threads.

# Problem solution formulation
Finding an optimal solution to the discrete programming problem involves dispatching threads as nodes. A nodes produces two children nodes if it is a better solution than our current objective. The pseudocode is as follows:

```
Pseudocode: Branch and Bound
Start
Sort Knapsack in ascending order (heuristic)
Create nodeArray with initial solution
for each node in nodeArray:
  if child_node’s value > maxValue and child_node’s weight < max_Weight:
        maxProfit = child node weight
  Find child node’s Bound (upper bound of possible weight reached) 
  If(child_node_bound > max_Value and ch):
     Add child_node to nodeArray
  Add sibling of current node to the node Array  

```

# Running the code
1. create a Cpp executable file by running
```
g++ -fopenmp -o knapSack knapSack.cpp
```
2. run the ompKnapSack.sh script which will run the executable in a SLURM processors in an HPC.

For more information regarding the alogorithm and the code , please see the wiki

# References:

The code for knapsack problem on a single processor was obtained from https://www.geeksforgeeks.org/implementation-of-0-1-knapsack-using-branch-and-bound/ . The problem context was changed and made to run on multiple processors.
