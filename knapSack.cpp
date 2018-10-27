/* Akshay Aravamudan
   15 April 2018 */

/* Inclusions */
#include <omp.h> /* OpenMP */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* prints */
#include <queue>
#include <algorithm>
#include <string.h>

/*---------------------------------------------

g++ -fopenmp -o knapSack knapSack.cpp

./knapSack  -n Nbins -s randomSeed -debug 

-----------------------------------------------*/

struct Product {
    double weight;
    double profit;
};
struct Node {
    int level, profit, bound;
    double weight;
};
int comparison(Product elem1, Product elem2)
{

    if ((elem1.profit) / (elem1.weight) > (elem2.profit) / (elem2.weight)) {
        return 1;
    }
    return 0;
}

int bound(Node u, int n, int W, Product arr[])
{
    // if weight overcomes the knapsack capacity, return
    // 0 as expected bound
    if (u.weight >= W)
        return 0;

    // initialize bound on profit by current profit
    int profit_bound = u.profit;

    // start including items from index 1 more to current
    // item index
    int j = u.level + 1;
    int totweight = u.weight;

    // checking index condition and knapsack capacity
    // condition
    while ((j < n) && (totweight + arr[j].weight <= W)) {
        totweight += arr[j].weight;
        profit_bound += arr[j].profit;
        j++;
    }

    // If j is not n, include last item partially for
    // upper bound on profit
    if (j < n)
        profit_bound += (W - totweight) * arr[j].profit / arr[j].weight;

    return profit_bound;
}

int main(int argc, char* argv[])
{

    /* declarations */
    int Nbins = 10, i, j, debug = 0, arg_index = 0;
    long long seed = 5;

    /* timers */
    double startTime = 0.0, endTime = 0.0;
    std::vector<Node> mainQueue;
    std::vector<Node> additionQueue;
    double maxProfit = 0.0, maxWeight = 0.0;
    Product* elementList = NULL;
    int W = 100, Qsize = 0;
    Node u, v;

    // reading in arguments if they exist
    while (arg_index < argc) {

        /* Optionally redefine maximum bins from command line */
        if (strcmp(argv[arg_index], "-n") == 0) {
            arg_index++;
            Nbins = atoi(argv[arg_index++]);
        }
        /* Optionally define random number seed from command line */
        else if (strcmp(argv[arg_index], "-s") == 0) {
            arg_index++;
            seed = atoi(argv[arg_index++]);
        }
        /* Optionally define random number seed from command line */
        else if (strcmp(argv[arg_index], "-w") == 0) {
            arg_index++;
            W = atoi(argv[arg_index++]);
        }

        /* Optionally define Debug mode */
        else if (strcmp(argv[arg_index], "-debug") == 0) {
            arg_index++;
            debug = 1;
        }
        else
            arg_index++;
    }
    u.level = -1;
    u.profit = u.weight = 0;
    mainQueue.push_back(u);
    printf("Nbins value = %d \n", Nbins);
    printf("seed value = %d \n", seed);
    printf("Maximum knapsack weight = %d \n", W);
    elementList = new Product[Nbins];
    srand(seed);

    for (i = 0; i < Nbins; i++) {
        Product obj;
        obj.profit = ((double)((rand()) % 1000) / 1000) * 80;
        obj.weight = ((double)((rand()) % 1000) / 1000) * 5;
        elementList[i] = obj;
    }
    for (i = 0; i < Nbins; i++) {
        printf("%f\t", elementList[i].profit);
        printf("%f \n", elementList[i].weight);
    }
    printf("\n");
    std::sort(elementList, elementList + Nbins, comparison);

    for (i = 0; i < Nbins; i++) {
        printf("%f\t", elementList[i].profit);
        printf("%f \n", elementList[i].weight);
    }

    // starting timer
    startTime = omp_get_wtime();

    while (!mainQueue.empty()) {
        Qsize = mainQueue.size();
#pragma omp parallel for reduction(max : maxProfit) shared(mainQueue, additionQueue) private(u, v) schedule(dynamic, 50)
        for (i = 0; i < Qsize; i++) {
            u = mainQueue[i];
            // If it is starting node, assign level 0
            if (u.level == -1)
                v.level = 0;

            // If there is nothing on next level
            if (u.level == Nbins - 1)
                continue;

            // Else if not last node, then increment level,
            // and compute profit of children nodes.
            v.level = u.level + 1;

            // Taking current level's item add current
            // level's weight and value to node u's
            // weight and value
            v.weight = u.weight + elementList[v.level].weight;
            if (v.weight > maxWeight)
                maxWeight = v.weight;
            v.profit = u.profit + elementList[v.level].profit;

            // If cumulated weight is less than W and
            // profit is greater than previous profit,
            // update maxprofit
            if (v.weight <= W && v.profit > maxProfit) {
                maxProfit = v.profit;
            }
            // Get the upper bound on profit to decide
            // whether to add v to Q or not.
            v.bound = bound(v, Nbins, W, elementList);

            // If bound value is greater than profit,
            // then only push into queue for further
            // consideration
            if (v.bound > maxProfit) {
				#pragma omp critical
                {
                    additionQueue.push_back(v);
                }
            }
            // Do the same thing,  but Without taking
            // the item in knapsack
            v.weight = u.weight;
            v.profit = u.profit;
            v.bound = bound(v, Nbins, W, elementList);
            if (v.bound > maxProfit) {
				#pragma omp critical
                {
                    additionQueue.push_back(v);
                }
            }
        }

        mainQueue = additionQueue;
        additionQueue.clear();
    }

    //ending timer
    endTime = omp_get_wtime();
    /* output solution time */
    printf("\n runtime = %.16e\n", endTime - startTime);
    printf("The maximum profit reached is %f \n", maxProfit);
    printf("The maximum weight reached is %f \n", maxWeight);
    delete[] elementList;
}
